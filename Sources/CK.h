/*
 * CK.h
 *
 *  Created on: Dec 17, 2020
 *      Author: rekuts
 */

#ifndef XSOURCES_CK_H_
#define XSOURCES_CK_H_
//=================================================================================================================================
#include "xType.h"
#include "xPort.h"
#include "xTransaction.h"
//=================================================================================================================================
enum ACTION_SET_HANDLER {
  ACTION_RESET_HANDLER,
  ACTION_SET_HANDLER  
};
//=================================================================================================================================
enum ACTION_TRY_MOVE_TO {
  ACTION_MOVE_DOWN,
  ACTION_MOVE_UPP
};
//=================================================================================================================================
enum ACTION_TRY_CLEAR {
  ACTION_CLEAR_MOTION
};
//=================================================================================================================================
#define DRV_ENABLE 0
#define DRV_DISABLE (!DRV_ENABLE)

#define DRV_RESET_DISABLE 1
#define DRV_RESET_ENABLE (!DRV_RESET_DISABLE)

#define DRV_DIRECTION_UPP 1
#define DRV_DIRECTION_DOWN (!DRV_DIRECTION_UPP)

#define DRV_DENSOR_UPP_ENABLE 0
#define DRV_DENSOR_UPP_DISABLE (!DRV_DENSOR_UPP_ENABLE)

#define DRV_MODE 3
#define DRV_DIVIDER (1 << DRV_MODE)

#define DRV_DENSOR_DOWN_ENABLE 0
#define DRV_DENSOR_DOWN_DISABLE (!DRV_DENSOR_DOWN_ENABLE)
   
#define CK_MAX_SAMPLING_PERIOD 10000L
#define CK_MIN_SAMPLING_PERIOD 1

#define CK_MAX_PERIOD_PWM 3200000L
#define CK_MIN_PERIOD_PWM 0.01

#define CK_STEP_PHASE_COUNT 1

#define CK_MOTION_VECTOR_TABLE_SIZE_MASK (0x1fff)
#define CK_MOTION_VECTOR_TABLE_SIZE (CK_MOTION_VECTOR_TABLE_SIZE_MASK + 1)

static char Firmware[] = "FW:1.0.0";

typedef struct { uint32_t Position; float Period; } CK_MotionT;

typedef struct{
  CK_MotionT Values[CK_MOTION_VECTOR_TABLE_SIZE];
  uint32_t TotalIndex;
  uint32_t HandlerIndex;
  uint32_t SizeMask;
}CK_MotionsT;

static CK_MotionsT Motions = { .SizeMask = CK_MOTION_VECTOR_TABLE_SIZE_MASK };
//static CK_MotionT CK_MOTION_VECTOR_TABLE[CK_MOTION_VECTOR_TABLE_SIZE] = { 0 };
//=================================================================================================================================
#define MASK_REQUEST_SET_HANDLER ((1 << 5) | (1 << 6))
//=================================================================================================================================
typedef union{
  struct{
    volatile uint32_t MotorIsEnable : 1;
    volatile uint32_t MotorEnabling : 1;
    volatile uint32_t MotorDisabling : 1;    
    volatile uint32_t PositionIsSet : 1;
    
    volatile uint32_t UpdatePeriod : 1;    
    volatile uint32_t MotionVector : 1;    
    volatile uint32_t RequestScanStepCount : 1;
    volatile uint32_t ScanStepCountIsEnable : 1;
    
    volatile uint32_t SensorUppIsFree : 1;
    volatile uint32_t SensorUppAccept : 1;
    volatile uint32_t SensorDownAccept : 1;    
    volatile uint32_t StepResolution : 1;
    
    volatile uint32_t UpdateState : 1;
    volatile uint32_t UpdateReducedHeight : 1;    
    volatile uint32_t MoveUpp : 1;
    volatile uint32_t MoveDown : 1;
    
    volatile uint32_t Stopping : 1;
    volatile uint32_t RequestStopping : 1;
    volatile uint32_t CalculateMotion : 1;
  };
  volatile uint32_t Value;
}CK_HandlerT;
//=================================================================================================================================
typedef union{
  struct{
    volatile uint32_t Stopped  : 1;
    volatile uint32_t PositionIsSet  : 1;
  };
  volatile uint32_t Value;
}CK_EventsT;
//=================================================================================================================================
typedef union{
  struct{    
    uint32_t SensorUpp : 1;
    uint32_t SensorDown : 1;
    
    uint32_t Enable : 1;
    uint32_t Dir : 1;
    uint32_t Divider : 3;
    uint32_t Reset : 1;
    
    uint32_t StepState : 1;
  };
  uint32_t Value;
}CK_DRVStateT;
//=================================================================================================================================
typedef struct{
  float RacingK;
  float Increment;
  float StartPeriod;
  
  float FallingK;
  float Decrement;
  float StopPeriod;
  
  uint16_t Divider;
  uint16_t EnableDelay;
  uint16_t DisableDelay;
  uint16_t Amperage;
}CK_OptionsT;
//=================================================================================================================================
typedef struct{
  float Period;
  float Increment;
  float K0;
  float Offset;
  uint32_t Position;
}CK_RacingRequestT;
//=================================================================================================================================
typedef struct{
  float Period;
  float Decrement;
  float K0;
  float Offset;
  uint32_t Position;
}CK_FallingRequestT;
//=================================================================================================================================
typedef struct{
  CK_HandlerT Handler;
  CK_DRVStateT DRV;
  
  float PeriodTotal;
  float PeriodRequest;
  
  int PositionTotal;
  int PositionRequest;
  
  int Difference;
  
  uint32_t MotionTotalIndex;
  uint32_t MotionHandlerIndex;
  uint32_t MotionPointsCount;
  
  float ReducedHeight;
  
  uint32_t MoveTime;
  uint32_t StepsPassed;
  uint32_t Pulses;
}CK_MotorStateT;
//=================================================================================================================================
typedef struct{
  uint16_t EnableDelay;
  uint16_t DisableDelay;
}CK_DelayT;
//=================================================================================================================================
typedef struct{
  float FallingPeriod;
  
  float Increment;
  float Decrement;
  
  float PeriodRequest;
  float PeriodTotal;
  
  uint32_t RacingSteps;
  uint32_t FallingSteps;
}CK_ControlT;
//=================================================================================================================================
typedef struct{
  uint32_t StepsCount;
  float StepSize;
  
  uint32_t PWM_Frequency;
  uint32_t PWM_Prescaler;
  float PWM_Const;  
  float Height;  
  uint32_t MotionPointsCountMax;  
}CK_InfoT;
//=================================================================================================================================
typedef union{
  struct{    
    uint32_t StartMotionVector : 1;
  };
  uint32_t Value;
}GC_RequestsT;
//=================================================================================================================================
typedef enum
{
  RequestSetPinActionResetPins,
  RequestSetPinActionSetPins,
    
}  RequestSetPinAction;
//=================================================================================================================================
typedef CK_OptionsT RequestSetOptionsT;
typedef struct { CK_HandlerT Value; uint32_t Action; } RequestSetHandlerT;
typedef struct { uint32_t StartIndex; uint32_t PointsCount; uint32_t Action; uint32_t Points[32]; } RequestSetMotionStepsT;
typedef struct { float Value; } RequestSetHeightT;
typedef struct { uint32_t Value; } RequestSetMotionPositionT;
typedef struct { uint32_t Action; } RequestTryClearT;
typedef struct { CK_OptionsT Options; int Position; float Period; uint32_t Action; } RequestSetPositionT;

