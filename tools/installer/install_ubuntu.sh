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

./install_vcpkg.sh
echo "Installation complete for Ubuntu."