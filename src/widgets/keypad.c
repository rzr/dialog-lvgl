/*
 * Copyright (C) 2021 Bartosz Golaszewski <bartosz.golaszewski@huawei.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <lvgl.h>

static const char *keymap[] = {
	"7", "8", "9", "\n",
	"4", "5", "6", "\n",
	"1", "2", "3", "\n",
	"OK", "#", ""
};

void show_widget(void)
{
	lv_obj_t *keypad;

	keypad = lv_btnmatrix_create(lv_scr_act(), NULL);
	lv_btnmatrix_set_map(keypad, keymap);
	lv_obj_align(keypad, NULL, LV_ALIGN_CENTER, 0, 0);
}
