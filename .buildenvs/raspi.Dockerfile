FROM chrissx/gcc-aarch64-none-elf:11

RUN apt install -y python3-pip imagemagick clang llvm lld
RUN pip3 install Pillow bitarray

VOLUME /amplos
WORKDIR /amplos
