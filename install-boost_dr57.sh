#!/bin/bash
set -ex

RIME_ROOT="$(cd "$(dirname "$0")"; pwd)"

boost_version="${boost_version=1.81.0}"
boost_x_y_z="${boost_version//./_}"

BOOST_ROOT="${BOOST_ROOT=${RIME_ROOT}/deps/boost_${boost_x_y_z}}"

boost_tarball="boost_${boost_x_y_z}.tar.bz2"
download_url="https://boostorg.jfrog.io/artifactory/main/release/${boost_version}/source/${boost_tarball}"
boost_tarball_sha256sum_1_78_0='71feeed900fbccca04a3b4f2f84a7c217186f28a940ed8b7ed4725986baf99fa  boost_1_81_0.tar.bz2'
boost_tarball_sha256sum="${boost_tarball_sha256sum=${boost_tarball_sha256sum_1_78_0}}"

download_boost_source() {
    cd "${RIME_ROOT}/deps"
    if ! [[ -f "${boost_tarball}" ]]; then
        curl -LO "${download_url}"
    fi
    echo "${boost_tarball_sha256sum}" | shasum -a 256 -c
    tar --bzip2 -xf "${boost_tarball}"
    [[ -f "${BOOST_ROOT}/bootstrap.sh" ]]
}

boost_libs="${boost_libs=filesystem,regex,system,locale}"
# dr57
# boost_cxxflags='-arch arm64 -arch x86_64'
# c++11 icu4c要c++11才能正常加载
boost_cxxflags='-arch arm64 -std=c++11'
# icu_root="${RIME_ROOT}/deps/icu4c_72_1"
# dr57 end

build_boost_macos() {
    cd "${BOOST_ROOT}"
    ./bootstrap.sh --with-toolset=clang --with-libraries="${boost_libs}"
    # ./bootstrap.sh --with-toolset=clang --with-libraries="${boost_libs}" --with-icu="${icu_root}"
    ./b2 -q -a link=static architecture=arm cxxflags="${boost_cxxflags}" stage
    # 我吐了，我才发现，我压根就没用上icu
    # ./b2 -q -a link=static toolset=clang architecture=arm cxxflags="${boost_cxxflags}" -sICU_PATH="${icu_root}" stage
    for lib in stage/lib/*.a; do
        lipo $lib -info
    done
}

if [[ $# -eq 0 || " $* " =~ ' --download ' ]]; then
    if [[ ! -f "${BOOST_ROOT}/bootstrap.sh" ]]; then
        download_boost_source
    else
        echo "found boost at ${BOOST_ROOT}"
    fi
fi
if [[ $# -eq 0 || " $* " =~ ' --build ' ]]; then
    if [[ "$OSTYPE" =~ 'darwin' ]]; then
        build_boost_macos
    fi
fi
