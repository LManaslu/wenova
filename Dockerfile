FROM ubuntu:latest

# Required for installing tzdata without user interaction
RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections

RUN apt-get update && apt-get install \
  g++ \
  libsdl2-2.0-0 \
  libsdl2-dev \
  libsdl2-image-2.0-0 \
  libsdl2-image-dev \
  libsdl2-mixer-2.0-0 \
  libsdl2-mixer-dev \
  libsdl2-ttf-2.0-0 \
  libsdl2-ttf-dev \
  make \
  vim -y
