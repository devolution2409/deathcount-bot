

get stream url with:

https://pwn.sh/tools/getstream.html


open m3u8 with ffmpeg to get screenshots:


get stream url using:
streamlink twitch.tv/forsen best --stream-url

export STREAM_URL=https://video-weaver.fra02.hls.ttvnw.net/v1/playlist/CrUD5MrMsL6WQ_ZMzVqZOjLFk9MMosWjXw1w2Q4hD0-SPXE6zhg4HfrqNODWjLisgloxJmyXetfd8yGmpfD5dLSHRAiI7FlFJHzn8LjTQ7VPQhm_I6wdsEzH6XfE2TH2hnc6aI5eLxxepGI0Ac2QbO0025Oslq43S9i-W161_l-aPXpDJtWnnpRkZsnnGZ5b7hn3lzSBFyEgilNrCuCrFaHEAEiz4vb1p0WM3rdd5CjgPXVy2GPXpZqMt2w9IC4JF7AUNmSJi6N9mpWDO3Vi8b4aDi4CjsmVpoGwToo0265wH73wYc0t0iHk-ivBCTE4n3Cp3OX1AJQPGlAv_P6U96Ta7f_1AIkR5Decy7jht7YW4mcoh0bWd7EcTC2R9Oynlruhzu2vEVzgIcQz3Oeq7tneb1erZEfIwPnNStwboiqC4N503upBxWF8vC7oUQffGiYevMz3wqEZphGZlnTf5OU7qcxdOFsK_QL70zPojqkP0iqFeUC8dI-D3zBQWEg_i8lhw387PrZLRrYFKWyzOHSHQVTpZDbkmCN6nKn6lNbdqfzVyTFCQ-1RKtK5w6RfBIfikJtw-TcSEEdfq2c_u6asW4dX5kxHJOMaDPhk27bkRLIQ3kAJ_g.m3u8

ffmpeg -i $STREAM_URL -vframes 1 -q:v 2 test.jpg )


or without vframes but with -vf fps=1 -q:v 2 pog%03d.jpg


to capture every 1 second

Idea: every x frames ( so if streamm is 1080p60fp ) for instance 10 fps

check with machine learning (or plain comparison) if the frame is a DETH 

if DETH, deth++;


CAN USE https://transfer.sh/ to upload thing


can also use ffmpeg -i $STREAM_URL -f segment -segment_time 10 -c:v libx264 -crf 20 -an "LUL%03d.ts"
to generate 10 secs extracts, mb we can use it? PepeS
without sound


idea:

1 thread to generate the 5/10 secs extracts
1 thread to turn them into frame
1 thread to analyse using machine learning?

GachiPls DETH


https://www.learnopencv.com/deep-learning-based-text-detection-using-opencv-c-python/
https://www.pyimagesearch.com/2018/08/20/opencv-text-detection-east-text-detector/
https://www.pyimagesearch.com/2017/07/10/using-tesseract-ocr-python/

TODO: move the sh in dockerfile as entrypoint
