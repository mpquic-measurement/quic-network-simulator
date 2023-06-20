FROM cr.jinwei.me/mpquic/quic-network-simulator:22.04

RUN apt-get update && \
  apt-get install -y wget net-tools iputils-ping tcpdump ethtool iperf iproute2

COPY setup.sh .
RUN chmod +x setup.sh

COPY run_endpoint.sh .
RUN chmod +x run_endpoint.sh

RUN wget https://raw.githubusercontent.com/vishnubob/wait-for-it/master/wait-for-it.sh && chmod +x wait-for-it.sh

WORKDIR /ns3/scratch/mpquic

ADD . /ns3/scratch/mpquic

WORKDIR /ns3

RUN ./ns3 configure --enable-examples \
                    --enable-python-bindings \
                    -- -DNS3_BINDINGS_INSTALL_DIR="$HOME/.local/lib/python3.10/site-packages"
RUN ./ns3 build -j$(nproc)

ENTRYPOINT [ "/run_endpoint.sh" ]
