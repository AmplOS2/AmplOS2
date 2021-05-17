FROM chrissx/gcc-x86_64-none-elf:11

RUN apt install -y nasm xorriso grub-pc-bin grub-common python3-pip imagemagick
RUN pip3 install Pillow bitarray

VOLUME /amplos
WORKDIR /amplos
