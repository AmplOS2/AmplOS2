FROM chrissx/llvm:latest

RUN apt install -y python3 curl

VOLUME /amplos
WORKDIR /amplos
