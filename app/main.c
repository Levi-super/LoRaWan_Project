/**
* @mainpage		介绍
* @author     	Lierda-WSN-LoRaWAN-TEAM
* @version 		V1.0.3
* @date 		2020-08-30
*
* LoRaWAN_TRAIN 是基于LoRaWAN协议的传感器数据传输实验代码，该实验终端使用利尔达LoRaWAN开发板完成；以下内容包含开发板使用、重要文件说明、重要函数说明
* - 1. ICA模块工作模式与工作状态控制接口
* 	- 激活状态与休眠状态切换
* 	- 指令与透传模式的切换
* - 2. AT指令进行模块参数配置接口
* - 3. 入网接口
* - 4. 数据发送接口
* - 5. 大数据包发送接口（驱动中已实现终端的拆分包协议）
*
* @section application_arch 01 模块驱动的应用框架
*
* LoRaWAN ICA Node Driver典型使用方式：
* @image html LoRaWAN_ICA-Module-driver.png "Figure 1: Uart driver of ICA Node
*
* @section  LoRaWAN_ICA_Node_Driver API
* 更多详细信息，请参考 @ref ICA-Driver
*/



/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_it.h"
#include "lorawan_node_driver.h"
#include "usart.h"
#include "dma.h"
#include "rtc.h"
#include "app.h"
#include "gpio.h"
#include "tim.h"
#include "i2c.h"
#include "hdc1000.h"
#include "opt3001.h"
#include "MPL3115.h"
#include "mma8451.h"
#include "ST7789v.h"
#include "XPT2046.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lvgl.h"
#include "gui_guider.h"
#include "events_init.h"
/* USER CODE BEGIN 0 */
void lvgl_first_demo_start(void);
lv_ui guider_ui;
/* USER CODE END 0 */

/**
* @brief  The application entry point.
*
* @retval None
*/
int main(void)
{
    HAL_Init();

    /** 系统时钟配置 */
    SystemClock_Config();

    /** 外设初始化 */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_RTC_Init();

    /** 串口初始化 */
    MX_LPUART1_Init(9600);  //MCU与模块相连串口
    MX_USART2_Init(115200);  //MCU与PC相连串口
    MX_I2C1_Init();

    HAL_Delay(20);
    LPUART1_Clear_IT();         //清除中断并开启空闲中断
    USART2_Clear_IT();           //清除中断并开启空闲中断

    /** 温湿度传感器 */
    HDC1000_Init();

    /** 光强传感器 */
    OPT3001_Init();

    /** 气压传感器 */
    MPL3115_Init(MODE_BAROMETER);

    /** 加速度传感器 */ 
    MMA8451_Init();

    /** 液晶 */
    LCD_Init();
		
		

    /** 复位模块 */
    HAL_Delay(500);              //模块上电初始化时间
    Node_Hard_Reset();

    /** 开发板信息打印 */
    LoRaWAN_Borad_Info_Print();

    /* Infinite loop */
    /* USER CODE BEGIN WHILE  */
		lv_init(); //lvgl 系统初始化
		lv_port_disp_init(); //lvgl 显示接口初始化,放在 lv_init()的后面
		lv_port_indev_init(); //lvgl 输入接口初始化,放在 lv_init()的后面
		
		
		setup_ui(&guider_ui);
		events_init(&guider_ui);
		//lvgl_first_demo_start();
    while (1)
    {		  
        LoRaWAN_Func_Process();
    }
}

static void btn_event_cb(lv_obj_t * btn, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) {
        static uint8_t cnt = 2;
        cnt++;

        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t * label = lv_obj_get_child(btn, NULL);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
    }
}

void lvgl_first_demo_start(void)
{
    lv_obj_t * btn = lv_btn_create(lv_scr_act(), NULL);     /*Add a button the current screen*/
    lv_obj_set_pos(btn, 10, 10);                            /*Set its position*/
    lv_obj_set_size(btn, 100, 50);                          /*Set its size*/
    lv_obj_set_event_cb(btn, btn_event_cb);                 /*Assign a callback to the button*/

    lv_obj_t * label = lv_label_create(btn, NULL);          /*Add a label to the button*/
    lv_label_set_text(label, "Button");                     /*Set the labels text*/
                    /*Set the labels text*/

}

/**
* @brief System Clock Configuration
* @retval None
*/
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /* PWR CLOCK ENABLE*/
    __PWR_CLK_ENABLE();

    /**Configure the main internal regulator output voltage
    */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI;
//	RCC_OscInitStruct.LSEState = RCC_LSE_ON ;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON ;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON ;
    RCC_OscInitStruct.HSICalibrationValue = 16;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 10;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;    //PLLCLK = HSI*N/M/R = 80MHz
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /**Initializes the CPU, AHB and APB busses clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; //select the PLL as the system clock source
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;        //AHB prescaler
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;         //APB1 prescaler
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;         //APB2 prescaler

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
        Error_Handler();
    }

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2
                                         |RCC_PERIPHCLK_LPUART1|RCC_PERIPHCLK_I2C1 |RCC_PERIPHCLK_RTC;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
    PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_HSI;
    PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
    PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        Error_Handler();
    }

    /**Configure the main internal regulator output voltage
      */
    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
    {
        Error_Handler();
    }

    /**Configure the Systick interrupt time
      */
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);  //1ms 中断一次

    /**Configure the Systick
    */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE END 4 */

/**
* @brief  This function is executed in case of error occurrence.
* @param  file: The file name as string.
* @param  line: The line in file as a number.
* @retval None
*/
void _Error_Handler(char *file, int line)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    while(1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
* @brief  Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  file: pointer to the source file name
* @param  line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
    tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
* @}
*/

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
