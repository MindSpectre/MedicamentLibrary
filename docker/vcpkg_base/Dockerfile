# Base Dockerfile (vcpkg-base)
FROM ubuntu:22.04
ENV VCPKG_ROOT=/vcpkg
RUN apt-get update && apt-get install -y \
    cmake \
    bison \
    flex \
    g++ \
    ninja-build \
    git \
    curl \
    unzip \
    zip \
    tar \
    autoconf \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

# Install vcpkg
RUN git clone https://github.com/microsoft/vcpkg.git $VCPKG_ROOT
WORKDIR /vcpkg
RUN git pull
RUN ./bootstrap-vcpkg.sh

# Install common dependencies
COPY vcpkg.json $VCPKG_ROOT/vcpkg.json
RUN ./vcpkg install --clean-after-build

ENV CMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake

# Cleanup
RUN rm -rf /var/lib/apt/lists/*
