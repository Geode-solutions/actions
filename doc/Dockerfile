FROM silkeh/clang:dev

RUN apt-get update && apt-get upgrade -y
RUN apt-get install -y git jq

COPY entrypoint.sh /entrypoint.sh
COPY style.py /style.py

ENTRYPOINT ["/entrypoint.sh"]