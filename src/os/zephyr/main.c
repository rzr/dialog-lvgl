// -*- mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
// vim: ft=cpp:expandtab:ts=4:sw=4:softtabstop=4:
// SPDX-FileCopyrightText: Huawei Inc.
// SPDX-License-Identifier: MIT

#include <device.h>
#include <drivers/display.h>
#include <lvgl.h>
#include <zephyr.h>

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <logging/log.h>
LOG_MODULE_REGISTER(app);

void show_widget(int argc, char **argv);

void main(void)
{
	const struct device *display_dev;

	display_dev = device_get_binding(CONFIG_LVGL_DISPLAY_DEV_NAME);
	if (!display_dev) {
		LOG_ERR("device not found. Aborting.");
		return;
	}

	show_widget(0, NULL);
	display_blanking_off(display_dev);
	lv_task_handler();

	while (1) {
		lv_task_handler();
		k_sleep(K_MSEC(10));
	}
}
