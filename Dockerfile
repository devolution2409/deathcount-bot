#FROM debian:latest

FROM ubuntu:latest
#trying new shit

RUN apt-get update
#  compiler
RUN apt-get -y install build-essential
#maybe uneeded
RUN apt-get -y install ffmpeg
# && apt-get -y install curl && apt-get -y install vim && apt-get -y install g++ &

#required
RUN apt-get -y install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev

#optional
RUN apt-get -y install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libdc1394-22-dev

#libjasper isn't available for older ubuntu and the following doesnt work, fuck it.
#RUN echo "deb http://us.archive.ubuntu.com/ubuntu/ yakkety universe" | tee -a /etc/apt/sources.list &&  apt-get -y install libjasper-dev

WORKDIR /source/

RUN git clone https://github.com/opencv/opencv.git && git clone https://github.com/opencv/opencv_contrib.git

#COPY ./bashrc /root/

RUN echo 'export PS1="\[\033[36m\]\u\[\033[m\]@\[\033[32m\]\h:\[\033[33;1m\]\w\[\033[m\] \\$ " ' >> ~/.bashrc


WORKDIR /source/opencv/build

RUN cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local -DOPENCV_EXTRA_MODULES_PATH=/source/opencv_contrib/modules /source/opencv 

RUN make -j 4
RUN make install
#is this needed now??
RUN apt-get -y download libc-bin && dpkg -x libc-bin*.deb unpackdir/ &&  cp unpackdir/sbin/ldconfig /sbin/ && apt-get install --reinstall libc-bin
RUN apt-get -y install xvfb 

#COPY ./src/  /source

WORKDIR /source/

RUN Xvfb :322 &
RUN export DISPLAY=:322
#COPY entrypoint.sh /root/entrypoint.sh

#RUN sed -i 's/geteuid/getppid/' /usr/bin/vlc

#todo: try with a shell script FeelsDankMan
#doesnt work with entrypoint either
#Xvfb :1

CMD ["bash"]