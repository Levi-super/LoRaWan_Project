#include <string.h>
#include "app.h"
#include "usart.h"
#include "gpio.h"
#include "lorawan_node_driver.h"
#include "hdc1000.h"
#include "sensors_test.h"
#include "opt3001.h"
#include "MPL3115.h"
#include "ST7789v.h"
#include "stdio.h"
#include "judge.h"
#include "lvgl.h"
#include "gui_guider.h"
#include "events_init.h"

#define SYSTEM_CLK_TICK HAL_GetTick()
#define LUX_MAX 300
#define PRESSURE_MAX 101
#define TEMP_MAX 30
#define HUMI_MAX 60


void ArrayToHexString(const char *buf, int len, char *out);
	
extern DEVICE_MODE_T device_mode;
extern DEVICE_MODE_T *Device_Mode_str;
extern lv_ui guider_ui;
down_list_t *pphead = NULL;

 uint16_t up_interval = 30000;   //ms
 uint16_t up_pick_cnt = 10;     //次
 float up_temper = 0, up_humi = 0, up_lux = 0, up_pressure = 0;
 float selected_up_data = 0;
uint8_t data[9];
char warning[30];	
char out[30];		
char receive_data[200];

const char deveui_my[50] = "AT+DEVEUI=009569000000F59E,D391010220102816,1\r\n";
const char appeui_my[50] = "AT+APPEUI=FDC5D8B6B84A9741\r\n";
const char appkey_my[50] = "AT+APPKEY=AF6E9C711B5BBCDA709F78ED71F235AF\r\n";

/*sensor_type
0x01：光照传感器
0x02：气压传感器
0x03：温度传感器
0x04：湿度传感器
*/
uint8_t sensor_type = 0x01;


