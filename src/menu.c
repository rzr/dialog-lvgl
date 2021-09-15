// -*- mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
// vim: ft=cpp:expandtab:ts=4:sw=4:softtabstop=4:
// SPDX-FileCopyrightText: Huawei Inc.
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <lvgl/lvgl.h>
#include "menu.h"


static char * menu_selected_value = 0;


static void ok_menu_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        fprintf(stderr, "%s\n",  menu_selected_value);
        exit(0);
    }
}


static void cancel_menu_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        exit(2);
    }
}


static void menu_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        lv_obj_t * parent = lv_obj_get_parent(btn);
        for(uint32_t i = 0; i < lv_obj_get_child_cnt(parent); i++) {
            lv_obj_t * child = lv_obj_get_child(parent, i);
            if(child == btn) {
                menu_selected_value = lv_obj_get_user_data(btn);
                lv_obj_add_state(btn, LV_STATE_CHECKED);
                printf("TODO: check widget update %d\n", lv_obj_get_state(btn) && LV_STATE_CHECKED);

            }
            else {
                lv_obj_clear_state(child, LV_STATE_CHECKED);
            }
        }
    }
}


void menu_ui_init(int argc, char ** argv)
{
    lv_obj_t * label1 = lv_label_create(lv_scr_act());
    char * text = (argc > 2) ? argv[2] : "";
    lv_label_set_text(label1, text);

    lv_obj_t * list1 = lv_list_create(lv_scr_act());
    lv_obj_set_size(list1, 160, 200);
    uint32_t i = 3 + 3; // skip w,h,d
    while(i < argc) {
        char * title = argv[i++];
        char * desc = argv[i++];
        int len = strlen(title) + sizeof(':') + strlen(desc) + sizeof('\0');
        char * text = malloc(len);
        snprintf(text, len, "%s:%s", title, desc);
        lv_obj_t * btn = lv_list_add_btn(list1, NULL, text);
        lv_obj_set_user_data(btn, title);
        lv_obj_add_event_cb(btn, menu_event_handler, LV_EVENT_CLICKED, NULL);
    }
    lv_obj_t * currentButton = lv_obj_get_child(list1, 1);
    lv_obj_add_state(currentButton, LV_STATE_CHECKED);
    {
        lv_obj_t * btn = lv_btn_create(lv_scr_act());
        lv_obj_add_event_cb(btn, ok_menu_event_handler, LV_EVENT_ALL, NULL);
        lv_obj_t * label = lv_label_create(btn);
        lv_label_set_text(label, "OK");
    }
    {
        lv_obj_t * btn = lv_btn_create(lv_scr_act());
        lv_obj_add_event_cb(btn, cancel_menu_event_handler, LV_EVENT_ALL, NULL);
        lv_obj_t * label = lv_label_create(btn);
        lv_label_set_text(label, "Cancel");
    }
}
