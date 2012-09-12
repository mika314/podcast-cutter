#!/bin/bash
pushd
cd `dirname $0`
root_dir=`pwd`
popd
ffmpeg -i "$1" -ac 1 -f s16le -ar 44100 - | $root_dir/podcast_cutter/bin/podcast_cutter 2> "`basename \"$1\" .m4a`.txt" | ffmpeg -ac 1 -f s16le -ar 44100 -i - -ab 128k -y "`basename \"$1\" .m4a`.mp3" 