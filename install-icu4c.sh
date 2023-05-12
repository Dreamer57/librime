#!/bin/bash
set -ev

RIME_ROOT="$(cd "$(dirname "$0")"; pwd)"

icu4c_version="${icu4c_version=72.1}"
icu4c_x_y_z="${icu4c_version//./_}"
icu4c_version_dash="${icu4c_version//./-}"

icu4c_ROOT="${icu4c_ROOT=${RIME_ROOT}/deps/icu4c_${icu4c_x_y_z}}"

icu4c_tarball="icu4c-${icu4c_x_y_z}-src.tgz"
download_url="https://github.com/unicode-org/icu/releases/download/release-${icu4c_version_dash}/${icu4c_tarball}"
icu4c_tarball_sha512sum_72_1='848c341b37c0ff077e34a95d92c6200d5aaddd0ee5e06134101a74e04deb08256a5e817c8aefab020986abe810b7827dd7b2169a60dacd250c298870518dcae8  icu4c-72_1-src.tgz'
icu4c_tarball_sha512sum="${icu4c_tarball_sha512sum=${icu4c_tarball_sha512sum_72_1}}"

download_icu4c_source() {
    cd "${RIME_ROOT}/deps"
    if ! [[ -f "${icu4c_tarball}" ]]; then
        curl -LO "${download_url}"
    fi
    echo "${icu4c_tarball_sha512sum}" | shasum -a 512 -c
    tar -xzvf "${icu4c_tarball}" icu/source
    cp -r icu/source $icu4c_ROOT
    rm -rf icu
}

icu4c_libs="${icu4c_libs=filesystem,regex,system}"
# dr57
# icu4c_cxxflags='-arch arm64 -arch x86_64'
icu4c_cxxflags='-arch arm64'
# dr57 end

build_icu4c_macos() {
    cd "${icu4c_ROOT}"
    ./runConfigureICU MacOSX
    make
}

if [[ "$OSTYPE" =~ 'darwin' ]]; then
   if ! [[ -f "${icu4c_ROOT}/bootstrap.sh" ]]; then
       download_icu4c_source
   fi
   build_icu4c_macos
fi