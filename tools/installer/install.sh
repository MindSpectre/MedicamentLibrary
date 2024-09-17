#!/bin/bash
# Ubuntu installation script for project dependencies
echo "Installation starts for Ubuntu."
# Update the package list
sudo apt update

# Install necessary packages
sudo apt install -y zip unzip
sudo apt install -y build-essential pkg-config
sudo apt install -y curl
sudo apt install -y haskell-platform
sudo apt install -y libicu-dev
sudo apt install -y uuid-dev
sudo apt install -y mesa-common-dev
sudo apt install -y libglu1-mesa-dev
sudo apt install -y libxkbcommon-x11-0
sudo apt install -y libssl-dev
sudo apt install -y openssl
sudo apt install -y zlib1g-dev

# Install and set up Vcpkg
sudo git clone https://github.com/Microsoft/vcpkg.git /opt/vcpkg
cd /opt/vcpkg/
sudo git checkout 2023.10.19
sudo ./bootstrap-vcpkg.sh

# Install dependencies from Vcpkg
sudo /opt/vcpkg/vcpkg install boost-system boost-filesystem boost-log boost-program-options grpc jsoncpp valijson libpq libuuid gtest

echo "Installation complete for Ubuntu."


