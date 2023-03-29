FROM ubuntu:20.04 as retesteth

ARG BESU_SRC="https://github.com/hyperledger/besu.git"
ARG PYSPECS_SRC="https://github.com/ethereum/execution-spec-tests"
ARG ETEREUMJS_SRC="https://github.com/ethereumjs/ethereumjs-monorepo.git"
ARG RETESTETH_SRC="https://github.com/ethereum/retesteth.git"
ARG GETH_SRC="https://github.com/ethereum/go-ethereum.git"
ARG NIMBUS_SRC="https://github.com/status-im/nimbus-eth1.git"

ARG BESU="main"
ARG GETH="master"
ARG NIMBUS="master"
ARG ETHEREUMJS="master"
ARG RETESTETH="develop"
ARG PYSPECS="main"

SHELL ["/bin/bash", "-c"]
ENV TZ=Etc/UTC
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Get necessary packages
RUN apt-get update \
    && apt install software-properties-common -y \
    && add-apt-repository -y ppa:ubuntu-toolchain-r/test \
    && add-apt-repository -y ppa:deadsnakes/ppa  \
    && add-apt-repository ppa:linuxuprising/java \
    && apt-get install --yes git cmake make perl psmisc curl wget gcc-11 python3.10 python3.10-venv python3-pip python3-dev \
    && apt-get install --yes libboost-filesystem-dev libboost-system-dev libboost-program-options-dev libboost-test-dev \
    && echo oracle-java17-installer shared/accepted-oracle-license-v1-3 select true | /usr/bin/debconf-set-selections  \
    && apt-get install --yes oracle-java17-installer oracle-java17-set-default \
    && rm -rf /var/lib/apt/lists/*
RUN rm /usr/bin/python3 && ln -s /usr/bin/python3.10 /usr/bin/python3


# Retesteth
RUN test -n "$RETESTETH" \
    && git clone $RETESTETH_SRC /retesteth \
    && cd /retesteth && git fetch && git checkout $RETESTETH && mkdir /build && cd /build \
    && cmake /retesteth -DCMAKE_BUILD_TYPE=Release \
    && make -j8 \
    && cp /build/retesteth/retesteth /usr/bin/retesteth \
    && rm -rf /build /retesteth /var/cache/* /root/.hunter/* \
    || echo "Retesteth is empty" > /usr/bin/retesteth

# Tests
#RUN git clone --depth 1 -b master https://github.com/ethereum/tests /tests

# Solidity LLLC
RUN test -n "$RETESTETH" \
    && git clone --depth 1 -b master https://github.com/winsvega/solidity.git /solidity \
    && mkdir /build && cd /build \
    && cmake /solidity -DCMAKE_BUILD_TYPE=Release -DLLL=1 && make lllc \
    && cp /build/lllc/lllc /bin/lllc \
    && rm -rf /build /solidity /var/cache/* /root/.hunter/* \
    || echo "Retesteth is empty, skip LLLC"

# Solidity solc
RUN wget https://github.com/ethereum/solidity/releases/download/v0.8.17/solc-static-linux \
   && cp solc-static-linux /bin/solc \
   && chmod +x /bin/solc

# Pyspecs
RUN git clone $PYSPECS_SRC /execution-spec-tests 
RUN cd /execution-spec-tests && git fetch && git checkout $PYSPECS \
    && python3 -m venv ./venv/ \
    && source ./venv/bin/activate \
    && pip install -e . \
    && wget https://raw.githubusercontent.com/ethereum/retesteth/develop/web/tfinit.sh \
    && cp tfinit.sh /usr/bin/tfinit.sh \
    && chmod +x /usr/bin/tfinit.sh


# Geth
RUN test -n "$GETH" \
     && git clone $GETH_SRC /geth \
     && cd /geth && git fetch && git checkout $GETH \
     && wget https://dl.google.com/go/go1.19.linux-amd64.tar.gz \
     && tar -xvf go1.19.linux-amd64.tar.gz \
     && mv go /usr/local && ln -s /usr/local/go/bin/go /bin/go \
     && make all && cp /geth/build/bin/evm /bin/evm \
     && cp /geth/build/bin/geth /bin/geth \
     && rm -rf /geth && rm -rf /usr/local/go \
    || echo "Geth is empty"

# Nimbus
RUN test -n "$NIMBUS" \
     && apt-get update \
     && apt-get install --yes librocksdb-dev \
     && rm -rf /var/lib/apt/lists/* \
     && git clone --recursive $NIMBUS_SRC /nimbus \
     && cd /nimbus && git fetch && git checkout $NIMBUS && make t8n \
     && cp /nimbus/tools/t8n/t8n /bin/evm_nimbus \
     && rm -rf /nimbus \
    || echo "Nimbus is empty"

# Ethereumjs
RUN test -n "$ETHEREUMJS" \
     && wget -qO- https://raw.githubusercontent.com/nvm-sh/nvm/v0.39.3/install.sh | bash \
     && . ~/.nvm/nvm.sh \
     && nvm install 19 && nvm alias default 19 && nvm use default \
     && cp -r ~/.nvm/versions/node/v19*/* /usr \
     && git clone $ETEREUMJS_SRC /ethereumjs \
     && cd /ethereumjs && git fetch && git checkout $ETHEREUMJS && npm i && npm run build --workspaces \
    || echo "Ethereumjs is empty"

# Besu
RUN test -n "$BESU" \
     && git clone $BESU_SRC /besu \
     && cd /besu && git fetch && git checkout $BESU \
     && ./gradlew build \
    || echo "Besu is empty"

RUN test -n "$BESU" \
     && cd /besu && ./gradlew ethereum:evmtool:installDist \
     && ln -s /besu/ethereum/evmtool/build/install/evmtool/bin/evm /usr/bin/besuevm \
    || echo "Besu is empty"

ENTRYPOINT ["/usr/bin/retesteth"]
