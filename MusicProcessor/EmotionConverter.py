#!/usr/bin/python
# -*- coding: utf-8 -*-
#
# Emotions Converter library for Ticket to Atlantis LARP.
#
try:
    from pytils.translit import translify
except ImportError, ex:
    raise ImportError("%s: %s\n\nPlease install pytils v0.2.3 or later: https://pypi.python.org/pypi/pytils\n" % (ex.__class__.__name__, ex))

try:
    from unidecode import unidecode
except ImportError, ex:
    raise ImportError("%s: %s\n\nPlease install Unidecode v0.04.16 or later: https://pypi.python.org/pypi/Unidecode/\n" % (ex.__class__.__name__, ex))

TRANSLIFY_PATCHES = {
    u'Ё': u'Е',
    u'ё': u'е',
    u'и\u0306': u'й' # й on Mac
}

EMOTION_PATCHES = {
    'somneniya': 'somnenie',
    'sex': 'seks',
    'napryazhenie': 'napryazhennost\'',
    'nastol\'giya': 'nostal\'giya',
    'sostrodanie': 'sostradanie',
    'udvoletvorenie': 'udovletvorenie',
    'udovletvorennost': 'udovletvorenie',
    'umrovotvorenie': 'umirotvorenie',
    'voodushevlennost\'': 'voodushevlenie'
}

def convert(s):
    for (f, t) in TRANSLIFY_PATCHES.iteritems():
        s = s.replace(f, t)
    ret = ''
    for c in s:
        try:
            c = translify(c)
        except ValueError:
            c = unidecode(c)
        ret += c
    return ret

def convertTitle(s):
    return convert(s.strip())

def convertEmotion(s):
    e = convertTitle(s).lower()
    return EMOTION_PATCHES.get(e, e)
