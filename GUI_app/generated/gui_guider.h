/*
 * Copyright 2021 NXP
 * SPDX-License-Identifier: MIT
 */

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"
#include "guider_fonts.h"

typedef struct
{
	lv_obj_t *screen;
	lv_obj_t *screen_tabview0;
	lv_obj_t *message_lb;
	lv_obj_t *Receive_lb;

	
	lv_obj_t *btnAddIntervel;
	lv_obj_t *btnSubIntervel;
	lv_obj_t *lbIntervel;
	
	lv_obj_t *btnAddPickCnt;
	lv_obj_t *btnSubPickCnt;
	lv_obj_t *lbPickCnt;
	
	lv_obj_t *dropDownListSensor;
	
}lv_ui;

void setup_ui(lv_ui *ui);
extern lv_ui guider_ui;
void setup_scr_screen(lv_ui *ui);

#ifdef __cplusplus
}
#endif
#endif