//-----------------Users application--------------------------
void LoRaWAN_Func_Process(void)
{
    static DEVICE_MODE_T dev_stat = NO_MODE;

    uint16_t temper = 0;

    switch((uint8_t)device_mode)
    {
    /* 指令模式 */
    case CMD_CONFIG_MODE:
    {
        /* 如果不是command Configuration function, 则进入if语句,只执行一次 */
        if(dev_stat != CMD_CONFIG_MODE)
        {
            dev_stat = CMD_CONFIG_MODE;
            debug_printf("\r\n[Command Mode]\r\n");

            nodeGpioConfig(wake, wakeup);
            nodeGpioConfig(mode, command);
        }
        /* 等待usart2产生中断 */
        if(UART_TO_PC_RECEIVE_FLAG)
        {
            UART_TO_PC_RECEIVE_FLAG = 0;
            lpusart1_send_data(UART_TO_PC_RECEIVE_BUFFER,UART_TO_PC_RECEIVE_LENGTH);
        }
        /* 等待lpuart1产生中断 */
        if(UART_TO_LRM_RECEIVE_FLAG)
        {
            UART_TO_LRM_RECEIVE_FLAG = 0;
            usart2_send_data(UART_TO_LRM_RECEIVE_BUFFER,UART_TO_LRM_RECEIVE_LENGTH);
        }
    }
    break;

    /* 透传模式 */
    case DATA_TRANSPORT_MODE:
    {
        /* 如果不是data transport function,则进入if语句,只执行一次 */
        if(dev_stat != DATA_TRANSPORT_MODE)
        {
            dev_stat = DATA_TRANSPORT_MODE;
            debug_printf("\r\n[Transperant Mode]\r\n");

            /* 模块入网判断 */
            if(nodeJoinNet(JOIN_TIME_120_SEC) == false)
            {
                return;
            }

            temper = HDC1000_Read_Temper()/1000;

            nodeDataCommunicate((uint8_t*)&temper,sizeof(temper),&pphead);
        }

        /* 等待usart2产生中断 */
        if(UART_TO_PC_RECEIVE_FLAG && GET_BUSY_LEVEL)  //Ensure BUSY is high before sending data
        {
            UART_TO_PC_RECEIVE_FLAG = 0;
            nodeDataCommunicate((uint8_t*)UART_TO_PC_RECEIVE_BUFFER, UART_TO_PC_RECEIVE_LENGTH, &pphead);
        }

        /* 如果模块正忙, 则发送数据无效，并给出警告信息 */
        else if(UART_TO_PC_RECEIVE_FLAG && (GET_BUSY_LEVEL == 0))
        {
            UART_TO_PC_RECEIVE_FLAG = 0;
            debug_printf("--> Warning: Don't send data now! Module is busy!\r\n");
        }

        /* 等待lpuart1产生中断 */
        if(UART_TO_LRM_RECEIVE_FLAG)
        {
            UART_TO_LRM_RECEIVE_FLAG = 0;
            usart2_send_data(UART_TO_LRM_RECEIVE_BUFFER,UART_TO_LRM_RECEIVE_LENGTH);
        }
    }
    break;

    /*工程模式*/
    case PRO_TRAINING_MODE:
    {
        /* 如果不是Class C云平台数据采集模式, 则进入if语句,只执行一次 */
        if(dev_stat != PRO_TRAINING_MODE)
        {
            dev_stat = PRO_TRAINING_MODE;
            debug_printf("\r\n[Project Mode]\r\n");
										
						nodeGpioConfig(wake, wakeup);
						nodeGpioConfig(mode, command);
						
						lpusart1_send_data((uint8_t *)deveui_my,strlen(deveui_my));
						HAL_Delay(100);
						lpusart1_send_data((uint8_t *)appeui_my,strlen(appeui_my));
						HAL_Delay(100);
						lpusart1_send_data((uint8_t *)appkey_my,strlen(appkey_my));
						HAL_Delay(100);
						lpusart1_send_data((uint8_t *)"AT+CLASS=0\r\n",12);
						HAL_Delay(100);
						lpusart1_send_data((uint8_t *)"AT+DEBUG=0\r\n",12);
						HAL_Delay(100);
						lpusart1_send_data((uint8_t *)"AT+CONFIRM=1\r\n",14);
						HAL_Delay(100);
						lpusart1_send_data((uint8_t *)"AT+SAVE\r\n",9);
						HAL_Delay(100);
						lpusart1_send_data((uint8_t *)"AT+RESET\r\n",10);
						HAL_Delay(100);					
						/* 模块入网判断 */
						if(nodeJoinNet(JOIN_TIME_120_SEC) == false)
						{
								return;
						}
						
						HDC1000_Test();
						OPT3001_Test();
						MPL3115_Test();								
        }
							
			/* 你的实验代码位置 */
								
			static uint16_t getdata_cnt = 0;	
			static uint16_t clk_cnt = 0;
			static uint16_t updata_cnt = 0;
			
			static uint16_t data_prc_flag = 1;
			static uint16_t data_ready_flag = 0;
				
			static float temper_array[30];
			static float humi_array[30];
			static float lux_array[30];
			static float pressure_array[30];
			
			lv_task_handler();   //lvgl运行的事件处理监测入口
				
			static uint32_t pre_tick = 0;
			if(pre_tick == 0)	pre_tick = SYSTEM_CLK_TICK;
				
       //判断是否到了设定的采样时间				
		  if (SYSTEM_CLK_TICK - pre_tick >= up_interval / up_pick_cnt)
			{
				pre_tick = SYSTEM_CLK_TICK;
			  timeout_start_flag = true;
				
        if(data_prc_flag == 1 && getdata_cnt < up_pick_cnt)
				{					
	       /*数据
					 温度: 28.13 ℃   湿度: 63.32% 
					 照度: 24.25 Lux 
					 气压: 100.61 KPa 
					*/
					float temper = 0, humi = 0;
					temper = HDC1000_Read_Temper()/1000.0;
					humi = HDC1000_Read_Humidi()/1000.0;					
					temper_array[getdata_cnt] = temper;
					humi_array[getdata_cnt] = humi;

					float lux = 0;
					uint16_t result;
					result = OPT3001_Result();
					lux = 0.01*(1 << ((result & 0xF000) >> 12))*(result & 0xFFF);
					lux_array[getdata_cnt] = lux;
					
					float pressure = 0;
					pressure = MPL3115_ReadPressure()/1000.0;
					pressure_array[getdata_cnt] = pressure;
					
				  getdata_cnt++;
					if (getdata_cnt == up_pick_cnt)
					{
						getdata_cnt = 0;
						data_ready_flag = 1;   //十次数据准备好了						
					}else
					{
						data_ready_flag = 0;
					}					
			}
			
       //开始进行数据处理			
			if (data_ready_flag == 1)
			{				 		
			   data_prc_flag = 0;
				 data_ready_flag = 0;
			   //////////////数据处理/////////////
				
				 //去除数据的最大值最小值，得到平均值
				 up_temper = AveDdata(temper_array, up_pick_cnt - 2);
			   up_humi = AveDdata(humi_array, up_pick_cnt - 2);
				 up_lux = AveDdata(lux_array, up_pick_cnt - 2);
				 up_pressure = AveDdata(pressure_array, up_pick_cnt - 2);
				
					
					//根据选择的传感器类型实现相应的功能
					switch(sensor_type)
					{
						case 1:
						{
							selected_up_data = up_lux; 
							if ( selected_up_data > LUX_MAX)
							{
								HAL_GPIO_WritePin(LedGpio_D6, LedPin_D6, GPIO_PIN_RESET);
								sprintf(warning,"[Light Warning]");
							}else
							{
								sprintf(warning,"[Normal]");
								HAL_GPIO_WritePin(LedGpio_D6, LedPin_D6, GPIO_PIN_SET);
							}							  
						}
						break;
						case 2:
						{
							selected_up_data = up_pressure; 
							if ( selected_up_data > PRESSURE_MAX)
							{
								sprintf(warning,"[Pressure Warning!]");
							  HAL_GPIO_WritePin(LedGpio_D7, LedPin_D7, GPIO_PIN_RESET);							
							}	
							else
							{
								sprintf(warning,"[Normal]");
								HAL_GPIO_WritePin(LedGpio_D7, LedPin_D7, GPIO_PIN_SET);	
							}									
						}							
						break;
						case 3: 
						{
							selected_up_data = up_temper;
							if( selected_up_data > TEMP_MAX)
							{
								sprintf(warning,"[Temp Warning!]");
							  HAL_GPIO_WritePin(LedGpio_D8, LedPin_D8, GPIO_PIN_RESET);
							}else{
								sprintf(warning,"[Normal]");
								HAL_GPIO_WritePin(LedGpio_D8, LedPin_D8, GPIO_PIN_SET);
							}
						}
						break;
						case 4: 
						{
							selected_up_data = up_humi; 
							if(selected_up_data > HUMI_MAX)
							{
								sprintf(warning,"[Humi Warning!]");								
								HAL_GPIO_WritePin(LedGpio_D11, LedPin_D11, GPIO_PIN_RESET);
							}
							else
							{
								sprintf(warning,"[Normal]");
								HAL_GPIO_WritePin(LedGpio_D11, LedPin_D11, GPIO_PIN_SET);
							}							
						}
						break;
						default: break;											
					}
				  
					
					//debug_printf("sensor type:%d  data:%.2f",sensor_type, selected_up_data);
					
					//生成数据通信格式
					data[0] = 0xAA;
					data[1] = 0x0F;
					data[2] = 0x9E;
					data[3] = sensor_type;
					data[4] = updata_cnt++;
					data[5] = ((int)selected_up_data)/256;
					data[6] = ((int)selected_up_data)%256;
					data[7] =  ((int)(selected_up_data*100))%100;
					data[8] = 0x0F;
				 
					/////////////////////////////////
			    data_prc_flag = 1;	
					
				 //上传数据
				  nodeDataCommunicate((uint8_t *)data, 9, &pphead);
					
					//界面显示
					sprintf(receive_data,"%s",pphead->down_info.business_data);							
					char State_str[10];
					if(receive_data[0] == 'H' && receive_data[2] == 'G')
					{
					  sprintf(State_str, "Error!");
					}else
					{
						sprintf(State_str, "OK!");
					}					
				  ArrayToHexString(data, 9, out);
				 			
				 lv_label_set_text_fmt(guider_ui.message_lb, 
											"DevEui:009569000000F59E\n"
											"UpDat:%s\n"
											"State:%s  %s\n",
											 out, State_str,warning);
				 lv_label_set_text_fmt(guider_ui.Receive_lb,"Receive:%s", receive_data);
				
				} 																										  													
			}
					
			/* 等待lpuart1产生中断 */
			if(UART_TO_LRM_RECEIVE_FLAG)
			{
					UART_TO_LRM_RECEIVE_FLAG = 0;
					usart2_send_data(UART_TO_LRM_RECEIVE_BUFFER,UART_TO_LRM_RECEIVE_LENGTH);
			}			  																
    }
    break;

    default:
        break;
    }
}


