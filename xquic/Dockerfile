FROM quic-network-simulator-endpoint:22.04

RUN apt-get update && apt-get install -y sudo vim nano tree libcurl4-openssl-dev libevent-dev htop \
                                         git build-essential cmake software-properties-common && \
    add-apt-repository ppa:longsleep/golang-backports -y && \
    apt update && \
    apt install -y golang-go

RUN useradd -ms /bin/bash ns3dce && adduser ns3dce sudo && echo -n 'ns3dce:ns3dce' | chpasswd

RUN sed -i.bkp -e \
      's/%sudo\s\+ALL=(ALL\(:ALL\)\?)\s\+ALL/%sudo ALL=NOPASSWD:ALL/g' \
      /etc/sudoers

WORKDIR /home/ns3dce

RUN git clone https://github.com/alibaba/xquic.git && \
    cd xquic && \
    git checkout 78cf6d1e50d4ebe8fb51fddfbdb55d36a5798386 && \
    git clone https://github.com/google/boringssl.git ./third_party/boringssl && \
    cd ./third_party/boringssl && \
    git checkout 6a7d8b54725d6e762f758726464da2dc2e112435 && \
    mkdir -p build && cd build && \
    cmake -DBUILD_SHARED_LIBS=0 -DCMAKE_C_FLAGS="-fPIC" -DCMAKE_CXX_FLAGS="-fPIC" .. && \
    make ssl crypto -j$(nproc)

ENV SSL_TYPE_STR="boringssl"
ENV SSL_PATH_STR="/home/ns3dce/xquic/third_party/boringssl"
ENV SSL_INC_PATH_STR="/home/ns3dce/xquic/third_party/boringssl/include"
ENV SSL_LIB_PATH_STR="/home/ns3dce/xquic/third_party/boringssl/build/ssl/libssl.a;/home/ns3dce/xquic/third_party/boringssl/build/crypto/libcrypto.a"

WORKDIR /home/ns3dce/xquic

RUN git submodule update --init --recursive && \
    mkdir -p build && cd build && \
    cmake -DGCOV=on -DCMAKE_BUILD_TYPE=Debug -DXQC_ENABLE_TESTING=1 -DXQC_SUPPORT_SENDMMSG_BUILD=1 -DXQC_ENABLE_EVENT_LOG=1 -DXQC_ENABLE_BBR2=1 -DXQC_DISABLE_RENO=0 -DSSL_TYPE=${SSL_TYPE_STR} -DSSL_PATH=${SSL_PATH_STR} -DSSL_INC_PATH=${SSL_INC_PATH_STR} -DSSL_LIB_PATH=${SSL_LIB_PATH_STR} .. && \
    make -j$(nproc)

# copy run script and run it
COPY run_endpoint.sh .
RUN chmod +x run_endpoint.sh
ENTRYPOINT [ "./run_endpoint.sh" ]
