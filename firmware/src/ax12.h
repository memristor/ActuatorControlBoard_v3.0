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

#ifndef AX12_H_
#define AX12_H_

#include "can.h"


#define AX12_TIMEOUT 20//15//ms

typedef enum _servoSelect{
    AX12 = 0,
    RX24 = 1
} ServoSelect;


void AX12_Initialize();

int AX12_SpeedInit(uint16_t speed);

int AX12_OnMessage(can_t AX12_CanMsg);

#endif