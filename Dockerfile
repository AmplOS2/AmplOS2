FROM chrissx/llvm:latest

RUN apt install -y python3 curl gcc git

RUN curl -L https://github.com/chrissxYT/tools/raw/master/bootstrap | sh

VOLUME /amplos
WORKDIR /amplos
