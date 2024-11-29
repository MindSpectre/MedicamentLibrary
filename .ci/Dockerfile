FROM ubuntu:latest

ENV DEBIAN_FRONTEND=noninteractive
ENV VCPKG_ROOT=/opt/vcpkg
ENV GIT_SSL_VERSION tlsv1.2

# Step 1: Install system dependencies
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

# Step 3: Install and bootstrap vcpkg
RUN git clone https://github.com/microsoft/vcpkg.git $VCPKG_ROOT && \
    cd $VCPKG_ROOT && \
    git pull && \
    ./bootstrap-vcpkg.sh && \
    $VCPKG_ROOT/vcpkg install \
        boost-system \
        boost-filesystem \
        boost-log \
        boost-program-options \
        grpc \
        jsoncpp \
        valijson \
        libpq \
        libuuid \
        gtest \
        libpqxx \
        drogon && \
    rm -rf $VCPKG_ROOT/buildtrees $VCPKG_ROOT/downloads $VCPKG_ROOT/packages

# Step 4: Set working directory
WORKDIR /workspace

# Step 5: Copy project files (this step changes often)
COPY . /workspace

# Step 6: Set default command
CMD ["/bin/bash"]
