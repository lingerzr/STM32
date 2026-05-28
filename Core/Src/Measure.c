#include "main.h"
#include "Measure.h"

uint32_t N_A1 = 0, N_A2 = 0;
uint32_t N_B1 = 0, N_B2 = 0;
uint8_t Gate_state = 0;
double frequency = 0.0;

void Start_Equal_Presicion_Measurement(void)
{
    HAL_TIM_Base_Start(&htim2);
    HAL_TIM_Base_Start(&htim5);

    Gate_state = 0;

    HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_1);     // 启动输入捕获中断
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM5) {
        if (Gate_state == 0) {

            N_B1 = HAL_TIM_ReadCapturedValue(&htim5, TIM_CHANNEL_1); // 捕获到的计数值
            N_A1 = htim2.Instance->CNT; // 读取当前计数值
            Gate_state = 1;
            HAL_TIM_IC_Stop(&htim5, TIM_CHANNEL_1);
        }
        else if (Gate_state == 1) {
            N_B2 = HAL_TIM_ReadCapturedValue(&htim5, TIM_CHANNEL_1); // 捕获到的计数值
            N_A2 = htim2.Instance->CNT; // 读取当前计数值
            Gate_state = 2;
            HAL_TIM_IC_Stop(&htim5, TIM_CHANNEL_1);

        }
    }
}

double Calculate_Frequency(void)
{
    Start_Equal_Presicion_Measurement();

    HAL_Delay(1000);

    if (Gate_state == 1){
        HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_1);     // 重新启动输入捕获中断
    }

    while(Gate_state != 2) {
        // 等待测量完成
    }

    uint32_t N_A = N_A2 - N_A1; 
    uint32_t N_B = N_B2 - N_B1; 
    
    return ((double)N_A / (double)N_B) * 84000000.0;
}
