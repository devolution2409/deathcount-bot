#FROM debian:latest

FROM ubuntu:latest
#trying new shit

#mb ffmpeg undeeded
RUN apt-get update && \
    apt-get -y install build-essential \
                        ffmpeg \
                        cmake \
                        git \
                        libgtk2.0-dev \
                        pkg-config \
                        libavcodec-dev \
                        libavformat-dev \
                        libswscale-dev \
                        python-dev \
                        python-numpy \
                        libtbb2 \
                        libtbb-dev \
                        libjpeg-dev \
                        libpng-dev \
                        libtiff-dev \
                        libdc1394-22-dev \
                        libleptonica-dev
                        
 
#libjasper isn't available for older ubuntu and the following doesnt work, fuck it.
#RUN echo "deb http://us.archive.ubuntu.com/ubuntu/ yakkety universe" | tee -a /etc/apt/sources.list &&apt-get -y install libjasper-dev

#leptonica

#building tesseract
WORKDIR /opt/
RUN git clone --single-branch --branch 4.1 https://github.com/tesseract-ocr/tesseract.git 

WORKDIR /opt/tesseract/

RUN ./autogen.sh 
RUN ./configure
RUN make
RUN make install
RUN ldconfig
RUN make training-install


WORKDIR /opt/

RUN git clone https://github.com/opencv/opencv.git && git clone https://github.com/opencv/opencv_contrib.git

#COPY ./bashrc /root/

RUN echo 'export PS1="\[\033[36m\]\u\[\033[m\]@\[\033[32m\]\h:\[\033[33;1m\]\w\[\033[m\] \\$ " ' >> ~/.bashrc


WORKDIR /opt/opencv/build

RUN cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local -DOPENCV_EXTRA_MODULES_PATH=/opt/opencv_contrib/modules /opt/opencv 

RUN make -j 4
RUN make install


WORKDIR /usr/local/share/tessdata
RUN apt-get update && apt-get install -y wget && wget https://github.com/tesseract-ocr/tessdata/raw/master/eng.traineddata
WORKDIR /source/

RUN apt-get install -y software-properties-common && \
add-apt-repository ppa:ubuntu-toolchain-r/test && \
apt-get update -y && \
apt-get install -y gcc-8 g++-8 && \
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 60 --slave /usr/bin/g++ g++ /usr/bin/g++-8
#doesnt work using that font pajapepe guess we have to find a way to train it better
#COPY ./traineddata/OptimusPrinceps.traineddata    /usr/local/share/tessdata/OptimusPrinceps.traineddata

#need to find out how to trian myself 
#COPY ./fonts/OptimusPrinceps /usr/share/fonts/truetype/OptimusPrinceps



CMD ["bash"]