#!/bin/sh
target=$1
tag=amplos-$target-buildenv
shift
docker images | grep $tag >/dev/null || \
    docker build .buildenvs -f .buildenvs/$target.Dockerfile -t $tag
docker run --rm -v $PWD:/amplos $tag sm $@
