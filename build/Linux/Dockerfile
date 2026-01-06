FROM ubuntu:20.04
ARG CMAKE_VERSION=3.27.7
ENV DEBIAN_FRONTEND=noninteractive

RUN dpkg --add-architecture i386 && \
    apt-get update && \
    apt-get install -y --no-install-recommends \
        software-properties-common \
        wget \
        ca-certificates \
        gnupg \
    && add-apt-repository ppa:ubuntu-toolchain-r/test -y && \
    apt-get update

RUN apt-get install -y --no-install-recommends \
    build-essential \
    make \
    gcc-11 \
    g++-11 \
    gcc-11-multilib \
    g++-11-multilib \
    linux-libc-dev:i386 \
    libc6-dev:i386 \
    && rm -rf /var/lib/apt/lists/*

RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 100 \
    --slave /usr/bin/g++ g++ /usr/bin/g++-11 \
    --slave /usr/bin/gcov gcov /usr/bin/gcov-11

RUN wget -q https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-linux-x86_64.sh \
    && chmod +x cmake-${CMAKE_VERSION}-linux-x86_64.sh \
    && ./cmake-${CMAKE_VERSION}-linux-x86_64.sh --prefix=/usr/local --skip-license \
    && rm cmake-${CMAKE_VERSION}-linux-x86_64.sh

WORKDIR /workspace