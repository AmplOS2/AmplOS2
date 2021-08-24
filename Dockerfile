FROM chrissx/llvm:latest

RUN apt install -y python3 curl wget

VOLUME /amplos
WORKDIR /amplos
