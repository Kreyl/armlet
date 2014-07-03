#!/usr/bin/env python
from Levenshtein import distance

from EmotionProcessor import processEmotions, guessEmotion

EMOTION_GUESS_RANGE = 3
EMOTION_GUESS_MAX_PROPOSALS = 9

def guessStatistics(emotions):
    mn = [2 ** 31, []]
    mx = [-1, []]
    for (i, a) in enumerate(emotions):
        for b in emotions[i + 1:]:
            d = distance(a, b)
            assert d
            if d < mn[0]:
                mn = [d, [(a, b)]]
            elif d == mn[0]:
                mn[1].append((a, b))
            if d > mx[0]:
                mx = [d, [(a, b)]]
            elif d == mx[0]:
                mx[1].append((a, b))
    print "Minimum %2d: %s" % (mn[0], ' '.join('%s-%s' % ab for ab in mn[1]))
    print "Maximum %2d: %s" % (mx[0], ' '.join('%s-%s' % ab for ab in mx[1]))

def main():
    emotions = sorted(processEmotions()[0])
    #guessStatistics(emotions)
    while True:
        e = None
        while not e:
            e = raw_input("\nInput: ").strip()
        _emotionToUse = guessEmotion(emotions, e)

if __name__ == '__main__':
    main()
