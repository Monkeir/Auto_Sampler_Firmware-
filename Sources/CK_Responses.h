/*
 * CK_Responses.h
 *
 *  Created on: 28.09.2021
 *      Author: rekuts
 */

#ifndef _CK_RESPONSES_H_
#define _CK_RESPONSES_H_
//==============================================================================
#include "xType.h"
#include "xTransaction.h"
//==============================================================================
enum REQUEST_CK
{
  //============================
  CK_SYSTEM = 17000,
    
  CK_GET_BY_ID = 17200,
  CK_GET_DRV_STATE,
  CK_GET_HANDLER,
  CK_GET_OPTIONS,
  CK_GET_MOTOR_STATE,
  CK_GET_INFO,  
  CK_GET_CONTROL,
  CK_GET_FIRMWARE,
  CK_GET_PINS_OUT_STATE,
  
  CK_SET_HANDLER = 17400,
  CK_SET_OPTIONS,
  CK_SET_POSITION,
  CK_SET_REQUESTS,
  CK_SET_MOTION_STEPS,
  CK_SET_PINS_OUT,
  
  CK_TRY = 17600,
  CK_TRY_CLEAR,
  CK_TRY_MOTION_START,
  CK_TRY_STOP,
  CK_TRY_RESET_STEPS,
  
  CK_EVT_POSITION_SET = 17800,
  CK_EVT_STOPPED,
  
  CK_CONFIRMATION = 17900
  //============================
};
//==============================================================================
enum ERRORS_CK
{
  ERROR_POSITION = ERROR_ACTION + 1
};
//==============================================================================
#define CK_REQUEST_HEADER "#REQ::"
#define CK_RESPONSE_HEADER "#RES::"

#define CK_RESPONSE_END "\r\n"

extern const xCommandT CK_Commands[];
extern const xTransactionT CK_Requests[];
//==============================================================================
int rx_endline(xObject context, uint8_t *obj, uint16_t size);
//==============================================================================
#endif /* _CK_RESPONSES_H_ */
