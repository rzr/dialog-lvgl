#!/bin/sh
# SPDX-FileCopyrightText: Huawei Inc.
# SPDX-License-Identifier: MIT

url="https://git.ostc-eu.org/rzr/dialog-lvgl"
exe=dialog
width=42
height=42
depth=42


usage_()
{
    cat<<EOF
Usage:
$0 # to run default dialog : $exe
$0 dialog-lvgl-sdl # to use custom one

More:
$url
EOF
}


demo_()
{
    "${exe}" --help

    echo "info: msgbox"
    "${exe}" --msgbox "This is 'msgbox' dialog, please close" ${width} ${height}
    echo ""

    echo "info: yesno: check error code"
    "${exe}" --yesno "This is 'yesno' dialog, please confirm" ${width} ${height}
    echo "info: yesno: exit code = $?"
    echo ""

    echo "info: inputbox: check standard output on close"
    "${exe}"  --inputbox "This is 'input' dialog, please type" ${width} ${height}
    echo ""

    echo "info: checklist: check standard output on close"
    "${exe}" --checklist "This is checklist dialog, please select" ${width} ${height} ${depth} \
	     "01" "First item" OFF \
	     "02" "Second entry is on by default" ON \
	     "03" "Last on is off by default"  OFF
    echo ""
}


main_()
{
    usage_ "$@"
    [ "" = "$1" ] || exe="$1"
    while true ; do
	demo_
    done
}


main_ "$@"
