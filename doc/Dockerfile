FROM ubuntu:18.04

RUN \
  apt-get update && \
  apt-get install -y wget git doxygen python3-pip

COPY entrypoint.sh /entrypoint.sh
COPY conf.py /doc/conf.py
COPY alabaster/* /doc/alabaster/

ENTRYPOINT ["/entrypoint.sh"]
