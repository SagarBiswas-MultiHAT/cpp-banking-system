FROM ubuntu:22.04

RUN apt-get update \
    ; apt-get install -y --no-install-recommends \
    cmake ninja-build g++ libssl-dev ca-certificates \
    ; rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . /app

RUN cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release \
    ; cmake --build build

CMD ["./build/banking_app"]
