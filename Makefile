#! /usr/bin/make -f
# -*- makefile -*-
# ex: set tabstop=4 noexpandtab:
# SPDX-FileCopyrightText: Huawei Inc.
# SPDX-License-Identifier: MIT

default: all
	-@sync

lvgl?=lvgl-sdl
project?=dialog-${lvgl}
url?=https://git.ostc-eu.org/rzr/dialog-lvgl
exe?=${project}
sysroot?=${CURDIR}/tmp/sysroot
prefix?=/usr/local
includedir?=${prefix}/include
base_bindir?=/bin
base_libdir?=/lib
bindir?=${prefix}/${base_bindir}
libdir?=${prefix}/${base_libdir}
libstatic?=${sysroot}/${libdir}/lib${lvgl}.a
srcs?=$(wildcard src/*.c | sort)
objs?=${srcs:.c=.o}
CFLAGS+=-I${sysroot}${includedir}/${lvgl}/
LDLIBS+=$(shell pkg-config --libs sdl2 2> /dev/null || echo -- "-lsdl2")

V?=1
width?=42
height=${width}
depth?=1

# TODO: Pin upstream URL once released
lvgl_branch?=sandbox/rzr/master
lvgl_branch?=master
lvgl_org?=astrolabe-coop
lvgl_org?=lvgl
lvgl_url?=https://github.com/${lvgl_org}/lv_sim_vscode_sdl

depsdir?=tmp/deps
sudo?=
export sudo


help:
	@echo "# URL: ${url}"
	@echo "# Usage:"
	@echo "#  make run # will run demo"
	@echo "#  make lib # will download build and install lib"
	@echo "# Env:"
	@echo "#  sysroot=${sysroot}"
	@echo "#  libexecdir=${libexecdir}"
	@echo "#  libstatic=${libstatic}"
	@echo "#  libdir=${libdir}"
	@echo "#  includedir=${includedir}"

all: help ${exe} ${libstatic}
	ls ${exe}

${libstatic}:
	@echo "error: $@ missing from ${sysroot}"
	@echo "error: Run make lib to build and install it"
	@exit 1

${exe}: ${objs} ${libstatic}
	${CC} ${LDFLAGS} -o $@ $^ ${LDLIBS}

exe: ${exe}

clean:
	rm -vf ${exe} *.o */*.o

cleanall: clean
	rm -rf tmp

run: ${exe}
	${<D}/${<F}

install: ${exe}
	install -d "${DESTDIR}/${bindir}"
	install $< "${DESTDIR}/${bindir}/${project}"


lib/%: ${depsdir}/${lvgl}
	${MAKE} -C $< ${@F} DESTDIR="${sysroot}"

lib: lib/install
	ls ${lib}

${depsdir}/${lvgl}:
	mkdir -p ${@D}
	git clone --depth 1 --recursive ${lvgl_url} --branch ${lvgl_branch} $@

format: extra/scripts/code-format.cfg
	astyle --options=$< "./src/*.c,.h"

run/%:
	PATH=${PATH}:. ${@F}
	@echo ""

msgbox/%:
	PATH=${PATH}:. ${@F} --msgbox "$@" ${width} ${height}
	@echo ""

yesno/%:
	@echo "info: yesno: check exit code {0,2}"
	PATH=${PATH}:. ${@F} --yesno "Confirm" ${width} ${height}
	@echo ""

inputbox/%:
	@echo "info: inputbox: check stdout value"
	PATH=${PATH}:. ${@F} --inputbox "Input" ${width} ${height}
	@echo ""

checklist/%:
	@echo "info: checklist: check stdout values"
	PATH=${PATH}:. ${@F} --checklist "$@: Choose an option" ${width} ${height} ${depth} \
"01" "First" OFF \
"02" "Second" ON \
"03" "Third"  OFF
	@echo ""


demo/%: run/% msgbox/% yesno/% inputbox/% checklist/%
	-@sync

dialog: demo/dialog
	-@sync

demo: ${exe} demo/${exe} demo/dialog

setup/debian:
# /etc/debian_version
	-${sudo} apt-get update -y 
	${sudo} apt-get install -y \
		dialog \
		gcc \
		git \
		libsdl2-dev \
		make \
		pkg-config \
		sudo

/etc/debian_version:
	@echo "error: distro not supported please file a bug: ${url}"

setup: setup/debian

docker_image?=${project}
docker_tag?=${project}

docker/build: Dockerfile
	docker build -t "${docker_tag}" "."
	docker create --name "${docker_image}" "${docker_tag}"

docker/run:
	xhost + localhost
	docker run \
		-e DISPLAY="${DISPLAY}" \
		-v /tmp/.X11-unix:/tmp/.X11-unix \
		"${docker_image}"
