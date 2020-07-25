FROM ubuntu:18.04 as retesteth-build
RUN apt-get update \
    && apt-get install --yes git cmake g++ make perl psmisc  \
    && rm -rf /var/lib/apt/lists/*

# Retesteth
# ADD . /retesteth
RUN git clone --depth 1 -b develop https://github.com/ethereum/retesteth.git /retesteth
RUN mkdir /build && cd /build \
    && cmake /retesteth -DCMAKE_BUILD_TYPE=Release \
    && make -j8 \
    && cp /build/retesteth/retesteth /usr/bin/retesteth \
    && rm -rf /build /retesteth /var/cache/* /root/.hunter/*

#RUN git clone --depth 1 -b master https://github.com/ethereum/tests /tests

# Solidity
RUN git clone --depth 1 -b master https://github.com/winsvega/solidity.git /solidity
RUN mkdir /build && cd /build \
    && apt-get update \
    && apt-get install --yes libboost-all-dev \
    && cmake /solidity -DCMAKE_BUILD_TYPE=Release -DLLL=1 && make lllc \
    && cp /build/lllc/lllc /bin/lllc \
    && rm -rf /build /solidity /var/cache/* /root/.hunter/*


# Geth
RUN git clone --depth 1 -b master https://github.com/ethereum/go-ethereum.git /geth
RUN cd /geth && apt-get install wget \
    && wget https://dl.google.com/go/go1.13.3.linux-amd64.tar.gz \
    && tar -xvf go1.13.3.linux-amd64.tar.gz \
    && mv go /usr/local && ln -s /usr/local/go/bin/go /bin/go \
    && make all && cp /geth/build/bin/evm /bin/evm \
    && cp /geth/build/bin/geth /bin/geth \
    && rm -rf /geth && rm -rf /usr/local/go

ENTRYPOINT ["/usr/bin/retesteth"]
