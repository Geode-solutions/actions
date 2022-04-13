FROM ubuntu:20.04

RUN \
  apt update && \
  apt install -y software-properties-common && \
  add-apt-repository ppa:git-core/ppa && \
  apt update && \
  apt install -y wget git clang-format-12

COPY entrypoint.sh /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]
