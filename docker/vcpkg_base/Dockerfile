# Base Dockerfile (vcpkg-base)
FROM ubuntu:latest
ENV VCPKG_ROOT=/vcpkg
RUN apt-get update -y && apt-get upgrade -y
RUN apt-get install -y software-properties-common \
    wget \
    flex \
    bison \
    build-essential \
    cmake \
    pkg-config \
    curl \
    libicu-dev \
    uuid-dev \
    mesa-common-dev \
    libglu1-mesa-dev \
    libxkbcommon-x11-0 \
    libssl-dev \
    openssl \
    zlib1g-dev \
    zip \
    unzip \
    autoconf \
    ninja-build \
    git && \
    rm -rf /var/lib/apt/lists/*

# Step 2: Install Clang 19
RUN wget https://apt.llvm.org/llvm.sh && \
    chmod +x llvm.sh && \
    ./llvm.sh 19 && \
    apt-get install -y \
        clang-19 \
        clang++-19 && \
    update-alternatives --install /usr/bin/clang clang /usr/bin/clang-19 100 && \
    update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-19 100 && \
    rm -f llvm.sh
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
