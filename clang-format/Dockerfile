FROM ubuntu:20.04

RUN \
  apt-get update && \
  apt-get install -y wget git clang-format-10

COPY entrypoint.sh /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]
