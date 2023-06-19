FROM cr.jinwei.me/mpquic/quic-network-simulator:22.04

WORKDIR /ns3/scratch/mpquic

COPY CMakeLists.txt .
COPY mpquic.cc .

WORKDIR /ns3

RUN ./ns3 configure --enable-examples \
                    --enable-python-bindings \
                    -- -DNS3_BINDINGS_INSTALL_DIR="$HOME/.local/lib/python3.10/site-packages"
RUN ./ns3 build -j$(nproc)
