FROM chrissx/gcc-x86_64-none-elf:11

RUN apt install -y nasm xorriso grub-pc-bin grub-common

VOLUME /amplos
WORKDIR /amplos
