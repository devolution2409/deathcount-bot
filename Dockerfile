FROM debian:latest

RUN apt-get update && apt-get -y install ffmpeg && apt-get -y install curl && apt-get -y install vim && apt-get -y install g++ 
RUN apt-get -y install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
# && apt-get -y install python && apt-get -y install python-pip && apt-get install python-tk
WORKDIR /opt/
RUN git clone https://github.com/opencv/opencv.git && git clone https://github.com/opencv/opencv_contrib.git 
WORKDIR /opt/opencv
RUN	mkdir release
WORKDIR /opt/opencv/release
RUN 	cmake -D BUILD_TIFF=ON -D WITH_CUDA=OFF -D ENABLE_AVX=OFF -D WITH_OPENGL=OFF -D WITH_OPENCL=OFF -D WITH_IPP=OFF -D WITH_TBB=ON -D BUILD_TBB=ON -D WITH_EIGEN=OFF -D WITH_V4L=OFF -D WITH_VTK=OFF -D BUILD_TESTS=OFF -D BUILD_PERF_TESTS=OFF -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D OPENCV_EXTRA_MODULES_PATH=/opt/opencv_contrib/modules /opt/opencv/
RUN 	apt-get clean && apt-get autoclean
#RUN	make -j4 && make install && ldconfig

#RUN apt-get -y install python && apt-get -y install python-pip && apt-get install python-tk
RUN apt install -y libtesseract-dev 
RUN apt install -y libleptonica-dev
#RUN pip install scikit-image

COPY ./bashrc /root/
RUN cat ~/bashrc ~/.bashrc > .bashrc

WORKDIR /test/
COPY ./source/*  /test/


#RUN sed -i 's/geteuid/getppid/' /usr/bin/vlc

CMD ["bash"]
