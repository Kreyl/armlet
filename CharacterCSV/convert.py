from os import listdir
from os.path import join

PREFIX = 'character_'

SOURCE_DIR = '.'
TARGET_DIR = 'E:/YandexDisk/AtlantisMusic/_CSV'

for fileName in listdir(SOURCE_DIR):
    if fileName.startswith(PREFIX):
        with open(join(SOURCE_DIR, fileName)) as f:
            with open(join(TARGET_DIR, fileName.replace(PREFIX, '')), 'w') as g:
                g.write('\n'.join(sorted(line.strip() for line in f if not line.startswith('#'))) + '\n')
