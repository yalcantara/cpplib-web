FROM ubuntu:22.04
ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update
RUN apt-get install -y cmake clang git libboost-all-dev libcpprest-dev libaio-dev
WORKDIR /usr/src/
RUN git clone https://github.com/yalcantara/cpplib-core.git
WORKDIR /usr/src/cpplib-web
CMD ["/bin/bash"]