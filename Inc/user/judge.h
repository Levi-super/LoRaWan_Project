#ifndef __JUDGE_H
#define __JUDGE_H

#include "stdio.h"
#include "stdint.h"


void linearSmooth7 (float in[],float out[], int N );

float AveDdata(float data[], uint16_t size);

void CopeData(float light[], float pa[], float temp[], float hum[]);

uint8_t * comunicate_protocol(uint8_t sensor_type, uint8_t upCount, float sensor_data);

#endif
