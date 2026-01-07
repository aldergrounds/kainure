FROM ubuntu:18.04

ARG GCC_VERSION=11
ARG CMAKE_VERSION=4.2.0

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
    gcc-${GCC_VERSION} \
    g++-${GCC_VERSION} \
    gcc-${GCC_VERSION}-multilib \
    g++-${GCC_VERSION}-multilib \
    linux-libc-dev:i386 \
    libc6-dev:i386 \
    && rm -rf /var/lib/apt/lists/*

RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-${GCC_VERSION} 100 \
    --slave /usr/bin/g++ g++ /usr/bin/g++-${GCC_VERSION} \
    --slave /usr/bin/gcov gcov /usr/bin/gcov-${GCC_VERSION}

RUN wget -q https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-linux-x86_64.sh \
    && chmod +x cmake-${CMAKE_VERSION}-linux-x86_64.sh \
    && ./cmake-${CMAKE_VERSION}-linux-x86_64.sh --prefix=/usr/local --skip-license \
    && rm cmake-${CMAKE_VERSION}-linux-x86_64.sh

WORKDIR /workspace
