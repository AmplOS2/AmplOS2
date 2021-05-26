FROM chrissx/llvm:latest

RUN apt install -y python3 curl wget
RUN wget https://github.com/Amplus2/Adscript/archive/refs/heads/master.tar.gz && \
    tar xzf master.tar.gz && \
    cd Adscript-master && \
    make -j$(nproc) install && \
    cd .. && \
    rm -rf Adscript-master master.tar.gz

VOLUME /amplos
WORKDIR /amplos
