FROM debian:jessie
RUN apt-get update && apt-get install -y \
  automake \
  git \
  cmake \
  gcc \
  g++ \
  libboost-filesystem-dev \
  libboost-system-dev \
  libboost-test-dev \
  flex \
  bison \
  && rm -rf /var/lib/apt/lists/* \
  && git clone git://github.com/ondrik/libvata.git \
  && make -C libvata release \
  && git clone https://github.com/cs-au-dk/MONA.git \
  && cd MONA \
  && ./configure \
  && make lib

COPY . /build-environment/

RUN cp -r /libvata/include /build-environment/include \
  && cp /libvata/build/src/libvata.a /build-environment/src/libs/ \
  && cp /MONA/BDD/.libs/libmonabdd.so /build-environment/src/libs/ \
  && cp /MONA/BDD/.libs/libmonabdd.so.1 /build-environment/src/libs/ \
  && cp /MONA/BDD/.libs/libmonabdd.so.1.0.4 /build-environment/src/libs/ \
  && cp /MONA/BDD/.libs/libmonabdd.a /build-environment/src/libs/ \
  && cp /MONA/DFA/.libs/libmonadfa.so /build-environment/src/libs/ \
  && cp /MONA/DFA/.libs/libmonadfa.so.1 /build-environment/src/libs/ \
  && cp /MONA/DFA/.libs/libmonadfa.so.1.0.4 /build-environment/src/libs/ \
  && cp /MONA/DFA/.libs/libmonadfa.a /build-environment/src/libs/ \
  && cp /MONA/GTA/.libs/libmonagta.so /build-environment/src/libs/ \
  && cp /MONA/GTA/.libs/libmonagta.so.1 /build-environment/src/libs/ \
  && cp /MONA/GTA/.libs/libmonagta.so.1.0.4 /build-environment/src/libs/ \
  && cp /MONA/GTA/.libs/libmonagta.a /build-environment/src/libs/ \
  && cp /MONA/Mem/.libs/libmonamem.so /build-environment/src/libs/ \
  && cp /MONA/Mem/.libs/libmonamem.so.1 /build-environment/src/libs/ \
  && cp /MONA/Mem/.libs/libmonamem.so.1.0.4 /build-environment/src/libs/ \
  && cp /MONA/Mem/.libs/libmonamem.a /build-environment/src/libs/ \
  && cd /build-environment/ \
  && mkdir -p build \
  && make release

WORKDIR /build-environment/build/
