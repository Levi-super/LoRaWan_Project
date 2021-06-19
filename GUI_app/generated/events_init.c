/*
 * Copyright 2021 NXP
 * SPDX-License-Identifier: MIT
 */

#include "events_init.h"
#include <stdio.h>
#include "lvgl/lvgl.h"
#include "usart.h"


extern lv_ui guider_ui;
extern uint16_t up_interval;
extern uint16_t up_pick_cnt;
extern uint8_t sensor_type;

static void btnAddIntervel_event_cb(lv_obj_t * btn, lv_event_t event);
static void btnSubIntervel_event_cb(lv_obj_t * btn, lv_event_t event);

static void btnAddPickCnt_event_cb(lv_obj_t * btn, lv_event_t event);
static void btnSubPickCnt_event_cb(lv_obj_t * btn, lv_event_t event);

static void dropDownListSensor_event_cb(lv_obj_t * ddl, lv_event_t event);

void events_init(lv_ui *ui)
{

	 lv_obj_set_event_cb(ui->btnAddIntervel, btnAddIntervel_event_cb);
	 lv_obj_set_event_cb(ui->btnSubIntervel, btnSubIntervel_event_cb);
	
	 lv_obj_set_event_cb(ui->btnAddPickCnt, btnAddPickCnt_event_cb);
	 lv_obj_set_event_cb(ui->btnSubPickCnt, btnSubPickCnt_event_cb);
	
	 lv_obj_set_event_cb(ui->dropDownListSensor, dropDownListSensor_event_cb);
//	 
}

static void btnAddIntervel_event_cb(lv_obj_t * btn, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) {
        up_interval += 1000;
        /*Get the first child of the button which is the label and change its text*/
        lv_label_set_text_fmt(guider_ui.lbIntervel, "Time:%ds", up_interval/1000);
    }
}

static void btnSubIntervel_event_cb(lv_obj_t * btn, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) {
        up_interval -= 1000;
        /*Get the first child of the button which is the label and change its text*/
        lv_label_set_text_fmt(guider_ui.lbIntervel, "Time:%ds", up_interval/1000);
    }
}

static void btnAddPickCnt_event_cb(lv_obj_t * btn, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) {
        up_pick_cnt += 1;
        /*Get the first child of the button which is the label and change its text*/
        lv_label_set_text_fmt(guider_ui.lbPickCnt, "Cnt:%d", up_pick_cnt);
    }
}

static void btnSubPickCnt_event_cb(lv_obj_t * btn, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) {
        up_pick_cnt -= 1;
        /*Get the first child of the button which is the label and change its text*/
        lv_label_set_text_fmt(guider_ui.lbPickCnt, "Cnt:%d", up_pick_cnt);
    }
}

static void dropDownListSensor_event_cb(lv_obj_t * ddl, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) {
			  int index = lv_dropdown_get_selected(guider_ui.dropDownListSensor);
		    sensor_type = index + 1;
        /*Get the first child of the button which is the label and change its text*/
        lv_label_set_text_fmt(guider_ui.lbPickCnt, "Cnt:%d", up_pick_cnt);
    }

}
