FROM ubuntu:18.04

RUN \
  apt-get update && \
  apt-get install -y wget clang-format-8

COPY entrypoint.sh /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]
