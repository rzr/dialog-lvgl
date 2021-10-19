// -*- mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
// vim: ft=cpp:expandtab:ts=4:sw=4:softtabstop=4:
// SPDX-FileCopyrightText: Huawei Inc.
// SPDX-License-Identifier: MIT

#define _DEFAULT_SOURCE /* needed for usleep() */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <lvgl/lvgl.h>

static void hal_init(void); /* see {sdl,...}.c */

#ifdef DIALOG_LVGL_WIDGETS_ENABLED
#include "widgets/msgbox.h"
#include "widgets/yesno.h"
#include "widgets/inputbox.h"
#include "widgets/checklist.h"
#endif


char * usage(int argc, char ** argv)
{
    printf("Usage: %s, <COMMAND> [OPTIONS]\n\
\n\
Description:\n\
 Basic UI utility to be used in scripts.\n\
 Inspired by ncurses' dialog, implemented using LVGL\n\
\n\
Commands: \n\
 --msgbox \"message\" 0 0\n\
 --yesno  \"message\" 0 0\n\
 --inputbox \"message\" 0 0\n\
 --checklist  \"message\" 0 0 0 \"keyword1\" \"Description1\" \"ON\" ... \n\
\n\
", argv[0]);
}


void default_ui_init(int argc, char ** argv)
{
    usage(argc, argv);
    char * msgbox_argv[] = {
        argv[0],
        "--msgbox",
        "Basic UI utility to be used in scripts.\n\
Inspired by ncurses dialog, implemented using LVGL\n\
For help run again with '--help' command option"
    };
#ifdef DIALOG_LVGL_WIDGETS_ENABLED
    msgbox_ui_init(3, msgbox_argv);
#endif
}


void ui_init(int argc, char ** argv)
{
#ifdef DIALOG_LVGL_WIDGETS_ENABLED
    if((argc >= 1) && (argv[1])) {
        if(strcmp(argv[1], "--msgbox") == 0) {
            msgbox_ui_init(argc, argv);
        }
        else if(strcmp(argv[1], "--yesno") == 0) {
            yesno_ui_init(argc, argv);
        }
        else if(strcmp(argv[1], "--inputbox") == 0) {
            inputbox_ui_init(argc, argv);
        }
        else if(strcmp(argv[1], "--checklist") == 0) {
            checklist_ui_init(argc, argv);
        }
        else {
            default_ui_init(argc, argv);
        }
    }
    else {
        default_ui_init(argc, argv);
    }
#else
            default_ui_init(argc, argv);
#endif
}



int main(int argc, char ** argv)
{
    if((argc > 1) && strcmp(argv[1], "--help") == 0) {
        usage(argc, argv);
        return 0;
    }

    lv_init();
    hal_init();
    ui_init(argc, argv);

    while(1) {
        usleep(5 * 1000);
    }
    return 0;
}
