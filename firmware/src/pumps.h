/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef VACUUMP_PUMP_H__
#define VACUUMP_PUMP_H__

#include "ax12.h"

#define VACUUM_PUMP_CONFIG_COUNT_MAX 6
#define VACUUM_SWITCH_RELASE_TIME 600

#define VACUUM_PUMP_STATE_ON 1
#define VACUUM_PUMP_STATE_RELEASING 2
#define VACUUM_PUMP_STATE_OFF 3


typedef struct _VacuumPump{
    uint8_t pumpPin;
    uint8_t switchPin;
    uint16_t CanID;
    uint8_t state;
    uint32_t delay_counter;
} VacuumPump;

typedef enum _PinValue {
	PIN_LOW = 0,
	PIN_HIGH = 1
} PinValue;
  
    
void VacuumPump_Add(uint8_t pumpPin, uint8_t switchPin, uint16_t number);

int VacuumPump_OnMessage(can_t Pump_CanMsg);

void SinglePump_State(uint8_t pumpPin, PinValue value);

void SingleSwitch_State(uint8_t switchPin, PinValue value);

void VacuumPump_UpdateStates();

#endif