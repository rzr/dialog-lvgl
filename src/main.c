// -*- mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
// vim: ft=cpp:expandtab:ts=4:sw=4:softtabstop=4:
// SPDX-FileCopyrightText: Huawei Inc.
// SPDX-License-Identifier: MIT

#define _DEFAULT_SOURCE /* needed for usleep() */
#include <unistd.h>

#include <lvgl/lvgl.h>
#include <lv_drivers/display/monitor.h>

static void hal_init(void)
{
    monitor_init();
}


int main(int argc, char ** argv)
{
    lv_init();
    hal_init();

    while(1) {
        lv_timer_handler();
        usleep(5 * 1000);
    }
    return 0;
}
