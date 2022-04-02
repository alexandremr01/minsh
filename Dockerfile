FROM ubuntu:18.04

WORKDIR /app

RUN apt-get upgrade && apt-get update && apt-get install -y build-essential

ENTRYPOINT ["bash"]