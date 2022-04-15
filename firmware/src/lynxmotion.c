#include <stdio.h>

#include <stdlib.h>
#include "lynxmotion.h"

// parse query msg to int
static int32_t lynxmotion_parse(char *txt)
{
    uint8_t start = 0, end = 0;
    char pos_str[10];

    while (txt[start] != 'D')start++;
    
    start++;

    end = start;
    
    while (txt[end] != '\r') end++;
        
    memcpy(pos_str,txt+start,end-start);

    pos_str[end-start] = 0;

    return atol(pos_str);
}

uint8_t lynxmotion_OnMessage(can_t lynxmotion_CanMsg)
{
    uint8_t lynxmotion_id;
    int16_t lynxmotion_position;
    uint16_t lynxmotion_speed;
//    uint8_t ret;
            
    char command_str[20];
    size_t rxmsg_length = 0;
    
    if(lynxmotion_CanMsg.ID != LYNXMOTION_CANID)
        return -1;
    
    lynxmotion_id = lynxmotion_CanMsg.data[0];
    lynxmotion_position = ((int16_t)lynxmotion_CanMsg.data[1]<<8)|lynxmotion_CanMsg.data[2];
    lynxmotion_speed = ((uint16_t)lynxmotion_CanMsg.data[3]<<8)|lynxmotion_CanMsg.data[4];
    
    sprintf(command_str, "#%uSD%d\r", lynxmotion_id, lynxmotion_speed);
    
    UART6_Write((uint8_t*)command_str, strlen(command_str));
    
    uint16_t timeout = 0;
    while(UART6_WriteCountGet()!=0 && timeout <= LYNXMOTION_TIMEOUT)
    {
        timeout++;
        CORETIMER_DelayMs(1);
    }
    
    //CORETIMER_DelayMs(LYNXMOTION_TX_DELAY);
    
    sprintf(command_str, "#%uD%d\r", lynxmotion_id, lynxmotion_position);
    UART6_Write((uint8_t*)command_str, strlen(command_str));
    
    timeout = 0;
    while(UART6_WriteCountGet()!=0 && timeout <= LYNXMOTION_TIMEOUT)
    {
        timeout++;
        CORETIMER_DelayMs(1);
    }
    
    //CORETIMER_DelayMs(LYNXMOTION_TX_DELAY);
    
    // query position
    sprintf(command_str, "#%uQD\r", lynxmotion_id);
    UART6_Write((uint8_t*)command_str, strlen(command_str));
    timeout = 0;
    while(UART6_WriteCountGet()!=0 && timeout <= LYNXMOTION_TIMEOUT)
    {
        timeout++;
        CORETIMER_DelayMs(1);
    }
    
    CORETIMER_DelayMs(LYNXMOTION_RX_DELAY);
    
    rxmsg_length = UART6_ReadCountGet();
    
    char rx_string[20];
    
    if(rxmsg_length > 0 && rxmsg_length < 20)
    {
        uint8_t i = 0;
        while(rxmsg_length > (size_t)0 )
        {
            UART6_Read((uint8_t*)(&rx_string[i]), 1);
            i++;
            rxmsg_length = UART6_ReadCountGet();
        }
        rx_string[i] = 0;
    }
    
    //ret = sscanf(rx_string, "*%uQD%d\r", (unsigned int *)&lynxmotion_id, (int *)&lynxmotion_position);
    
    lynxmotion_position = (int16_t) lynxmotion_parse(rx_string);
    // query speed
    sprintf(command_str, "#%uQSD\r", lynxmotion_id);
    UART6_Write((uint8_t*)command_str, strlen(command_str));
    
    timeout = 0;
    while(UART6_WriteCountGet()!=0 && timeout <= LYNXMOTION_TIMEOUT)
    {
        timeout++;
        CORETIMER_DelayMs(1);
    }
    
    CORETIMER_DelayMs(LYNXMOTION_RX_DELAY);
    
    rxmsg_length = UART6_ReadCountGet();
    
    
    if(rxmsg_length > 0)
    {
        uint8_t i = 0;
        while(rxmsg_length > (size_t)0)
        {
            UART6_Read((uint8_t*)(&rx_string[i]), 1);
            i++;
            rxmsg_length = UART6_ReadCountGet();
        }
        rx_string[i] = 0;
    }
    // *00QSD1000\r
    //ret = sscanf(rx_string, "*%uQSD%u\r", (unsigned int *)&lynxmotion_id, (unsigned int *)&lynxmotion_speed);
    
    lynxmotion_speed = (uint16_t) lynxmotion_parse(rx_string);
    
    uint8_t rxpacket[5];
    rxpacket[0] = lynxmotion_id;
    rxpacket[1] = (uint8_t) (lynxmotion_position >> 8);
    rxpacket[2] = (uint8_t) lynxmotion_position;
    rxpacket[3] = (uint8_t) (lynxmotion_speed >> 8);
    rxpacket[4] = (uint8_t) lynxmotion_speed;
    
    while(CAN4_MessageTransmit(LYNXMOTION_CANID+1, 5, (uint8_t*)(rxpacket), 0, CAN_MSG_TX_DATA_FRAME) == false);
    
    return 0 ;
   
}
