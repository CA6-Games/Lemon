#!/bin/bash

CURRENT_DIR=$(pwd)

# Detect the distribution and install the appropriate package
if [ -f /etc/os-release ]; then
    . /etc/os-release
    case "$ID" in
        fedora)
            sudo dnf install -y gcc make cmake git nano
            sudo dnf install -y SDL2 SDL2_image SDL2_mixer SDL2_ttf SDL2_net
            sudo dnf install -y SDL2-devel SDL2_image-devel SDL2_mixer-devel SDL2_ttf-devel SDL2_net-devel
            ;;
        *)
            echo "Unsupported distribution: $ID"
            exit 1
            ;;
    esac
else
    echo "Cannot detect the distribution. Neither /etc/os-release file found."
    exit 1
fi

mkdir ~/Workspaces
mkdir ~/Workspaces/toolchain
cd ~/Workspaces/toolchain

# Install Emscripten
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ~/Workspaces/toolchain/emsdk/emsdk_env.sh

# Download SDL2 source
cd ~/Workspaces/toolchain
git clone https://github.com/libsdl-org/SDL.git
cd SDL
git checkout release-2.26.0
mkdir build
cd build
emcmake cmake ..
emmake make

