// -*- mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
// vim: ft=cpp:expandtab:ts=4:sw=4:softtabstop=4:
// SPDX-FileCopyrightText: Huawei Inc.
// SPDX-License-Identifier: MIT

#include <stdlib.h>
#include <lvgl/lvgl.h>
#include "yesno.h"


static void yes_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        exit(0);
    }
}


static void no_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        exit(2);
    }
}


void yesno_ui_init(int argc, char ** argv)
{
    lv_obj_t * label1 = lv_label_create(lv_scr_act());
    char * text = (argc > 2) ? argv[2] : "";
    lv_label_set_text(label1, text);

    lv_obj_t * yesbtn = lv_btn_create(lv_scr_act());
    lv_obj_align(yesbtn, LV_ALIGN_LEFT_MID, 10, 0);
    lv_obj_add_event_cb(yesbtn, yes_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_t * yeslabel = lv_label_create(yesbtn);
    lv_label_set_text(yeslabel, "Yes");

    lv_obj_t * nobtn = lv_btn_create(lv_scr_act());
    lv_obj_align(nobtn, LV_ALIGN_LEFT_MID, 10, 0);
    lv_obj_add_event_cb(nobtn, no_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_t * nolabel = lv_label_create(nobtn);
    lv_label_set_text(nolabel, "No");
}
