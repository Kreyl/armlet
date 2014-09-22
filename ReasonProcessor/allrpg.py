#!/usr/bin/python
#
# allrpg.info API
#
# Requires xlrd package from https://pypi.python.org/pypi/xlrd
# On Windows, a newer version of sqlite3.dll must be downloaded from http://sqlite.org/download.html and put to C:\Python27\DLLs
#
from cookielib import Cookie, LWPCookieJar
from os import listdir
from os.path import expanduser, getmtime, isfile, join
from platform import system
from sqlite3 import connect
from urllib2 import build_opener, HTTPCookieProcessor, Request

try:
    from xlrd import open_workbook
except ImportError, ex:
    raise ImportError("%s: %s\n\nPlease install xlrd v0.9.3 or later: https://pypi.python.org/pypi/xlrd\n" % (ex.__class__.__name__, ex))

FIREFOX_PROFILES_LINUX = '~/.mozilla/firefox'
FIREFOX_PROFILES_WINDOWS = r'~\Application Data\Mozilla\Firefox\Profiles'
FIREFOX_COOKIE_FILE = 'cookies.sqlite'
FIREFOX_COOKIES_SQL_REQUEST = "SELECT host, path, isSecure, expiry, name, value FROM moz_cookies WHERE host LIKE '%%%s%%'"

def getFirefoxCookies(domain = ''):
    """Returns Mozilla Firefox cookies for the specified domain."""
    isWindows = system().lower().startswith('win')
    profilesPath = unicode(expanduser(FIREFOX_PROFILES_WINDOWS if isWindows else FIREFOX_PROFILES_LINUX))
    cookieFiles = (join(profilesPath, f, FIREFOX_COOKIE_FILE) for f in listdir(profilesPath))
    cookieDB = sorted((f for f in cookieFiles if isfile(f)), key = getmtime)[-1]
    cursor = connect(cookieDB).cursor()
    cursor.execute(FIREFOX_COOKIES_SQL_REQUEST % domain)
    cookieJar = LWPCookieJar()
    for (host, path, isSecure, expiry, name, value) in cursor.fetchall():
        cookieJar.set_cookie(Cookie(0, name, value, None, False,
            host, host.startswith('.'), host.startswith('.'),
            path, False, isSecure, expiry, not expiry, None, None, {}))
    return cookieJar

COOKIE_SOURCES = {'firefox': getFirefoxCookies}

COOKIE_DOMAIN = 'allrpg.info'

ALL_ROLES_URL = 'http://site.allrpg.info/orders/site=%d/action=export-xlsx-single'

def loadWithCookies(url, cookieJar, userAgent = 'Mozilla/4.0 (compatible; MSIE 5.5; Windows NT)'):
    """Returns a document download from the specified URL, accessed with specified cookies."""
    opener = build_opener(HTTPCookieProcessor(cookieJar))
    return opener.open(Request(url, None, {'User-Agent' : userAgent})).read()

def getAllRoles(gameID, cookieSource = 'firefox'):
    """Returns all roles data for a game with specified ID."""
    cookieJar = COOKIE_SOURCES[cookieSource.lower()](COOKIE_DOMAIN)
    allRolesXLS = loadWithCookies(ALL_ROLES_URL % gameID, cookieJar)
    xls = open_workbook(file_contents = allRolesXLS)
    sheet = xls.sheets()[0]
    return tuple(tuple(cell.value for cell in sheet.row(row)) for row in xrange(sheet.nrows))
