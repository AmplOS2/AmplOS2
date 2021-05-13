#!/bin/sh
target=$1
tag=amplos-$target-buildenv
shift
docker images | grep amplos-raspi-buildenv >/dev/null || \
    docker build buildenvs -f buildenvs/$target.Dockerfile -t $tag
docker run --rm -it -v $PWD:/amplos $tag
