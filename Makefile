#! /usr/bin/make -f
# -*- makefile -*-
# ex: set tabstop=4 noexpandtab:
# SPDX-FileCopyrightText: Huawei Inc.
# SPDX-License-Identifier: MIT

default: all
	-@sync

lvgl?=lvgl
project?=dialog-${lvgl}
url?=https://git.ostc-eu.org/rzr/dialog-lvgl

cmake_options?=-DCMAKE_VERBOSE_MAKEFILE:BOOL=ON
cmake_options+=-Dinstall:BOOL=ON

exe?=${project}
sysroot?=${CURDIR}/tmp/sysroot
prefix?=/usr/local
includedir?=${prefix}/include
base_bindir?=/bin
base_libdir?=/lib
bindir?=${prefix}/${base_bindir}
libdir?=${prefix}/${base_libdir}
srcs?=$(wildcard src/*.c | sort)
objs?=${srcs:.c=.o}
CFLAGS+=-I${sysroot}${includedir}/
CFLAGS+=-I${sysroot}${includedir}/lvgl
CFLAGS+=-I${sysroot}${includedir}/lvgl/lv_drivers
LDLIBS+=-L${sysroot}/${libdir}
LDLIBS+=-llv_drivers
LDLIBS+=-llvgl
V?=1
width?=42
height=${width}
depth?=1

# TODO: Pin upstream URL once released
lvgl_branch?=master
lvgl_org?=lvgl
lvgl_url?=https://github.com/${lvgl_org}/lvgl
lv_drivers_url?=https://github.com/${lvgl_org}/lv_drivers
lv_drivers_branch?=${lvgl_branch}

depsdir?=tmp/deps
sudo?=
export sudo

lvgl_driver?=sdl


CFLAGS+=-DLV_CONF_INCLUDE_SIMPLE=1

ifeq (wayland, ${lvgl_driver})
CFLAGS+=-DUSE_WAYLAND=1
CFLAGS+=$(shell pkg-config --cflags wayland-client 2> /dev/null || echo -- "")
CFLAGS+=$(shell pkg-config --cflags xkbcommon 2> /dev/null || echo -- "")
LDLIBS+=-pthread
LDLIBS+=$(shell pkg-config --libs wayland-client 2> /dev/null || echo -- "-lwayland-client")
LDLIBS+=$(shell pkg-config --libs xkbcommon 2> /dev/null || echo -- "-lxkbcommon")
endif

ifeq (sdl, ${lvgl_driver})
CFLAGS+=-DUSE_SDL=1
CFLAGS+=$(shell pkg-config --cflags sdl2 2> /dev/null || echo -- "")
LDLIBS+=$(shell pkg-config --libs sdl2 2> /dev/null || echo -- "-lsdl2")
endif

help:
	@echo "# URL: ${url}"
	@echo "# Usage:"
	@echo "#  make deps # will download build in depsdir and install to sysroot"
	@echo "#  make run # will run demo"
	@echo "# Env:"
	@echo "#  depsdir=${depsdir}"
	@echo "#  sysroot=${sysroot}"
	@echo "#  libdir=${libdir}"
	@echo "#  includedir=${includedir}"

all: help ${exe}
	ls ${exe}

${exe}: ${objs}
	${CC} ${LDFLAGS} -o $@ $^ ${LDLIBS}

exe: ${exe}

clean:
	rm -vf ${exe} *.o */*.o

cleanall: clean
	rm -rf tmp

run: ${exe}
	${<D}/${<F}

install: ${exe}
	${sudo} install -d "${DESTDIR}/${bindir}"
	${sudo} install $< "${DESTDIR}/${bindir}/${project}"


deps/lvgl: ${depsdir}/${lvgl}/lv_conf.h
	cd ${<D} \
	  && export CFLAGS="${CFLAGS}" \
	  && cmake ${cmake_options} . \
	  && make \
	  && make install DESTDIR=${sysroot}

${depsdir}/${lvgl}/lv_conf.h: ${depsdir}/${lvgl}/lv_conf_template.h
	sed \
	    -e 's|#if 0 .*Set it to "1" to enable the content.*|#if 1 // Enabled manualy|g' \
	< $< > $@

${depsdir}/${lvgl}/lv_conf_template.h:
	@mkdir -p ${@D}
	git clone --depth 1 --recursive ${lvgl_url} --branch ${lvgl_branch} ${@D}

deps/lv_drivers: ${depsdir}/lv_drivers/lv_drv_conf.h
	cd ${<D} \
	  && export CFLAGS="${CFLAGS} -DLV_CONF_INCLUDE_SIMPLE=1\
	    -I${sysroot}/${includedir}/lvgl \
	    -I${sysroot}/${includedir}/ " \
	  && cmake ${cmake_options} . \
	  && make \
	  && make install DESTDIR=${sysroot}

${depsdir}/lv_drivers/lv_drv_conf.h: ${depsdir}/lv_drivers/lv_drv_conf_template.h
	sed \
	    -e 's|#if 0 .*Set it to "1" to enable the content.*|#if 1 // Enabled manualy|g' \
	< $< > $@

${depsdir}/lv_drivers/lv_drv_conf_template.h:
	@mkdir -p ${@D}
	git clone --depth 1 --recursive ${lv_drivers_url} --branch ${lv_drivers_branch} ${@D}


deps: deps/lvgl deps/lv_drivers
	@-sync

lib: deps
	@-sync

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

setup/debian: /etc/debian_version
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
