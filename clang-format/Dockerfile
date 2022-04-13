FROM ubuntu:20.04

RUN \
  apt-get update && \
  apt-get install -y software-properties-common && \
  add-apt-repository ppa:git-core/ppa && \
  apt-get update && \
  apt-get install -y wget git clang-format-12

COPY entrypoint.sh /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]
