FROM ubuntu:22.04
ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update
RUN apt-get install -y g++ cmake git libboost-all-dev libcpprest-dev libaio-dev libcurl4-openssl-dev
WORKDIR /usr/src/
RUN git clone https://github.com/yalcantara/cpplib-core.git
WORKDIR /usr/src/cpplib-web
CMD ["/bin/bash"]