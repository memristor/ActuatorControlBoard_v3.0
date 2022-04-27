/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#include "definitions.h"


static VacuumPump instances[VACUUM_PUMP_CONFIG_COUNT_MAX];

static uint8_t count = 0;
extern volatile uint32_t sys_time;


void VacuumPump_Add(uint8_t pumpPin, uint8_t switchPin, uint16_t number)
{
    // Initialize Vacuum Pumps
    instances[count].pumpPin = pumpPin;
    instances[count].switchPin = switchPin;
    instances[count].CanID = VACUUM_PUMP_CANID + number;
    instances[count].state = VACUUM_PUMP_STATE_OFF;
    
    // Return index
    count++;
}

int VacuumPump_OnMessage(can_t Pump_CanMsg)
{
    
    for(int i = 0; i < count; i++)
    {
        if(Pump_CanMsg.ID == instances[i].CanID)
        {
            
            switch(Pump_CanMsg.data[0])
            {
                case 0:
                    SinglePump_State(instances[i].pumpPin, PIN_LOW);
                    SingleSwitch_State(instances[i].switchPin, PIN_HIGH);
                    instances[i].state = VACUUM_PUMP_STATE_RELEASING;
                    instances[i].delay_counter = sys_time;
                    //CORETIMER_DelayMs(VACUUM_SWITCH_RELASE_TIME);
                    //SingleSwitch_State(instances[i].switchPin, PIN_LOW);
                    break;
                case 1:
                    SinglePump_State(instances[i].pumpPin, PIN_HIGH);
                    SingleSwitch_State(instances[i].switchPin, PIN_LOW);
                    instances[i].state = VACUUM_PUMP_STATE_ON;
                    break;
            }
           
            return 0;
        }
    }
    
    return -1;
    
}

void SinglePump_State(uint8_t pumpPin, PinValue value)
{
    switch(pumpPin)
    {
        case PUMP_1:
            RD5 = value;
            break;
        case PUMP_2:
            RC8 = value;
            break;
        case PUMP_3:
            RC6 = value;
            break;
        case PUMP_4:
            RB9 = value;
            break;
        case PUMP_5:
            RD6 = value;
            break;
        case PUMP_6:
            RC7 = value;
            break;
    }
}

void SingleSwitch_State(uint8_t switchPin, PinValue value)
{
    switch(switchPin)
    {
        case SWITCH_1:
            RG6 = value;
            break;
        case SWITCH_2:
            RB14 = value;
            break;
        case SWITCH_3:
            RA12 = value;
            break;
        case SWITCH_4:
            RG9 = value;
            break;
        case SWITCH_5:
            RA0 = value;
            break;
        case SWITCH_6:
            RA11 = value;
            break;
    }
}

void VacuumPump_UpdateStates()
{
    for(int i = 0; i < count; i++)
    {
        if ((sys_time - instances[i].delay_counter >= VACUUM_SWITCH_RELASE_TIME)
                && instances[i].state == VACUUM_PUMP_STATE_RELEASING)
        {
            SingleSwitch_State(instances[i].switchPin, PIN_LOW);
            instances[i].state = VACUUM_PUMP_STATE_OFF;
        }
    }
}