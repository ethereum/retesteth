FROM ubuntu

ENV TZ=Etc/UTC
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get update
RUN apt-get install \
        git \
        cmake \
        g++ \
        make \
        perl -y

RUN git clone --recursive https://github.com/ethereum/retesteth --branch master --single-branch --depth 1
RUN mkdir /build
RUN cd /build && cmake /retesteth -DCMAKE_BUILD_TYPE=Release
RUN cd /build && make -j8
RUN cp /build/retesteth/retesteth /usr/bin/retesteth
RUN cd / && rm /build -rf \
    && rm /retesteth -rf \
    && rm /var/cache/* -rf \
    && rm /root/.hunter/* -rf

ENTRYPOINT ["/usr/bin/retesteth"]
