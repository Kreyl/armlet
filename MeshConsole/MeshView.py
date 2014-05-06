#!/usr/bin/python
#
# Mesh Console
# Device table visualization
#
try:
    from PyQt4.QtCore import Qt, QAbstractTableModel, QVariant
    from PyQt4.QtGui import QAction, QPalette, QStyle
    from PyQt4.QtGui import QItemDelegate, QItemSelection, QSortFilterProxyModel, QTableView
except ImportError, ex:
    raise ImportError("%s: %s\n\nPlease install PyQt4 v4.10.4 or later: http://riverbankcomputing.com/software/pyqt/download\n" % (ex.__class__.__name__, ex))

FONT_METRICS_CORRECTION = 1.1

INVALID_DATA = QVariant()

RAW_ROLE = Qt.UserRole
CHANGED_ROLE = Qt.UserRole + 1
ACCENT_ROLE = Qt.UserRole + 2

# Column types
CONST = 0
RAW = 1
PROC = 2

class Column(object):
    def __init__(self, number, checked, changing, highlight, alignment, name, description, fieldName, longestValue = 999, formatter = None, fmt = None):
        self.number = number
        self.checked = checked
        self.changing = changing
        self.highlight = highlight
        self.alignment = Qt.AlignRight if alignment else Qt.AlignLeft
        self.name = name
        self.description = description
        self.fieldName = fieldName
        self.formatter = formatter
        self.fmt = fmt
        self.longestValue = self.process(longestValue)
        self.defaults = { # This really is a part of View, but moving it off here doesn't work well
            Qt.ToolTipRole: description or name,
            Qt.StatusTipRole: description or name,
            Qt.TextAlignmentRole: self.alignment
        }
        self.headers = {
            Qt.DisplayRole: name,
            Qt.ToolTipRole: description or name,
            Qt.StatusTipRole: description or name,
            Qt.TextAlignmentRole: self.alignment
        }

    def process(self, data):
        if data is None:
            return None
        if self.formatter:
            data = self.formatter(data)
        return self.fmt % data if self.fmt else unicode(data) if data is not None else ''

class ColumnAction(QAction):
    def __init__(self, column, toggleCallback, menu):
        QAction.__init__(self, column.name, menu)
        self.column = column
        self.setCheckable(True)
        self.setChecked(column.checked)
        self.setToolTip(column.description or column.name)
        self.setStatusTip(column.description or column.name)
        toggle = lambda checked: toggleCallback(column.number, not checked)
        toggle(column.checked)
        self.toggled.connect(toggle)

class Cell(dict):
    def __init__(self, device, column):
        dict.__init__(self)
        self[CHANGED_ROLE] = None
        self[RAW_ROLE] = None
        self[ACCENT_ROLE] = None
        self[Qt.DisplayRole] = ''
        self.device = device
        self.column = column

    def setData(self, initial = False):
        if self.column.changing is CONST:
            if initial:
                self[Qt.DisplayRole] = self.column.process(getattr(self.device, self.column.fieldName))
        elif self.column.changing is RAW:
            data = getattr(self.device, self.column.fieldName)
            if data == self[RAW_ROLE]:
                if self.column.highlight:
                    self[CHANGED_ROLE] = False
            else:
                if self.column.highlight:
                    self[CHANGED_ROLE] = not initial
                self[RAW_ROLE] = data
                self[Qt.DisplayRole] = self.column.process(data)
        elif self.column.changing is PROC:
            data = self.column.process(getattr(self.device, self.column.fieldName))
            if self.column.highlight:
                self[CHANGED_ROLE] = False if data == self[Qt.DisplayRole] else not initial
            self[Qt.DisplayRole] = data
        self[ACCENT_ROLE] = self.device.hops

    def getData(self, role):
        data = self.get(role)
        return data if data is not None else self.column.defaults.get(role, INVALID_DATA)

