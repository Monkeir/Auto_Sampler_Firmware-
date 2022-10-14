//==============================================================================
#ifndef X_RX_RECIVER_H
#define X_RX_RECIVER_H
//==============================================================================
#include <stdint.h>
#include <stdbool.h>
#include "xType.h"
//==============================================================================
#define XRX_CIRCLE_BASE\
  uint8_t *Buffer;\
  uint16_t TotalIndex;\
  uint16_t HandlerIndex;\
  uint16_t SizeMask
//==============================================================================
enum RX_RESULT
{
  RX_STORAGE,
  RX_RESET
};
//==============================================================================
typedef struct { uint16_t size; uint8_t* ptr; } xRxObjectT;
//==============================================================================
typedef int (*xRxEventEndLine)(xObject action, uint8_t* object, uint16_t size);
//==============================================================================
typedef struct
{
  uint16_t Storage : 1;
  uint16_t BufLoaded : 1;
  uint16_t ReceiveComlite : 1;
  uint16_t EndLineControl : 1;
  
  uint16_t CrcEnable : 1;
}xRxStateT;
//==============================================================================
typedef struct
{
  XRX_CIRCLE_BASE;
}xRxCircleBaseT;
//==============================================================================
typedef struct
{
  XRX_CIRCLE_BASE;
  volatile uint16_t ReceiveDelay;
}xRxCircleReceiverT;
//==============================================================================
typedef struct
{
  uint8_t* Object;
  uint16_t Size;
  uint16_t BytesCount;
  xRxEventEndLine EventEndLine;
}xRxObjectReceiverT;
//==============================================================================
typedef struct
{
  OBJECT_ATTACHMENT(xObject);
  
  volatile xRxStateT State;
  xRxCircleReceiverT CircleReceiver;
  xRxObjectReceiverT ObjectReceiver;
  
  xObject Tx;
}xRxT;
//==============================================================================
//void xRxCircleAdd(xRxT* xRX, uint8_t Byte);
void xRxAdd(xObject context, xRxT *rx, uint8_t *data, uint16_t data_size);
void xRxUpdate(xObject context, xRxT* rx);
void xRxClear(xRxT* rx);
//==============================================================================
#define xRxCircleAdd(xRX, byte)\
xRX.CircleReceiver.Buffer[xRX.CircleReceiver.TotalIndex] = byte;\
xRX.CircleReceiver.TotalIndex++;\
xRX.CircleReceiver.TotalIndex &= xRX.CircleReceiver.SizeMask
//==============================================================================
#define RX_BUF_INIT(name)\
uint8_t name##_RX_CIRCLE_BUF[name##_RX_CIRCLE_BUF_SIZE_MASK + 1];\
uint8_t name##_RX_OBJECT_BUF[name##_RX_OBJECT_BUF_SIZE]
//==============================================================================
#define RX_OBJECT_RECEIVER_INIT(name, callback)\
.ObjectReceiver =\
{\
  .EventEndLine = (xRxEventEndLine)callback,\
  .Object = name##_RX_OBJECT_BUF,\
  .Size = name##_RX_OBJECT_BUF_SIZE\
}
//==============================================================================
#define RX_CIRCLE_RECEIVER_INIT(name)\
.CircleReceiver =\
{\
  .Buffer = name##_RX_CIRCLE_BUF,\
  .SizeMask = name##_RX_CIRCLE_BUF_SIZE_MASK\
}
//==============================================================================
#define RX_INIT(name, size_mask_circle_buf, size_object_buf, event_end_line)\
uint8_t name##_RX_CIRCLE_BUF[size_mask_circle_buf + 1];\
uint8_t name##_RX_OBJECT_BUF[size_object_buf];\
xRxT name##_RX =\
{\
  .Description = "xRxT",\
  .CircleReceiver = { .Buffer = name##_RX_CIRCLE_BUF, .SizeMask = size_mask_circle_buf },\
  .ObjectReceiver = { .EventEndLine = event_end_line, .Object = name##_RX_OBJECT_BUF, .Size = size_object_buf }\
}
//==============================================================================
#endif
