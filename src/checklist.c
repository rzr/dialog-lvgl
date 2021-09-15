// -*- mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
// vim: ft=cpp:expandtab:ts=4:sw=4:softtabstop=4:
// SPDX-FileCopyrightText: Huawei Inc.
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <lvgl/lvgl.h>
#include "checklist.h"


static lv_obj_t * * checklist_model = 0;
static int checklist_model_size = 0;


static void ok_checklist_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        for(int i = 0; i < checklist_model_size; i++) {
            lv_obj_t * cb = checklist_model[i];
            if(lv_obj_get_state(cb) == LV_STATE_CHECKED) {
                fprintf(stderr, "%s\n", (char *) lv_obj_get_user_data(cb));
            }
        }
        checklist_ui_free();
        exit(0);
    }
}

static void cancel_checklist_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        exit(2);
    }
}


void checklist_ui_init(int argc, char ** argv)
{
    lv_obj_t * label1 = lv_label_create(lv_scr_act());
    char * text = (argc > 2) ? argv[2] : "";
    lv_label_set_text(label1, text);

    int i = 3 + 3; // skip w,h,d
    int index = 0;
    checklist_model_size = (argc - 2 - 4) / 3;
    checklist_model = malloc(checklist_model_size * sizeof(lv_obj_t));

    lv_obj_set_flex_flow(lv_scr_act(), LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(lv_scr_act(), LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);

    while(i < argc - 2) {
        char * title = argv[i++];
        char * desc = argv[i++];
        char * onoff = argv[i++];
        int len = strlen(title) + sizeof(':') + strlen(desc) + sizeof('\0');
        char * text = malloc(len);
        snprintf(text, len, "%s:%s", title, desc);
        int state = (strcmp("ON", onoff) == 0) ? LV_STATE_CHECKED : LV_STATE_DEFAULT;

        lv_obj_t * cb = lv_checkbox_create(lv_scr_act());
        checklist_model[index] = cb;
        lv_checkbox_set_text(cb, text);
        lv_obj_set_user_data(cb, title);
        lv_obj_add_state(cb, state);

        index++;
    }

    {
        lv_obj_t * btn = lv_btn_create(lv_scr_act());
        lv_obj_add_event_cb(btn, ok_checklist_event_handler, LV_EVENT_ALL, NULL);
        lv_obj_t * label = lv_label_create(btn);
        lv_label_set_text(label, "OK");
    }

    {
        lv_obj_t * btn = lv_btn_create(lv_scr_act());
        lv_obj_add_event_cb(btn, cancel_checklist_event_handler, LV_EVENT_ALL, NULL);
        lv_obj_t * label = lv_label_create(btn);
        lv_label_set_text(label, "Cancel");
    }
}


void checklist_ui_free()
{
    free(checklist_model);
}
