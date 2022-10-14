//==============================================================================
#ifndef X_TX_H
#define X_TX_H
//==============================================================================
#include <stdint.h>
#include <stdbool.h>
#include "xType.h"
//==============================================================================
typedef struct { uint8_t* ptr; uint16_t size; } xPacketT;
//==============================================================================
typedef bool (*TransmitActionT)(xObject context, uint8_t* ptr, uint16_t size);
//==============================================================================
#define xTX_SET_MASK_SIZE (bits)(~(0xffff << bits))
//==============================================================================
//==============================================================================
typedef struct
{
    volatile uint16_t TotalIndex;
    uint16_t HandlerIndex;
    uint16_t UpdateDelay;
    volatile uint16_t TxDisableDelay;
    uint16_t SizeMask;
}xTxStateT;
//==============================================================================
typedef union
{
  struct
  {
    uint16_t Transmitted : 1;
    uint16_t IsAdd : 1;
    uint16_t IsUpdate : 1;
    uint16_t TransferComplite : 1;
    
    uint16_t TxBufFull : 1;
    uint16_t Lock : 1;
    uint16_t CrcEnable : 1;
  };
  uint16_t Value;
}xTxHandlerT;
//==============================================================================
typedef struct
{
  OBJECT_ATTACHMENT(xObject);
  
  uint8_t* Buffer;
  volatile xTxHandlerT Handler;
  xTxStateT State;
  uint16_t Crc;
  TransmitActionT TransmitAction;
  
  xObject Rx;
}xTxT;
//==============================================================================
extern void xTxAdd(xTxT *Tx, xObject obj, uint16_t obj_size);
extern inline void xTxPutByte(xTxT *Tx, uint8_t byte);
extern inline void xTxAddStr(xTxT *Tx, char* str);

xPacketT xTxGetPacket(xTxT *Tx);
//==============================================================================
#define xTxSend(xTx, reg)\
  reg = xTx.Buffer[xTx.State.HandlerIndex];\
  xTx.State.HandlerIndex++;\
  xTx.State.HandlerIndex &= xTx.State.SizeMask
//==============================================================================
#define TX_BUF_INIT(name)\
uint8_t name##_TX_CIRCLE_BUF[name##_TX_CIRCLE_BUF_SIZE_MASK + 1]

#define TX_BINDING(name, transmit_action)\
.State = { .SizeMask = name##_TX_CIRCLE_BUF_SIZE_MASK },\
.Buffer = name##_TX_CIRCLE_BUF,\
.TransmitAction = transmit_action
//==============================================================================
#define TX_INIT(name, size_mask_circle_buf, transmit_action)\
uint8_t name##_TX_CIRCLE_BUF[size_mask_circle_buf + 1];\
xTxT name##_TX =\
{\
  .Description = "xTxT",\
  .State = { .SizeMask = size_mask_circle_buf },\
  .Buffer = name##_TX_CIRCLE_BUF,\
  .TransmitAction = transmit_action\
}
//==============================================================================
#endif
