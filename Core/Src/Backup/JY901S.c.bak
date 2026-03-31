#include "JY901S.h"
#include "stdlib.h"
#include "string.h"

JY901S *JY901S_instance; //用来存放JY901S模块的地址

/******************************************
发送给JY901S模块的指令，发送步骤：
1、解锁寄存器：0XFF, 0XAA, 0X01, 0X01, 0X00
2、发送需要修改的指令
3、保存命令：0XFF, 0XAA, 0X00, 0X00, 0X00
******************************************/
uint8_t unlock_data[5] = {0XFF, 0XAA, 0X69, 0X88, 0XB5};
uint8_t acc_cali_data[5] = {0XFF, 0XAA, 0X01, 0X01, 0X00}; //加速度计校准
uint8_t angle_zero_data[5] = {0XFF, 0XAA, 0X01, 0X08, 0X00}; //XY轴角度归零
uint8_t save_data[5] = {0XFF, 0XAA, 0X00, 0X00, 0X00};

void JY901S_Init()
{
    memset(&JY901S_instance, 0, sizeof(JY901S_instance));
}

JY901S* JY901S_Create()
{
    JY901S *obj = (JY901S*)malloc(sizeof(JY901S));
    memset(obj, 0, sizeof(JY901S));

    JY901S_instance = obj;
    return obj;
}

void JY901S_AccCali()
{   
    BSP_UART_Send(JY901S_UART, unlock_data, sizeof(unlock_data), 100);
    //HAL_Delay(1000);
    BSP_UART_Send(JY901S_UART, acc_cali_data, sizeof(acc_cali_data), 100);
    //HAL_Delay(5000);   
    BSP_UART_Send(JY901S_UART, save_data, sizeof(save_data), 100);
}

void JY901S_SetZero()
{
    BSP_UART_Send(JY901S_UART, unlock_data, sizeof(unlock_data), 100);
    HAL_Delay(1000);
    BSP_UART_Send(JY901S_UART, angle_zero_data, sizeof(angle_zero_data), 100);
    HAL_Delay(3000);
    BSP_UART_Send(JY901S_UART, save_data, sizeof(save_data), 100);
}

void JY901S_RxCallback(uint8_t *data)
{

// // if(data[1] == 0x53)
//     // {
//     //     JY901S_instance->angleX = (float)((short)(((short)data[3]<<8)|data[2]))/32768*180;
//     //     JY901S_instance->angleY = (float)((short)(((short)data[5]<<8)|data[4]))/32768*180;
//     //     JY901S_instance->angleZ = (float)((short)(((short)data[7]<<8)|data[6]))/32768*180;
//     // }

//     if(data[1] == 0x52)
//     {
//         //角速度
//         JY901S_instance->velocityX = (float)((short)(((short)data[3]<<8)|data[2]))/32768*2000;
//         JY901S_instance->velocityY = (float)((short)(((short)data[5]<<8)|data[4]))/32768*2000;
//         JY901S_instance->velocityZ = (float)((short)(((short)data[7]<<8)|data[6]))/32768*2000;

//         if(data[12] == 0x53)
//         {
//             //角度
//             JY901S_instance->angleX = (float)((short)(((short)data[14]<<8)|data[13]))/32768*180;
//             JY901S_instance->angleY = (float)((short)(((short)data[16]<<8)|data[15]))/32768*180;
//             JY901S_instance->angleZ = (float)((short)(((short)data[18]<<8)|data[17]))/32768*180;
//         }
        
//     }

    uint16_t i = 0;
    uint16_t data_len = 255;
    
    while(i < data_len - 10)
    {
        if(data[i] == 0x55)
        {
            uint8_t packet_type = data[i + 1];
            
            switch(packet_type)
            {
                case 0x52:
                    //角速度
                    JY901S_instance->omega[0] = (float)((short)(((short)data[i + 3] << 8) | data[i + 2])) / 32768.0f * 2000.0f;
                    JY901S_instance->omega[1] = (float)((short)(((short)data[i + 5] << 8) | data[i + 4])) / 32768.0f * 2000.0f;
                    JY901S_instance->omega[2] = (float)((short)(((short)data[i + 7] << 8) | data[i + 6])) / 32768.0f * 2000.0f;
                    // JY901S_instance->omega[0] = JY901S_instance->velocityX;
                    // JY901S_instance->omega[1] = JY901S_instance->velocityY;
                    // JY901S_instance->omega[2] = JY901S_instance->velocityZ;
                    break;
                    
                case 0x53:
                    //角度
                    JY901S_instance->euler[0] = (float)((short)(((short)data[i + 3] << 8) | data[i + 2])) / 32768.0f * 180.0f;
                    JY901S_instance->euler[1] = (float)((short)(((short)data[i + 5] << 8) | data[i + 4])) / 32768.0f * 180.0f;
                    JY901S_instance->euler[2] = (float)((short)(((short)data[i + 7] << 8) | data[i + 6])) / 32768.0f * 180.0f;
                    // JY901S_instance->euler[0] = JY901S_instance->angleX;
                    // JY901S_instance->euler[1] = JY901S_instance->angleY;
                    // JY901S_instance->euler[2] = JY901S_instance->angleZ;
                    break;
                    
                case 0x59:
                    //四元数
                    JY901S_instance->q[0] = (float)((short)(((short)data[i + 3] << 8) | data[i + 2])) / 32768.0f;
                    JY901S_instance->q[1] = (float)((short)(((short)data[i + 5] << 8) | data[i + 4])) / 32768.0f;
                    JY901S_instance->q[2] = (float)((short)(((short)data[i + 7] << 8) | data[i + 6])) / 32768.0f;
                    JY901S_instance->q[3] = (float)((short)(((short)data[i + 9] << 8) | data[i + 8])) / 32768.0f;
                    break;
                    
                default:
                    break;
            }
            
            i += 11;
        }
        else
        {
            i++;
        }
    }
}