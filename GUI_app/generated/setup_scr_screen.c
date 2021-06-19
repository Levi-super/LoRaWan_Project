/*
 * Copyright 2021 NXP
 * SPDX-License-Identifier: MIT
 */

#include "lvgl/lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"


void setup_scr_screen(lv_ui *ui){

	//Write codes screen
	ui->screen = lv_obj_create(NULL, NULL);

	//Write codes screen_tabview0
	ui->screen_tabview0 = lv_tabview_create(ui->screen, NULL);

	//Write style LV_TABVIEW_PART_BG for screen_tabview0
	static lv_style_t style_screen_tabview0_bg;
	lv_style_init(&style_screen_tabview0_bg);

	//Write style state: LV_STATE_DEFAULT for style_screen_tabview0_bg
	lv_style_set_bg_color(&style_screen_tabview0_bg, LV_STATE_DEFAULT, lv_color_make(0xea, 0xef, 0xf3));
	lv_style_set_bg_grad_color(&style_screen_tabview0_bg, LV_STATE_DEFAULT, lv_color_make(0xea, 0xef, 0xf3));
	lv_style_set_bg_grad_dir(&style_screen_tabview0_bg, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_tabview0_bg, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_screen_tabview0_bg, LV_STATE_DEFAULT, lv_color_make(0xc0, 0xc0, 0xc0));
	lv_style_set_border_width(&style_screen_tabview0_bg, LV_STATE_DEFAULT, 0);
	lv_style_set_text_color(&style_screen_tabview0_bg, LV_STATE_DEFAULT, lv_color_make(0x4d, 0x4d, 0x4d));
	lv_style_set_text_font(&style_screen_tabview0_bg, LV_STATE_DEFAULT, &lv_font_simsun_24);
	lv_style_set_text_letter_space(&style_screen_tabview0_bg, LV_STATE_DEFAULT, 2);
	lv_style_set_text_line_space(&style_screen_tabview0_bg, LV_STATE_DEFAULT, 16);
	lv_obj_add_style(ui->screen_tabview0, LV_TABVIEW_PART_BG, &style_screen_tabview0_bg);

	//Write style LV_TABVIEW_PART_INDIC for screen_tabview0
	static lv_style_t style_screen_tabview0_indic;
	lv_style_init(&style_screen_tabview0_indic);

	//Write style state: LV_STATE_DEFAULT for style_screen_tabview0_indic
	lv_style_set_bg_color(&style_screen_tabview0_indic, LV_STATE_DEFAULT, lv_color_make(0x01, 0xa2, 0xb1));
	lv_style_set_bg_grad_color(&style_screen_tabview0_indic, LV_STATE_DEFAULT, lv_color_make(0x01, 0xa2, 0xb1));
	lv_style_set_bg_grad_dir(&style_screen_tabview0_indic, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_tabview0_indic, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->screen_tabview0, LV_TABVIEW_PART_INDIC, &style_screen_tabview0_indic);

	//Write style LV_TABVIEW_PART_TAB_BG for screen_tabview0
	static lv_style_t style_screen_tabview0_tab_bg;
	lv_style_init(&style_screen_tabview0_tab_bg);

	//Write style state: LV_STATE_DEFAULT for style_screen_tabview0_tab_bg
	lv_style_set_bg_color(&style_screen_tabview0_tab_bg, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_tabview0_tab_bg, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_tabview0_tab_bg, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_tabview0_tab_bg, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_screen_tabview0_tab_bg, LV_STATE_DEFAULT, lv_color_make(0xc0, 0xc0, 0xc0));
	lv_style_set_border_width(&style_screen_tabview0_tab_bg, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_left(&style_screen_tabview0_tab_bg, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_screen_tabview0_tab_bg, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_screen_tabview0_tab_bg, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_screen_tabview0_tab_bg, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_tabview0, LV_TABVIEW_PART_TAB_BG, &style_screen_tabview0_tab_bg);

	//Write style LV_TABVIEW_PART_TAB_BTN for screen_tabview0
	static lv_style_t style_screen_tabview0_tab_btn;
	lv_style_init(&style_screen_tabview0_tab_btn);

	//Write style state: LV_STATE_DEFAULT for style_screen_tabview0_tab_btn
	lv_style_set_text_color(&style_screen_tabview0_tab_btn, LV_STATE_DEFAULT, lv_color_make(0x4d, 0x4d, 0x4d));
	lv_style_set_text_letter_space(&style_screen_tabview0_tab_btn, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->screen_tabview0, LV_TABVIEW_PART_TAB_BTN, &style_screen_tabview0_tab_btn);
	lv_obj_set_pos(ui->screen_tabview0, 0, 0);
	lv_obj_set_size(ui->screen_tabview0, 240, 320);
	lv_obj_t *screen_tabview0_message = lv_tabview_add_tab(ui->screen_tabview0,"message");
	lv_obj_t *screen_tabview0_setting = lv_tabview_add_tab(ui->screen_tabview0,"setting");

	lv_tabview_set_anim_time(ui->screen_tabview0, 300);
	
	//通信界面显示
	static lv_style_t font_style;
	lv_style_init(&font_style);		
	lv_style_set_text_font(&font_style, LV_STATE_DEFAULT, &lv_font_montserrat_12);
	
	ui->message_lb = lv_label_create(screen_tabview0_message, NULL);
  lv_obj_add_style(ui->message_lb,LV_LABEL_PART_MAIN, &font_style);
	lv_obj_set_pos(ui->message_lb, 10, 10); 
	lv_obj_set_size(ui->message_lb, 230, 150);
  lv_label_set_long_mode(ui->message_lb, LV_LABEL_LONG_EXPAND);
	lv_label_set_text(ui->message_lb, 
							"DevEui:009569000000F59E\n"
						  "UpDat: \n"
						  "State:  \n");						
							
	ui->Receive_lb = lv_label_create(screen_tabview0_message, NULL);
  lv_obj_add_style(ui->Receive_lb,LV_LABEL_PART_MAIN, &font_style);
  lv_label_set_long_mode(ui->Receive_lb, LV_LABEL_LONG_BREAK);
	lv_obj_set_size(ui->Receive_lb, 230, 150); //设置label大小，放在长文本模式后
	lv_obj_set_pos(ui->Receive_lb, 10, 100); 
  lv_label_set_text(ui->Receive_lb, "Receive:");

	//参数设置界面
	//增加时间间隔
	ui->btnAddIntervel = lv_btn_create(screen_tabview0_setting, NULL);
	lv_obj_set_pos(ui->btnAddIntervel, 10, 10);                            /*Set its position*/
	lv_obj_set_size(ui->btnAddIntervel, 70, 25);
	lv_obj_t * label1 = lv_label_create(ui->btnAddIntervel, NULL);          /*Add a label to the button*/
	lv_obj_add_style(label1, LV_LABEL_PART_MAIN, &font_style);
	lv_label_set_text(label1, "AddTime");
	
	//减少时间间隔
	ui->btnSubIntervel = lv_btn_create(screen_tabview0_setting, NULL);
	lv_obj_set_pos(ui->btnSubIntervel, 100, 10);                            /*Set its position*/
	lv_obj_set_size(ui->btnSubIntervel, 70, 25);
	lv_obj_t * label2 = lv_label_create(ui->btnSubIntervel, NULL);          /*Add a label to the button*/
	lv_obj_add_style(label2,LV_LABEL_PART_MAIN, &font_style);
	lv_label_set_text(label2, "SubTime");
	ui->lbIntervel = lv_label_create(screen_tabview0_setting, NULL);
	lv_obj_add_style(ui->lbIntervel,LV_LABEL_PART_MAIN, &font_style);
	lv_obj_set_pos(ui->lbIntervel, 10, 40);
	lv_label_set_text(ui->lbIntervel, "Time:30s");	
	
	//增加采样次数
	ui->btnAddPickCnt = lv_btn_create(screen_tabview0_setting, NULL);
	lv_obj_set_pos(ui->btnAddPickCnt, 10, 70);                            /*Set its position*/
	lv_obj_set_size(ui->btnAddPickCnt, 70, 25);
	lv_obj_t * label3 = lv_label_create(ui->btnAddPickCnt, NULL);          /*Add a label to the button*/
	lv_obj_add_style(label3, LV_LABEL_PART_MAIN, &font_style);
	lv_label_set_text(label3, "AddCnt");
	
	//减少采样次数
	ui->btnSubPickCnt = lv_btn_create(screen_tabview0_setting, NULL);
	lv_obj_set_pos(ui->btnSubPickCnt, 100, 70);                            /*Set its position*/
	lv_obj_set_size(ui->btnSubPickCnt, 70, 25);
	lv_obj_t * label4 = lv_label_create(ui->btnSubPickCnt, NULL);          /*Add a label to the button*/
	lv_obj_add_style(label4,LV_LABEL_PART_MAIN, &font_style);
	lv_label_set_text(label4, "SubCnt");
	ui->lbPickCnt = lv_label_create(screen_tabview0_setting, NULL);
	lv_obj_add_style(ui->lbPickCnt,LV_LABEL_PART_MAIN, &font_style);
	lv_obj_set_pos(ui->lbPickCnt, 10, 100);
	lv_label_set_text(ui->lbPickCnt, "Cnt:10");
	
	ui->dropDownListSensor = lv_dropdown_create(screen_tabview0_setting, NULL);
	lv_dropdown_set_options(ui->dropDownListSensor,
															"Light Sensor\n"
															"Pressure Sensor\n"
															"Temp Sensor\n"
															"Humi Sensor\n");
															
	lv_obj_set_pos(ui->dropDownListSensor, 10, 140);
	lv_obj_set_size(ui->dropDownListSensor, 200, 30);

}


