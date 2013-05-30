#!/bin/bash
avconv -i $1 -ac 1 -f s16le -ar 44100 -y qq
pv qq | podcast_cutter/podcast_cutter > zz 2> rmlist
avconv -ac 1 -f s16le -ar 44100 -i zz -y kk.wav
avconv -i $1 -f yuv4mpegpipe - | frame_remover/frame_remover | avconv -f yuv4mpegpipe -i - -b 2500k -threads 8 -y les.mp4
avconv -i les.mp4 -i kk.wav -map 0:0 -map 1:0 -vcodec copy -ab 256k `basename $1 .mp4`_mix.mp4
