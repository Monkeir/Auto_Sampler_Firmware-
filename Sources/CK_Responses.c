/*
 * CK_Responses.c
 *
 *  Created on: 28.09.2021
 *      Author: rekuts
 */
//==============================================================================
#include "xTransaction.h"
#include "CK_Responses.h"
#include "CK.h"
#include "xRx.h"
#include "xTx.h"
#include "Bootloader.h"
#include "Usart2.h"
//==============================================================================
EVENT_PATTERN(Response,
               xRxT* Rx,
               xTransactionT* Transaction);
               
EVENT_PATTERN(Command,
               xRxT* Rx,
               xCommandT* Command);
//==============================================================================
void Response_REQUEST_GET(xEventResponseT* event, xObject request_obj, uint16_t request_obj_size, uint8_t error){
  ResponseInfoT info =
  {
    .Key = event->Transaction->Id,
    .Size = event->Transaction->Content.size
  };
  
  xTxAdd(event->Rx->Tx, &info, sizeof(info));  
  xTxAdd(event->Rx->Tx, event->Transaction->Content.obj, event->Transaction->Content.size);
}
//==============================================================================
void Response_REQUEST_SET(xEventResponseT* event, xObject request_obj, uint16_t request_obj_size, uint8_t error){
  ResponseInfoT info =
  {
    .Key = event->Transaction->Id,
    .Size = event->Transaction->Content.size + sizeof(error) + request_obj_size
  };
  
  xTxAdd(event->Rx->Tx, &info, sizeof(info));  
  xTxAdd(event->Rx->Tx, request_obj, request_obj_size);
  xTxAdd(event->Rx->Tx, &error, sizeof(error));
  xTxAdd(event->Rx->Tx, event->Transaction->Content.obj, event->Transaction->Content.size);
}
//==============================================================================
void Response_REQUEST_TRY(xEventResponseT* event, xObject request_obj, uint16_t request_obj_size, uint8_t error){
  ResponseInfoT info =
  {
    .Key = event->Transaction->Id,
    .Size = event->Transaction->Content.size + sizeof(error) + request_obj_size
  };
  
  xTxAdd(event->Rx->Tx, &info, sizeof(info));  
  xTxAdd(event->Rx->Tx, request_obj, request_obj_size);
  xTxAdd(event->Rx->Tx, &error, sizeof(error));
  xTxAdd(event->Rx->Tx, event->Transaction->Content.obj, event->Transaction->Content.size);
}
//==============================================================================
void CK_Request(xEventCommandT* event, uint8_t* object, uint16_t size)
{
  uint16_t action_error = ACCEPT;  
  xTransactionT* transaction = xTransactionIdentify(event, (xTransactionT*)CK_Requests, ((RequestInfoT*)object)->Key);
  
  if(transaction)
  {
    object += sizeof(RequestInfoT);
    size -= sizeof(RequestInfoT);
    
    if(transaction->Action)
    {
      ((xEventBaseT*)event)->Context = transaction;
      action_error = transaction->Action(event, object, size);
    }
    
    if(transaction->Response)
    {
      ((xEventBaseT*)event)->Context = transaction;
      xTxAdd(event->Rx->Tx, CK_RESPONSE_HEADER, sizeof_str(CK_RESPONSE_HEADER));
      transaction->Response(event, object, size, action_error);
      xTxAdd(event->Rx->Tx, CK_RESPONSE_END, sizeof_str(CK_RESPONSE_END));
    }
  }
}
//==============================================================================
int rx_endline(xObject context, uint8_t *obj, uint16_t size)
{ 
  if(size >= sizeof(RequestHeaderT) && obj[0] == REQUEST_START_CHARACTER && obj[5] == RESPONSE_END_CHARACTER)
  {
    RequestT *request = (RequestT*)obj;
    uint16_t action_size = size - sizeof(RequestT);

    if(action_size < request->Info.Size) { return RX_STORAGE; }
    if(action_size > request->Info.Size) { return RX_RESET; }
    
    xCommandIdentify(context, (xCommandT*)CK_Commands, obj, size);    
    return RX_RESET;
  }
  return RX_RESET;
}
//==============================================================================
//==============================================================================
const xCommandT CK_Commands[] =
{
  NEW_COMMAND1(CK_REQUEST_HEADER, CK_Request),
  NEW_COMMAND1(BOOT_REQUEST_HEADER, Bootloader_RequestAction),
};
//==============================================================================
const xTransactionT CK_Requests[] =
{
  NEW_TRANSACTION0(CK_GET_DRV_STATE, Response_REQUEST_GET, 0, CK.MotorState.DRV),
  NEW_TRANSACTION0(CK_GET_MOTOR_STATE, Response_REQUEST_GET, 0, CK.MotorState),
  NEW_TRANSACTION0(CK_GET_HANDLER, Response_REQUEST_GET, 0, CK.MotorState.Handler),
  NEW_TRANSACTION0(CK_GET_INFO, Response_REQUEST_GET, 0, CK.Info),
  NEW_TRANSACTION0(CK_GET_OPTIONS, Response_REQUEST_GET, 0, CK.Options),
  NEW_TRANSACTION0(CK_GET_CONTROL, Response_REQUEST_GET, 0, CK.Control),
  NEW_TRANSACTION0(CK_GET_FIRMWARE, Response_REQUEST_GET, 0, Firmware),
  
  NEW_TRANSACTION0(CK_SET_POSITION, Response_REQUEST_SET, CK_SetPosition, CK.MotorState),
  NEW_TRANSACTION0(CK_SET_HANDLER, Response_REQUEST_SET, CK_SetHandler, CK.MotorState.Handler),
  NEW_TRANSACTION0(CK_SET_MOTION_STEPS, Response_REQUEST_SET, CK_SetMotionSteps, CK.MotorState),
  NEW_TRANSACTION0(CK_SET_OPTIONS, Response_REQUEST_SET, CK_SetOptions, CK.Options),
  NEW_TRANSACTION0(CK_SET_REQUESTS, Response_REQUEST_SET, CK_SetRequests, CK.Requests),
  NEW_TRANSACTION0(CK_SET_PINS_OUT, Response_REQUEST_SET, CK_SetPins, CK.PinsOutState),
  
  NEW_TRANSACTION0(CK_TRY_CLEAR, Response_REQUEST_TRY, CK_TryClear, CK.MotorState),
  NEW_TRANSACTION0(CK_TRY_MOTION_START, Response_REQUEST_TRY, CK_TryMotionStart, CK.MotorState),
  NEW_TRANSACTION0(CK_TRY_STOP, Response_REQUEST_TRY, CK_TryStop, CK.MotorState),
  NEW_TRANSACTION0(CK_TRY_RESET_STEPS, Response_REQUEST_TRY, CK_TryResetSteps, CK.MotorState),
  { .Id = -1 }
};
//==============================================================================
