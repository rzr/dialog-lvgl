#!/bin/echo docker build . -f
# -*- coding: utf-8 -*-
# SPDX-FileCopyrightText: Huawei Inc.
# SPDX-License-Identifier: MIT


FROM debian:11
LABEL maintainer="Philippe Coval <philippe.coval@huawei.com>"

ENV DEBIAN_FRONTEND noninteractive
ENV LC_ALL en_US.UTF-8
ENV LANG ${LC_ALL}

RUN echo "# log: Setup system" \
&& set -x \
&& apt-get update -y \
&& apt-get install -y make \
&& sync

RUN echo "# log: Install ${project}"\
&& set -x \
&& sync


ENV project dialog-lvgl
COPY Makefile /usr/local/opt/${project}/src/${project}/
WORKDIR /usr/local/opt/${project}/src/${project}/
RUN echo "# log: Build ${project}" \
&& set -x \
&& make setup/debian \
&& sync

COPY . /usr/local/opt/${project}/src/${project}/
WORKDIR /usr/local/opt/${project}/src/${project}/
RUN echo "# log: Build ${project}" \
&& set -x \
&& make help lib exe \
&& sync

WORKDIR /usr/local/opt/${project}/src/${project}/
ENTRYPOINT [ "/usr/bin/make" ]
CMD [ "run" ]
