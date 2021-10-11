// -*- mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
// vim: ft=cpp:expandtab:ts=4:sw=4:softtabstop=4:
// SPDX-FileCopyrightText: Huawei Inc.
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <lvgl/lvgl.h>
#include "inputbox.h"

const char * inputbox_value = "";

static void ok_inputbox_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        fprintf(stderr, "%s\n",  inputbox_value);
        exit(0);
    }
}

static void cancel_inputbox_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        exit(2);
    }
}


static void inputbox_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        inputbox_value = lv_textarea_get_text(obj);
    }
}


void inputbox_ui_init(int argc, char ** argv)
{
    lv_obj_t * label1 = lv_label_create(lv_scr_act());
    char * text = (argc > 2) ? argv[2] : "";
    lv_label_set_text(label1, text);

    lv_obj_t * ta = lv_textarea_create(lv_scr_act());
    lv_obj_set_size(ta, 200, 100);
    lv_obj_add_event_cb(ta, inputbox_event_handler,  LV_EVENT_ALL, NULL);

    lv_obj_t * kb = lv_keyboard_create(lv_scr_act());
    lv_keyboard_set_textarea(kb, ta);
    {
        lv_obj_t * btn = lv_btn_create(lv_scr_act());
        lv_obj_add_event_cb(btn, ok_inputbox_event_handler, LV_EVENT_ALL, NULL);
        lv_obj_t * label = lv_label_create(btn);
        lv_label_set_text(label, "OK");
        lv_obj_align(btn, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    }
    {
        lv_obj_t * btn = lv_btn_create(lv_scr_act());
        lv_obj_add_event_cb(btn, cancel_inputbox_event_handler, LV_EVENT_ALL, NULL);
        lv_obj_t * label = lv_label_create(btn);
        lv_label_set_text(label, "Cancel");
        lv_obj_align(btn, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    }


}
