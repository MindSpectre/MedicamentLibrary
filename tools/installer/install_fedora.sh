#!/bin/bash
# Fedora installation script for project dependencies

set -e  # Exit immediately if a command exits with a non-zero status

echo "Installation starts for Fedora."

# Update the package list
sudo dnf update -y
# Install necessary packages
sudo dnf install -y \
    @development-tools \
    pkgconf-pkg-config \
    curl \
    haskell-platform \
    libicu-devel \
    libuuid-devel \
    mesa-libGL-devel \
    mesa-libGLU-devel \
    xkeyboard-config \
    openssl-devel \
    zlib-devel \
    zip \
    unzip \
    git \
    perl-IPC-Cmd
sudo dnf install -y autoconf automake libtool

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

echo "Installation complete for Fedora."
