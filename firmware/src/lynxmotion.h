/* 
 * File:   lynxmotion.h
 * Author: hepek
 *
 * Created on April 5, 2022, 7:54 PM
 */

#ifndef LYNXMOTION_H
#define	LYNXMOTION_H

#include "can.h"
#include "definitions.h"
#include <stdint.h>

#define LYNXMOTION_RX_DELAY 10
#define LYNXMOTION_TIMEOUT 100

uint8_t lynxmotion_OnMessage(can_t lynxmotion_CanMsg);

#endif	/* LYNXMOTION_H */

