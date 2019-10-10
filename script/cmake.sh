#!/bin/bash
build_type=${1:-Debug}
compiler=${2:-gcc}

cd "$(realpath "${BASH_SOURCE[0]%/*}")/.."

source_dir="$(pwd)"
build_dir="build/$compiler/$build_type"

# cookbook is very old, it use FIND_PATH to search boot head files and library,
# i need to setup CMAKE_PREFIX_PATH to help it find the proper library

(
cd "$build_dir" && \
       cmake \
         -DCMAKE_PREFIX_PATH:STRING='/usr/local/osg/gl2;/usr/local/osg/gl2/lib' \
         -DBUILD_INTEGRATIONS=OFF \
         -DDESIRED_QT_VERSION:STRING=4 \
         -DBUILD_SHARED_LIBS:BOOLEAN=ON \
         -DCMAKE_CXX_FLAGS:STRING= \
         -DCMAKE_BUILD_TYPE:STRING="$build_type" \
         -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOLEAN=ON \
         "$source_dir"
)

ln -fs "$build_dir"/compile_commands.json compile_commands.json
