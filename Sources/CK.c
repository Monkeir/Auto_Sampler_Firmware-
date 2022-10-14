/*
 * CK.c
 *
 *  Created on: Dec 17, 2020
 *      Author: rekuts
 */
//==============================================================================
#include <string.h>
#include <stdlib.h>
#include "xTimer.h"
#include "Usart2.h"
#include "xTx.h"
#include "CK.h"
#include "CK_Responses.h"
#include "CK_Ports.h"
#include "i2c.h"
//==============================================================================
extern volatile I2C_RegT *I2C_Reg;
uint32_t loger_steps = 0;
//==============================================================================
uint32_t r_count = 0;
uint8_t error;
//==============================================================================
uint32_t CK_GetRacingSteps(CK_RacingRequestT request, float period_total){
  uint32_t steps = 0;  
  while(request.Period > period_total){
    period_total += request.Increment;
    request.Increment *= request.K0;
    steps++;
  }  
  return steps;
}

uint32_t CK_GetFallingSteps(CK_FallingRequestT request, float period_total){
  uint32_t steps = 0;  
  while(request.Period < period_total){
    period_total -= request.Decrement;
    request.Decrement *= request.K0;
    steps++;
  }  
  return steps;
}
//==============================================================================
uint8_t CK_SetPosition(xEventBaseT* event, RequestSetPositionT *request){
  RequestSetPositionT request_set_position;
  
  memcpy(&request_set_position, (uint8_t*)request, sizeof(RequestSetPositionT));
  
  if(!request){ return ERROR_REQUEST; }
  //if(CK.MotorState.Handler.MotionVector || Ports.C.Out->DrvEnable == DRV_ENABLE){ return BUSY; }
  if(!CK.MotorState.Handler.PositionIsSet){ return BUSY; }
  if(request_set_position.Period < 0.1 || request_set_position.Position == CK.MotorState.PositionRequest){ return ERROR_RESOLUTION; }
  if(Ports.C.In->SensorDown == DRV_DENSOR_DOWN_ENABLE && request->Position < CK.MotorState.PositionRequest){ return ERROR_RESOLUTION; }
  if(Ports.C.In->SensorUp == DRV_DENSOR_UPP_ENABLE && request->Position > CK.MotorState.PositionRequest){ return ERROR_RESOLUTION; }
  
  if(request_set_position.Options.Amperage != 0){ CK_SetMotorCurrent(request_set_position.Options.Amperage); }
  
  if(request_set_position.Options.Increment != 0){ CK.Options.Increment = request_set_position.Options.Increment; }
  if(request_set_position.Options.Decrement != 0){ CK.Options.Decrement = request_set_position.Options.Decrement; }
  
  if(request_set_position.Options.RacingK != 0){ CK.Options.RacingK = request_set_position.Options.RacingK; }
  if(request_set_position.Options.FallingK != 0){ CK.Options.FallingK = request_set_position.Options.FallingK; }
  
  if(request_set_position.Options.StartPeriod != 0){ CK.Options.StartPeriod = request_set_position.Options.StartPeriod; }
  if(request_set_position.Options.StopPeriod != 0){ CK.Options.StopPeriod = request_set_position.Options.StopPeriod; }
  
  if(request_set_position.Options.DisableDelay != 0){ CK.Options.DisableDelay = request_set_position.Options.DisableDelay; }
  if(request_set_position.Options.EnableDelay != 0){ CK.Options.EnableDelay = request_set_position.Options.EnableDelay; }
  
  Ports.C.Out->DrvDivider = (request_set_position.Options.Divider & 0x07);
  CK.Options.Divider = Ports.C.Out->DrvDivider;
  
  CK.Control.Increment = CK.Options.Increment;
  CK.Control.Decrement = CK.Options.Decrement;
  
  CK.Control.RacingSteps = 0;
  CK.Control.FallingSteps = 0;
  
  CK.Control.PeriodRequest = request_set_position.Period;
  CK.Control.PeriodTotal = CK.Options.StartPeriod;
  CK.Control.FallingPeriod = CK.Options.StopPeriod;
  
  CK.MotorState.PositionRequest = request_set_position.Position;
  CK.MotorState.PeriodRequest = request_set_position.Period;
  
  CK.Events.PositionIsSet = true;
  CK.MotorState.MoveTime = 0;
  CK.MotorState.Handler.PositionIsSet = false;
  xTimer2->CR1.CounterEnable = true;
  
  return ACCEPT;
}
//==============================================================================
inline void CK_MotorStep(){
  xTimer3->CR1.CounterEnable = false;
  
  CK.MotorState.StepsPassed++;  
  Ports.C.Out->DrvEnable = DRV_ENABLE;
  CK.Delay.DisableDelay = CK.Options.DisableDelay;
  
  xTimer3->Counter = 0;
  xTimer3->CR1.CounterEnable = true;
}
//==============================================================================
inline uint8_t CK_ApplyPeriodPWM(float request){
  bool state = xTimer2->CR1.CounterEnable;
  uint32_t period = (uint32_t)(CK.Info.PWM_Const/request - 1);
  
  xTimer2->CR1.CounterEnable = false;
  xTimer2->Period = period;
  if(xTimer2->Counter >= period){ xTimer2->Counter = period - 1; }
  xTimer2->CR1.CounterEnable = state;
  return ACCEPT;
}
//==============================================================================
inline void CK_HandlerPWM(){
  int step_difference;
  CK.MotorState.Handler.UpdateState = true;
  
  xTimer5->Counter = 0;
  xTimer5->CR1.CounterEnable = true;
  
  CK.MotorState.Difference = CK.MotorState.PositionTotal - CK.MotorState.PositionRequest;
  if(CK.MotorState.Difference > 0){ Ports.C.Out->DrvDir = DRV_DIRECTION_DOWN; CK.MotorState.PositionTotal--; }
  else if(CK.MotorState.Difference < 0){ Ports.C.Out->DrvDir = DRV_DIRECTION_UPP; CK.MotorState.PositionTotal++; }
  else { xTimer2->CR1.CounterEnable = false; goto end; }
  
  CK_ApplyPeriodPWM(CK.Control.PeriodTotal);
  CK_MotorStep();
  
  step_difference = abs(CK.MotorState.Difference) - CK.Control.FallingSteps;
  
  if(step_difference <= 0){
    CK.Control.FallingPeriod -= CK.Control.Decrement;
    CK.Control.PeriodTotal = CK.Control.FallingPeriod;
    if(CK.Control.FallingPeriod < CK.Options.StopPeriod){ CK.Control.FallingPeriod = CK.Options.StopPeriod; }
  }
  else{
    while(CK.Control.FallingPeriod < CK.Control.PeriodTotal && CK.Control.Decrement > 0){
      CK.Control.FallingPeriod += CK.Control.Decrement;
      CK.Control.FallingSteps++;
    }
    
    if(CK.Control.PeriodTotal < CK.Control.PeriodRequest){
      CK.Control.PeriodTotal += CK.Control.Increment;
      if(CK.Control.PeriodTotal > CK.Control.PeriodRequest){ CK.Control.PeriodTotal = CK.Control.PeriodRequest; }
      CK.Control.RacingSteps++;
    }
  }
  
  end:;
  xTimer5->CR1.CounterEnable = false;
  if(loger_steps < xTimer5->Counter){ loger_steps = xTimer5->Counter; }
}
//==============================================================================
void CK_TransmitEvent(uint16_t id, xObject obj, uint16_t size)
{
  ResponseInfoT info = { .Key = id, .Size = size };
  
  xTxAdd(&Usart2.Tx, CK_RESPONSE_HEADER, sizeof_str(CK_RESPONSE_HEADER));
  xTxAdd(&Usart2.Tx, &info, sizeof(info));
  xTxAdd(&Usart2.Tx, obj, size);
  xTxAdd(&Usart2.Tx, CK_RESPONSE_END, sizeof_str(CK_RESPONSE_END));
}
//==============================================================================
inline void CK_EventsThread(){
  if(CK.Events.PositionIsSet && CK.MotorState.Handler.PositionIsSet){ CK.Events.PositionIsSet = false;
    CK_TransmitEvent(CK_EVT_POSITION_SET, &CK.MotorState, sizeof(CK.MotorState));
  }
  
  if(CK.Events.Stopped && CK.MotorState.Handler.PositionIsSet){ CK.Events.Stopped = false;
    CK_TransmitEvent(CK_EVT_STOPPED, &CK.MotorState, sizeof(CK.MotorState));
  }
}
//==============================================================================
inline void CK_DRVStateUpdate(){
  CK.MotorState.DRV.Dir = Ports.C.Out->DrvDir == DRV_DIRECTION_UPP;
  CK.MotorState.DRV.Divider = Ports.C.Out->DrvDivider;
  CK.MotorState.DRV.Enable = Ports.C.Out->DrvEnable == DRV_ENABLE;
  CK.MotorState.DRV.Reset = Ports.C.Out->DrvReset == DRV_RESET_ENABLE;
}
//==============================================================================
inline void CK_Thread(){
  CK_DRVStateUpdate();
  
  CK.MotorState.DRV.SensorDown = Ports.C.In->SensorDown == DRV_DENSOR_DOWN_ENABLE;
  CK.MotorState.DRV.SensorUpp = Ports.C.In->SensorUp == DRV_DENSOR_UPP_ENABLE;
  
  CK.MotorState.Handler.PositionIsSet = CK.MotorState.PositionTotal == CK.MotorState.PositionRequest && !xTimer2->CR1.CounterEnable;
  
  if(CK.MotorState.DRV.SensorDown && CK.MotorState.PositionRequest < CK.MotorState.PositionTotal){ CK.MotorState.PositionRequest = CK.MotorState.PositionTotal; }
  if(CK.MotorState.DRV.SensorUpp && CK.MotorState.PositionRequest > CK.MotorState.PositionTotal){ CK.MotorState.PositionRequest = CK.MotorState.PositionTotal; }
  
  CK_EventsThread();
}
//==============================================================================
uint8_t CK_TryStop(xEventBaseT* event, RequestTryStopT *request){
  RequestTryStopT options;
  if(CK.MotorState.Handler.ScanStepCountIsEnable){ return BUSY; }
  if(CK.MotorState.Handler.PositionIsSet){ return ACCEPT; }
  
  memcpy(&options, (uint8_t*)request, sizeof(RequestTryStopT));
  
  xTimer2->CR1.CounterEnable = false;
  
  float decrement = CK.Control.Decrement;  
  float period_start = CK.Control.PeriodTotal;
  float period_stop = CK.Options.StopPeriod;
  float period = CK.Control.PeriodTotal;
  uint32_t steps = 0;
  
  if(options.Decrement){ decrement = options.Decrement; }
  if(options.StartPeriod){ period_start = options.StartPeriod; period = period_start; }
  if(options.StopPeriod){ period_stop = options.StopPeriod; }  
  
  while(period > period_stop){ period -= decrement; steps++; }
  
  int difference = CK.MotorState.PositionTotal - CK.MotorState.PositionRequest;
  if(difference < 0){ CK.MotorState.PositionRequest = CK.MotorState.PositionTotal + steps; }
  else if(difference > 0) { CK.MotorState.PositionRequest = CK.MotorState.PositionTotal - steps; }
  
  CK.Control.FallingSteps = steps;
  CK.Control.FallingPeriod = period_start;
  CK.Control.Decrement = decrement;
  
  CK.Control.RacingSteps = 0;
  
  CK_ApplyPeriodPWM(period_start);
  
  CK.Events.Stopped = true;
  CK.MotorState.Handler.Stopping = true;
  xTimer2->CR1.CounterEnable = true;
  return ACCEPT;
}
//==============================================================================
uint8_t CK_TryResetSteps(xEventBaseT* event){
  if(!CK.MotorState.Handler.PositionIsSet){ return BUSY; }  
  CK.MotorState.PositionTotal = 0;
  CK.MotorState.PositionRequest = 0;
  CK.MotorState.StepsPassed = 0;
  CK.MotorState.Pulses = 0;
  CK.MotorState.MoveTime = 0;
  return ACCEPT;
}
//==============================================================================
//==============================================================================
uint8_t CK_SetHandler(xEventBaseT* event, RequestSetHandlerT *request){
  if(!request){ return ERROR_REQUEST; }
  
  switch(request->Action){
    case ACTION_RESET_HANDLER: CK.MotorState.Handler.Value &= ~(request->Value.Value & MASK_REQUEST_SET_HANDLER); break;
    case ACTION_SET_HANDLER: CK.MotorState.Handler.Value |= (request->Value.Value & MASK_REQUEST_SET_HANDLER); break;
    default: return ERROR_REQUEST;
  }
  return ACCEPT;
}
//==============================================================================
uint8_t CK_SetPins(xEventBaseT* event, RequestSetPinsT *request){
  
  if (request->Pin1)
  {
    switch (request->Action)
    {
      case RequestSetPinActionSetPins: EXT_0_GPIO_Port->ODR |= EXT_0_Pin; break;
      case RequestSetPinActionResetPins: EXT_0_GPIO_Port->ODR &= ~EXT_0_Pin; break;
    }
  }
  
  return ACCEPT;
}
//==============================================================================
uint8_t CK_SetRequests(xEventBaseT* event, RequestSetRequestsT *request){
  if(!request){ return ERROR_REQUEST; }
  CK.Requests.Value |= request->Value;
  return ACCEPT;
}
//==============================================================================
uint8_t CK_SetOptions(xEventBaseT* event, RequestSetOptionsT *request){
  if(!request){ return ERROR_REQUEST; }
  memcpy(&CK.Options, (uint8_t*)request, sizeof(CK.Options));
  return ACCEPT;
}
//==============================================================================
uint8_t CK_SetMotionSteps(xEventBaseT* event, RequestSetMotionStepsT *request){
  if(!request){ return ERROR_REQUEST; }
  return ACCEPT;
}
//==============================================================================
uint8_t CK_SetMotionPosition(xEventBaseT* event, RequestSetMotionPositionT *request){
  if(!request){ return ERROR_REQUEST; }
  if(!CK.MotorState.Handler.PositionIsSet && CK.MotorState.Handler.MotionVector){ return BUSY; }
  
  return ACCEPT;
}
//==============================================================================
uint8_t CK_SetHeight(xEventBaseT* event, RequestSetHeightT *request){
  if(!request){ return ERROR_REQUEST; }
  if(!CK.MotorState.Handler.PositionIsSet && CK.MotorState.Handler.MotionVector){ return BUSY; }
  
  float height = request->Value * 1000;
  
  if(height >= CK.Info.Height){ return OUTSIDE; }
  
  //if(height < 0){ CK.MotorState.PositionTotal = CK.MotorState.PositionRequest - (int)(height / CK.MotorState.StepSize); }
  //else { CK.MotorState.PositionRequest = (uint32_t)(height / CK.MotorState.StepSize); }
  return ACCEPT;
}
//==============================================================================
uint8_t CK_TryClear(xEventBaseT* event, RequestTryClearT *request){
  if(CK.MotorState.Handler.PositionIsSet){ return BUSY; }
  if(!request){ return ERROR_REQUEST; }
  
  switch(request->Action){
    case ACTION_CLEAR_MOTION :
      CK.MotorState.MotionPointsCount = 0;
      CK.MotorState.MotionTotalIndex = 0;
    break;
    
    default : return ERROR_ACTION;
  }
  
  return ACCEPT;
}
//==============================================================================
uint8_t CK_TryMotionStart(xEventBaseT* event){
  if(CK.MotorState.Handler.ScanStepCountIsEnable || !CK.MotorState.Handler.PositionIsSet || CK.MotorState.Handler.MotionVector){ return BUSY; }  
  if(!CK.MotorState.MotionTotalIndex && Ports.C.In->SensorDown == DRV_DENSOR_DOWN_DISABLE) { return ERROR_RESOLUTION; }
  if(!CK.MotorState.MotionPointsCount || (CK.MotorState.MotionTotalIndex + 1 >= CK.MotorState.MotionPointsCount)) { return ERROR_RESOLUTION; }        
  return ACCEPT;
}
//==============================================================================
inline void CK_Init(){
  CK.Info.PWM_Prescaler = xTimer2->Prescaler + 1;
  Ports.C.Out->DrvEnable = DRV_DISABLE;
  Ports.C.Out->DrvDivider = 0x07;
  Ports.C.Out->DrvReset = DRV_RESET_DISABLE;
  
  CK.MotorState.Handler.StepResolution = true;
  //CK.Handler.RequestScanStepCount = true;
}
//==============================================================================
MCP4725DeviceAddresingT MCP4725DeviceAddresing;
MCP4725ControlT MCP4725Control;
//==============================================================================
uint8_t CK_SetMotorCurrent(uint16_t amp){  
  MCP4725DeviceAddresing.Addres = 0;
  MCP4725DeviceAddresing.DeviceCode = 0x0c;
  MCP4725DeviceAddresing.Operation = 0;  
  MCP4725Control.Command = 0x02;
  
  //if(amp == CK.Options.Amperage){ return ACCEPT; }
  //error = HAL_I2C_Master_Transmit(&hi2c1, MCP4725DeviceAddresing.Value, (uint8_t*)&Packet, sizeof(Packet), 1000);

  I2C_Reg->SR1.Value = 0;
  I2C_Reg->SR2.Value = 0;
  
  I2C_Reg->CR1.PeripheralEnable = true;
  I2C_Reg->CR1.StartGeneration = true;
  
  while(!I2C_Reg->SR1.StartBit){}
  I2C_Reg->SR1.Value;
  I2C_Reg->SR2.Value;
  
  I2C_Reg->DR = MCP4725DeviceAddresing.Value;
  while(!I2C_Reg->SR1.AddressSent){}
  I2C_Reg->SR1.Value;
  I2C_Reg->SR2.Value;
  
  I2C_Reg->DR = MCP4725Control.Value;
  while(!I2C_Reg->SR1.TxEmpty){}  

  uint16_t temp = amp & 0xfff;
  temp <<= 4;
  
  I2C_Reg->DR = temp >> 8;
  while(!I2C_Reg->SR1.TxEmpty){}
  
  I2C_Reg->DR = temp & 0xf0;
  while(!I2C_Reg->SR1.TxEmpty){}
  
  I2C_Reg->CR1.StartGeneration = false;
  CK.Options.Amperage = amp;

  return ACCEPT;
}
//==============================================================================
//==============================================================================
CK_T CK = {
  .Description = "CK_T",
    
  .Options = {
    .RacingK = 1.000,
    .Increment = 10,
    .StartPeriod = 100,    
    
    .FallingK = 1.0,
    .Decrement = 10,
    .StopPeriod = 100,
    
    .EnableDelay = 10,
    .DisableDelay = 100,  
  },
  
  .Info = {
    .Height = 52000.0,
    .PWM_Frequency = 84000000,
    .PWM_Const = 84000000/CK_STEP_PHASE_COUNT,
    
    .MotionPointsCountMax = CK_MOTION_VECTOR_TABLE_SIZE
  },
  
  .MotorState = {
    .PeriodRequest = 1000,  // there is was 10000
    .MotionPointsCount = 0x1fff
  }
};
//==============================================================================