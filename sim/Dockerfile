FROM ubuntu:22.04 AS builder

ARG TARGETPLATFORM
RUN echo "TARGETPLATFORM : $TARGETPLATFORM"

RUN apt-get update && \
  DEBIAN_FRONTEND=noninteractive apt-get install -y cmake sudo git wget curl python3 python3-dev python3-pip pkg-config \
                                                    cmake-format libxml2-dev libeigen3-dev nano libsqlite3-dev vim htop && \
    pip install cppyy

ENV VERS 3.38
RUN wget https://www.nsnam.org/release/ns-allinone-$VERS.tar.bz2
RUN tar xjf ns-allinone-$VERS.tar.bz2 && rm ns-allinone-$VERS.tar.bz2
RUN mv /ns-allinone-$VERS/ns-$VERS /ns3

WORKDIR /ns3

RUN ./ns3 configure --enable-examples \
                    --enable-python-bindings \
                    -- -DNS3_BINDINGS_INSTALL_DIR="$HOME/.local/lib/python3.10/site-packages"
RUN ./ns3 build -j$(nproc)

RUN apt-get update && apt-get install software-properties-common -y && \
    add-apt-repository ppa:longsleep/golang-backports -y && \
    apt update && \
    apt install -y golang-go

## make including of the QuicNetworkSimulatorHelper class possible
COPY CMakeLists.txt.patch .
RUN patch -p1 < CMakeLists.txt.patch

RUN rm -r scratch/subdir scratch/scratch-simulator.cc
COPY scenarios scratch/

## compile all the scenarios
RUN ./ns3 build && \
    rm -rf /ns3/scratch/nested-subdir && \
    cd build/lib && du -sh . && strip -v * && du -sh . && cd ../.. && \
    cd build/scratch && rm -r subdir *-scratch-simulator*

# add mpquic module
WORKDIR /ns3/src
RUN git clone https://github.com/mpquic-measurement/ns3-mpquic-module.git quic && \
    cp -r quic/quic-applications/helper/. applications/helper/. && \
    cp -r quic/quic-applications/model/. applications/model/. && \
    mv quic/examples ../examples/quic && \
    cd /ns3 && \
    git apply < src/quic/ns3.patch

WORKDIR /ns3

RUN ./ns3 configure --enable-examples \
                    --enable-python-bindings \
                    -- -DNS3_BINDINGS_INSTALL_DIR="$HOME/.local/lib/python3.10/site-packages"
RUN ./ns3 build -j$(nproc)


ENV PATH="/go/bin:${PATH}"
COPY wait-for-it-quic /wait-for-it-quic
RUN cd /wait-for-it-quic && go build .

RUN apt-get update && \
  apt-get install -y net-tools iptables libsqlite3-dev libxml2-dev libeigen3-dev vim htop && \
  apt-get clean

WORKDIR /ns3/build/
RUN  cp -r /wait-for-it-quic/wait-for-it-quic /usr/bin

# see https://gitlab.com/nsnam/ns-3-dev/issues/97
ENV PATH="/ns3/build/src/fd-net-device/:${PATH}"
ENV LD_LIBRARY_PATH="/ns3/build/lib"

COPY run.sh .
RUN chmod +x run.sh
RUN mkdir /logs

ENTRYPOINT [ "./run.sh" ]
