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
./install_vcpkg.sh

echo "Installation complete for Fedora."
