FROM debian:trixie-slim

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends build-essential procps && apt-get clean && rm -rf /var/lib/apt/lists/*
RUN mkdir -p /demon

WORKDIR /demon

CMD ["bash"]
