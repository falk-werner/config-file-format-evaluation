FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive
RUN apt update && apt install -y --no-install-recommends \
    ca-certificates \
    git \
    build-essential \
    pkg-config \
    cmake \
    libgtest-dev \
    libgmock-dev \
    clang-tidy \
    valgrind \
    lcov \
    cloc \
    nlohmann-json3-dev \
    libxml2-dev \
    libyaml-cpp-dev \
    libtoml11-dev

RUN mkdir -p /src
WORKDIR /src
RUN git clone --single-branch --branch 2.3.0 https://github.com/pboettch/json-schema-validator.git

WORKDIR /src/json-schema-validator
RUN cmake -B build
RUN cmake --build build
RUN cmake --build build --target install

WORKDIR /
RUN rm -rf /src

FROM builder as devcontainer

ARG USERNAME=user
ARG USER_UID=1000
ARG USER_GID=$USER_UID

RUN groupadd --gid $USER_GID $USERNAME
RUN useradd --uid $USER_UID --gid $USER_GID -m $USERNAME