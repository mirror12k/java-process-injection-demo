FROM ubuntu:22.04

# Install "software-properties-common" (for the "add-apt-repository")
RUN apt-get update && apt-get install -y \
    software-properties-common

## Install Oracle's JDK
# add oracle jdk repository
RUN add-apt-repository ppa:ts.sch.gr/ppa \
# accept oracle license
  && echo debconf shared/accepted-oracle-license-v1-1 select true | debconf-set-selections \
  && echo debconf shared/accepted-oracle-license-v1-1 seen true | debconf-set-selections \
  && apt-get update \
# install oracle jdk 8 and make it default
  && apt-get -y install oracle-java8-installer \
  && apt-get -y install oracle-java8-set-default \
# install stuff
  && apt-get -y install make gcc-mingw-w64 \
# clean up
  && apt-get clean all \
  && rm -rf /var/lib/apt/lists/*


ENV JAVA_HOME /usr/lib/jvm/java-9-oracle

WORKDIR /work

COPY . /work

CMD ["/bin/bash"]