typedef struct
{
  union
  {
    struct
    {
      uint8_t Pin1 : 1;
      uint8_t Pin2 : 1;
      uint8_t Pin3 : 1;
      uint8_t Pin4 : 1;
    };
    uint8_t Value;
  };
  
  uint8_t Action;
  
} RequestSetPinsT;

typedef struct {
  float FallingK;
  float Decrement;
  
  float StartPeriod;
  float StopPeriod;
  
  uint16_t Divider;
  uint16_t DisableDelay;
}RequestTryStopT;

typedef GC_RequestsT RequestSetRequestsT;
//=================================================================================================================================
typedef struct{
  OBJECT_ATTACHMENT(xObject);
  //CK_HandlerT Handler;
  CK_EventsT Events;
  //CK_DRVStateT DRV;
  GC_RequestsT Requests;
  CK_OptionsT Options;
  CK_InfoT Info;
  uint8_t PinsOutState;
  
  CK_MotorStateT MotorState;
  CK_ControlT Control;

  CK_DelayT Delay;
}CK_T;
//=================================================================================================================================
typedef union{
  struct{    
    uint8_t Operation : 1;
    uint8_t Addres : 3;
    uint8_t DeviceCode : 4;
  };
  uint8_t Value;
}MCP4725DeviceAddresingT;
//=================================================================================================================================
typedef union{
  struct{    
    uint8_t Unused_0 : 1;
    uint8_t PowerDownSelection : 2;
    uint8_t Unused_3 : 1;
    uint8_t Unused_4 : 1;
    uint8_t Command : 3;
  };
  uint8_t Value;
}MCP4725ControlT;
//=================================================================================================================================
extern CK_T CK;
//=================================================================================================================================
uint8_t CK_SetHandler(xEventBaseT* event, RequestSetHandlerT *request);
uint8_t CK_SetRequests(xEventBaseT* event, RequestSetRequestsT *request);
uint8_t CK_SetMotionSteps(xEventBaseT* event, RequestSetMotionStepsT *request);
uint8_t CK_SetOptions(xEventBaseT* event, RequestSetOptionsT *request);
uint8_t CK_SetMotionPosition(xEventBaseT* event, RequestSetMotionPositionT *request);
uint8_t CK_SetHeight(xEventBaseT* eventt, RequestSetHeightT *request);
uint8_t CK_SetPosition(xEventBaseT* event, RequestSetPositionT *request);
uint8_t CK_SetPins(xEventBaseT* event, RequestSetPinsT *request);

uint8_t CK_TryClear(xEventBaseT* event, RequestTryClearT *request);
uint8_t CK_TryStop(xEventBaseT* event, RequestTryStopT *request);
uint8_t CK_TryResetSteps(xEventBaseT* event);
uint8_t CK_TryMotionStart(xEventBaseT* event);

extern inline void CK_Init();
extern inline void CK_HandlerPWM();
extern inline void CK_Thread();

uint8_t CK_ApplyPeriodPWM(float period);
uint8_t CK_SetMotorCurrent(uint16_t amp);

uint32_t CK_MotionVectorTableAdd(uint32_t *arg, uint32_t start_index, uint32_t point_count);
//=================================================================================================================================
#endif /* XSOURCES_CK_H_ */