class DevicesModel(QAbstractTableModel):
    def __init__(self, devices, columns, parent):
        QAbstractTableModel.__init__(self, parent)
        self.columns = columns
        self.cache = tuple(tuple(Cell(device, column) for column in columns) for device in devices)
        self.numRows = len(devices)
        self.numColumns = len(columns)
        self.minIndex = self.createIndex(0, 0)
        self.maxIndex = self.createIndex(self.numRows - 1, self.numColumns - 1)
        self.refresh(True)

    def rowCount(self, _parent = None):
        return self.numRows

    def columnCount(self, _parent = None):
        return self.numColumns

    def getDeviceSelection(self, nRow):
        return QItemSelection(self.index(nRow, 0), self.index(nRow, self.columnCount() - 1))

    def headerData(self, section, orientation, role = Qt.DisplayRole):
        if orientation == Qt.Vertical:
            return section
        if section in xrange(len(self.columns)):
            headers = self.columns[section].headers
            ret = headers.get(role)
            if ret is not None:
                return ret
        return INVALID_DATA

    def data(self, index, role = Qt.DisplayRole):
        try:
            return self.cache[index.row()][index.column()].getData(role)
        except LookupError: pass
        except AttributeError: pass
        return INVALID_DATA

    def refresh(self, initial = False):
        for cacheRow in self.cache:
            for cell in cacheRow:
                cell.setData(initial)
        self.dataChanged.emit(self.minIndex, self.maxIndex)

class RoleDefaultSortProxyModel(QSortFilterProxyModel):
    def __init__(self, sourceModel, role = Qt.DisplayRole, parent = None):
        QSortFilterProxyModel.__init__(self, parent)
        self.role = role
        self.setSourceModel(sourceModel)
        self.setDynamicSortFilter(True)

    def lessThan(self, left, right):
        leftData = self.sourceModel().data(left, self.role)
        rightData = self.sourceModel().data(right, self.role)
        return leftData < rightData if leftData != rightData else left.row() < right.row()

class DevicesTableDelegate(QItemDelegate): # QStyledItemDelegate doesn't handle selection background color properly
    def __init__(self, inactivePalette, customPalettes, activePalette, parent):
        QItemDelegate.__init__(self, parent)
        self.inactivePalette = inactivePalette
        self.customPalettes = customPalettes
        self.activePalette = activePalette

    def paint(self, paint, option, index):
        option.palette = self.activePalette if index.data(CHANGED_ROLE).toBool() else self.customPalettes.get(index.data(ACCENT_ROLE).toInt()[0], self.inactivePalette)
        QItemDelegate.paint(self, paint, option, index)

    def drawFocus(self, painter, option, rect):
        option.state &= ~QStyle.State_HasFocus
        QItemDelegate.drawFocus(self, painter, option, rect)

class DevicesTableView(QTableView):
    def configure(self, devicesModel, changedDataSample, accentSamples):
        self.setModel(RoleDefaultSortProxyModel(devicesModel, RAW_ROLE))
        self.columnWidths = tuple(self.fontMetrics().boundingRect(column.longestValue).width() * FONT_METRICS_CORRECTION for column in devicesModel.columns)
        #for column in devicesModel.columns: # It works for width but not for height, find current row height?
        #    column.headers[Qt.SizeHintRole] = QSize(self.fontMetrics().boundingRect(column.longestValue).size().width(), self.rowHeight(0))
        inactivePalette = self.palette()
        inactivePalette.setColor(QPalette.HighlightedText, inactivePalette.color(QPalette.Text))
        activePalette = QPalette(inactivePalette)
        activeColor = changedDataSample.palette().color(QPalette.WindowText)
        activePalette.setColor(QPalette.Text, activeColor)
        activePalette.setColor(QPalette.HighlightedText, activeColor)
        customPalettes = {}
        for (number, sample) in accentSamples.iteritems():
            color = sample.palette().color(QPalette.WindowText)
            palette = QPalette(inactivePalette)
            palette.setColor(QPalette.Text, color)
            palette.setColor(QPalette.HighlightedText, color)
            customPalettes[number] = palette
        self.setItemDelegate(DevicesTableDelegate(inactivePalette, customPalettes, activePalette, self))
        self.resizeRowsToContents()
        self.resizeColumnsToContents()
        header = self.horizontalHeader()
        header.setHighlightSections(False)
        header.setStyleSheet('font-weight: bold')

    def sizeHintForColumn(self, nColumn):
        return self.columnWidths[nColumn]
