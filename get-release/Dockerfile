FROM alpine

RUN apk add jq curl unzip tar

COPY entrypoint.sh /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]
