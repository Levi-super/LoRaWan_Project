#include "judge.h"
#include "ST7789v.h"

/********************À„∑®…Ëº∆**********************/

void linearSmooth7 (float in[],float out[], int N )
{
    if(N<7)
    {
        for(int i = 0; i <= N - 1; i++ )
        {
            out[i]= in[i];
        }
    }
    else
    {
        out[0] =( 13 * in[0] + 10 * in[1] + 7 * in[2] + 4 * in[3] +
                  in[4] - 2 * in[5] - 5 * in[6] ) / 28;

        out[1] = ( 5 * in[0] + 4 * in[1] + 3 * in[2] + 2 * in[3] +
                  in[4] - in[6] ) / 14;

        out[2] = ( 7 * in[0] + 6 * in [1] + 5 * in[2] + 4 * in[3] +
                  3 * in[4] + 2 * in[5] + in[6] ) / 28;

        for (int  i = 3; i <= N - 4; i++ )
        {
            out[i] = ( in[i - 3] + in[i - 2] + in[i - 1] + in[i] + in[i + 1] + in[i + 2] + in[i + 3] ) / 7;
        }

        out[N - 3] = ( 7 * in[N - 1] + 6 * in [N - 2] + 5 * in[N - 3] +
                      4 * in[N - 4] + 3 * in[N - 5] + 2 * in[N - 6] + in[N - 7] ) / 28;

        out[N - 2] = ( 5 * in[N - 1] + 4 * in[N - 2] + 3 * in[N - 3] +
                      2 * in[N - 4] + in[N - 5] - in[N - 7] ) / 14;

        out[N - 1] = ( 13 * in[N - 1] + 10 * in[N - 2] + 7 * in[N - 3] +
                      4 * in[N - 4] + in[N - 5] - 2 * in[N - 6] - 5 * in[N - 7] ) / 28;
    }
}


float AveDdata(float data[], uint16_t size)
{
    	  int i;
        float min,max,minx,maxx,sum,ave;

        ave = sum = 0;
        min = max = data[0];
        minx = maxx = 0;
        for(i = 0; i < size; i++)
        {
            sum = sum + data[i];
            if(data[i] >= max)
            {
                max = data[i];
                maxx = i;
            }
            else if(data[i] <= min)
            {
                min = data[i];
                minx = i;
            }
        }
				sum = sum - (min + max);
				ave = sum/(size - 2);
        return ave;

}


void CopeData(float light[], float pa[], float temp[], float hum[])

{


	float data_light, data_pa, data_temp, data_hum;


    linearSmooth7(light,light,10);
    linearSmooth7(pa,pa,10);
    linearSmooth7(temp,temp,10);
    linearSmooth7(hum, hum, 10);



//    data_light = AveDdata(light);
//    data_pa = AveDdata(pa);
//    data_temp = AveDdata(temp);
//    data_hum = AveDdata(hum);


	if(data_light >= 300)
    {
//			LCD_Fill(10, 50, 200, 70, WHITE);
//			LCD_ShowString(10,50, (uint8_t *)"[Light Warning]",RED);				
      //printf("[Light Warning]\n");
    }

	else if(data_pa >= 101)
    {
//			LCD_Fill(10, 50, 200, 70, WHITE);
//			LCD_ShowString(10,50, (uint8_t *)"[Air pressure warning]",RED);				
      //printf("[Air pressure warning]\n");
    }

	else if(data_temp >= 30)
    {
//			LCD_Fill(10, 50, 200, 70, WHITE);
//			LCD_ShowString(10,50, (uint8_t *)"[Temperature warning]",RED);
      //printf("[Temperature warning]\n");
    }

	else if(data_hum >= 60)
    {
//			LCD_Fill(10, 50, 200, 70, WHITE);
//			LCD_ShowString(10,50, (uint8_t *)"[Humidity Warning]",RED);			
      //printf("[Humidity Warning]\n");
    }

    else
    {
//			LCD_Fill(10, 50, 200, 70, WHITE);
//			LCD_ShowString(10,50, (uint8_t *)"[Normal]",RED);				
      //printf("[Normal]\n");
    }
}


uint8_t * comunicate_protocol(uint8_t sensor_type, uint8_t upCount, float sensor_data)
{
	uint8_t data[9] = {0};
	uint8_t *data_point;
	data[0] = 0xAA;
	data[1] = 0x0F;
	data[2] = 0x9E;
	data[3] = sensor_type;
	data[4] = upCount;
	data[5] = ((int)sensor_data)/256;
	data[6] = ((int)sensor_data)%256;
	data[7] =  ((int)(sensor_data*100))%100;
	data[8] = 0x0F;
	
	data_point = data;
	return data_point;
}

