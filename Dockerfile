# for travis ci debug
# https://hub.docker.com/r/travisci/ci-ubuntu-2004
FROM travisci/ci-ubuntu-2004:packer-minimal-1629271179-73a5ea99
WORKDIR /code

RUN apt-get update -y && \
    apt-get install -y cmake cmake-data ninja-build cpputest \
    libc6-dev-i386 linux-headers-generic bison flex libfl-dev libbison-dev

COPY . .
RUN bash rebuild.sh
