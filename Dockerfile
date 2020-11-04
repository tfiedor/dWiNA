FROM debian:jessie

# add dependencies
RUN apt-get update && apt-get install -y \
  autotools-dev \
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
  && rm -rf /var/lib/apt/lists/*

# clone MONA and libvata
RUN git clone https://github.com/cs-au-dk/MONA.git \
  && git clone git://github.com/ondrik/libvata.git

# build MONA
WORKDIR /MONA
RUN ./configure
RUN make lib

# build libvata
WORKDIR /libvata
RUN make release

# add dWiNA code
COPY . /build-environment/

# copy all required libraries to the expected places
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
