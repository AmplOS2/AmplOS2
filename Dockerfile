FROM chrissx/llvm:latest

ENV DEBIAN_FRONTEND=noninteractive
RUN apt update && apt upgrade -y && apt install -y python3 curl wget npm pandoc

VOLUME /amplos
WORKDIR /amplos
