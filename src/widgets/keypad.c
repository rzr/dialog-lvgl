// -*- mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
// vim: ft=cpp:expandtab:ts=4:sw=4:softtabstop=4:
// SPDX-FileCopyrightText: Huawei Inc.
// SPDX-License-Identifier: MIT

#include <lvgl.h>

static const char *keymap[] = {
	"7", "8", "9", "\n",
	"4", "5", "6", "\n",
	"1", "2", "3", "\n",
	"OK", "#", ""
};

static lv_obj_t *text;

static void event_handler(lv_obj_t *obj, lv_event_t event)
{
	const char *txt;

	if (event != LV_EVENT_VALUE_CHANGED)
		return;

	txt = lv_btnmatrix_get_active_btn_text(obj);

	if (strcmp(txt, "OK") == 0)
		lv_textarea_set_text(text, "");
	else
		lv_textarea_add_char(text, txt[0]);
}

void show_widget(int argc, char **argv)
{
	lv_obj_t *keypad;

	text = lv_textarea_create(lv_scr_act(), NULL);
	lv_obj_set_size(text, 200, 100);
	lv_obj_align(text, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
	lv_textarea_set_text(text, "");

	keypad = lv_btnmatrix_create(lv_scr_act(), NULL);
	lv_btnmatrix_set_map(keypad, keymap);
	lv_obj_align(keypad, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
	lv_obj_set_event_cb(keypad, event_handler);

}