/**
 * @brief   开发板版本信息和按键使用说明信息打印
 * @details 上电所有灯会短暂亮100ms
 * @param   无
 * @return  无
 */
void LoRaWAN_Borad_Info_Print(void)
{
    debug_printf("\r\n\r\n");
    PRINT_CODE_VERSION_INFO("%s",CODE_VERSION);
    debug_printf("\r\n");
    debug_printf("--> Press Key1 to: \r\n");
    debug_printf("-->  - Enter command Mode\r\n");
    debug_printf("-->  - Enter Transparent Mode\r\n");
    debug_printf("--> Press Key2 to: \r\n");
    debug_printf("-->  - Enter Project Trainning Mode\r\n");
    LEDALL_ON;
    HAL_Delay(100);
    LEDALL_OFF;
}


/*
 *	@param1		buf:输入数组
 *	@param2		len:输入数组的长度
 *	@param3		out:输出的字符串
 */
void ArrayToHexString(const char *buf, int len, char *out)
{
#define	IntToASCII(c)	(c)>9?((c)+0x37):((c)+0x30);

	char temp;
	int i = 0;

	for (i = 0; i < len; i++) {
		temp = buf[i]&0xf0;
		out[2 * i + 0] = IntToASCII(temp>>4);
		temp = buf[i]&0x0f;
		out[2 * i + 1] = IntToASCII(temp);
//		/* space */
//		out[3 * i + 2] = 0x20;
	}

	out[2 * i] = '\0';

}



