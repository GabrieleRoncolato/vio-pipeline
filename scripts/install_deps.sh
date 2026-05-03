#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
DATA_DIR="$REPO_ROOT/data"
GTSAM_BUILD_DIR="/tmp/gtsam-vio-build"

EUROC_URL="https://www.research-collection.ethz.ch/bitstreams/7b2419c1-62b5-4714-b7f8-485e5fe3e5fe/download"
EUROC_MARKER="$DATA_DIR/machine_hall/MH_01_easy/mav0/cam0/data.csv"

trap 'echo "ERROR: install_deps.sh failed." >&2' ERR

log(){
    echo "==> $*"
}

install_recent_cmake(){
    if command -v cmake >/dev/null && \
       dpkg --compare-versions "$(cmake --version | head -n1 | awk '{print $3}')" ge 3.30; then
        log "CMake $(cmake --version | head -n1 | awk '{print $3}') already installed, skipping."
        return
    fi
    log "Installing CMake ≥ 3.30 from Kitware apt repo..."
    sudo apt-get install -y ca-certificates gpg wget
    wget -qO - https://apt.kitware.com/keys/kitware-archive-latest.asc \
        | gpg --dearmor \
        | sudo tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null
    UBUNTU_CODENAME="$(. /etc/os-release && echo "$VERSION_CODENAME")"
    echo "deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ ${UBUNTU_CODENAME} main" \
        | sudo tee /etc/apt/sources.list.d/kitware.list >/dev/null
    sudo apt-get update
    sudo apt-get install -y cmake
}

install_apt_deps(){
    log "Installing apt packages..."
    sudo apt-get update
    sudo apt-get install -y \
        build-essential \
        git \
        wget \
        unzip \
        python3-pip \
        libeigen3-dev \
        libopencv-dev \
        libboost-all-dev \
        libtbb-dev \
        libsuitesparse-dev
}

build_gtsam(){
    if find /usr/local/lib -name 'libgtsam*' 2>/dev/null | grep -q .; then
        log "GTSAM already installed, skipping."
        return
    fi
    log "Building GTSAM 4.2 from source (~20 min)..."
    rm -rf "$GTSAM_BUILD_DIR"
    git clone --depth 1 --branch 4.2.0 https://github.com/borglab/gtsam.git "$GTSAM_BUILD_DIR"
    (
        cd "$GTSAM_BUILD_DIR"
        mkdir build
        cd build
        cmake .. \
            -DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
            -DCMAKE_BUILD_TYPE=Release \
            -DGTSAM_BUILD_EXAMPLES_ALWAYS=OFF \
            -DGTSAM_BUILD_TESTS=OFF \
            -DGTSAM_WITH_TBB=ON
        make -j"$(nproc)"
        sudo make install
        sudo ldconfig
    )
    rm -rf "$GTSAM_BUILD_DIR"
}

install_python_tools(){
    log "Installing python analysis tools..."
    pip3 install --user --upgrade \
        numpy matplotlib scipy transforms3d evo opencv-python
}

download_dataset(){
    if [ -f "$EUROC_MARKER" ]; then
        log "EuRoC MH_01 dataset already extracted, skipping."
        return
    fi
    log "Downloading EuRoC MH_01_easy (~12 GB)..."
    mkdir -p "$DATA_DIR"
    (
        cd "$DATA_DIR"
        wget -O download "$EUROC_URL"
        log "Extracting MH_01_easy..."
        unzip -q download "machine_hall/MH_01_easy/*" -d "$DATA_DIR"
        unzip -q "$DATA_DIR/machine_hall/MH_01_easy/MH_01_easy.zip" "mav0/*" \
            -d "$DATA_DIR/machine_hall/MH_01_easy"
        log "Removing archive files..."
        rm -f download
        rm -f "$DATA_DIR/machine_hall/MH_01_easy/MH_01_easy.zip"
    )
}

main(){
    install_recent_cmake
    install_apt_deps
    build_gtsam
    install_python_tools
    download_dataset
    log "All dependencies installed."
}

main "$@"
