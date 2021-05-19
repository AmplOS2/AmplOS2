FROM chrissx/llvm:latest

RUN apt install -y python3

VOLUME /amplos
WORKDIR /amplos
