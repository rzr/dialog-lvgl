#! /usr/bin/make -f
# -*- makefile -*-
# ex: set tabstop=4 noexpandtab:
# SPDX-FileCopyrightText: Huawei Inc.
# SPDX-License-Identifier: MIT

default: all
	-@sync

lvgl?=lvgl-sdl
project?=dialog-${lvgl}
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
LDLIBS+=$(shell pkg-config --libs sdl2)

V?=1
width?=42
height=${width}

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


demo/%:
	PATH=${PATH}:. ${@F}
	@echo ""
	PATH=${PATH}:. ${@F} --msgbox "$@" ${width} ${height}
	@echo ""
	@echo "info: yesno: check exit code {0,2}"
	PATH=${PATH}:. ${@F} --yesno "Confirm" ${width} ${height}
	@echo ""
	@echo "info: inputbox: check stdout value"
	PATH=${PATH}:. ${@F} --inputbox "Input" ${width} ${height}
	@echo ""

demo: ${exe} demo/${exe}
