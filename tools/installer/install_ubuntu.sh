#!/bin/bash
# Ubuntu installation script for project dependencies

set -e  # Exit immediately if a command exits with a non-zero status

echo "Installation starts for Ubuntu."

# Update the package list, but don't exit if it fails
sudo apt-get update || true
# Install necessary packages
sudo apt-get install -y \
    build-essential \
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
    ninja-build

echo "Base packages installation completed."

VCPKG_ROOT="/opt/vcpkg"

if [ ! -d "$VCPKG_ROOT" ]; then
    echo "Cloning vcpkg repository..."
    sudo git clone https://github.com/Microsoft/vcpkg.git "$VCPKG_ROOT"
    sudo chown -R "$USER":"$USER" "$VCPKG_ROOT"
fi

cd "$VCPKG_ROOT" || exit

echo "Updating vcpkg..."
git pull origin master

echo "Bootstrapping vcpkg..."
./bootstrap-vcpkg.sh

# Install dependencies from Vcpkg
echo "Installing vcpkg packages..."
./vcpkg install \
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
    drogon \
    abseil \
    libevent
echo "Installation complete for Ubuntu."