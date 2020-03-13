FROM ubuntu:18.04 as retesteth-build
RUN apt-get update \
    && apt-get install --yes git cmake g++ make perl  \
    && rm -rf /var/lib/apt/lists/*

# Retesteth
ADD . /retesteth
RUN mkdir /build && cd /build \
    && cmake /retesteth -DCMAKE_BUILD_TYPE=Release \
    && make -j8 \
    && cp /build/retesteth/retesteth /usr/bin/retesteth \
    && rm -rf /build /retesteth /var/cache/* /root/.hunter/*

RUN git clone --depth 1 -b master https://github.com/ethereum/tests /tests

ENTRYPOINT ["/usr/bin/retesteth"]
