raspi:
        docker build . -f raspi.Dockerfile -t amplos-raspi-buildenv
        docker run --rm -it -v $$(dirname $$PWD):/amplos amplos-raspi-buildenv

pc:
        docker build . pc.Dockerfile -t amplos-pc-buildenv
        docker run --rm -it -v $$(dirname $$PWD):/amplos amplos-pc-buildenv
