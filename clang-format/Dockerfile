FROM ubuntu:20.04

RUN apt-get update
RUN apt-get install -y wget git clang-format-12

COPY entrypoint.sh /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]
