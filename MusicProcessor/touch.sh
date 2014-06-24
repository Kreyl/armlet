#!/bin/bash
pushd $ATLANTIS_MUSIC
for f in `find . -name music_ok -o -name music_errors` ; do touch $f ; done
popd
