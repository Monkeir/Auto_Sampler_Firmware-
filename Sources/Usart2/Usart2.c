//==============================================================================
#include <string.h>
#include "xPort.h"
#include "xTransaction.h"
#include "main.h"
#include "usart.h"
#include "CK_Ports.h"
#include "CK_Responses.h"
#include "Usart2.h"
#include "xTx.h"
//==============================================================================
#define USART2_RX_CIRCLE_BUF_SIZE_MASK 0x1ff
#define USART2_RX_OBJECT_BUF_SIZE 0x3ff

#define USART2_TX_CIRCLE_BUF_SIZE_MASK 0x3ff
//==============================================================================
TX_BUF_INIT(USART2);
RX_BUF_INIT(USART2);
//==============================================================================
//==============================================================================
inline void Usart2_RxTxHandler()
{
  xRxUpdate(&Usart2, &Usart2.Rx);
  
  if(Usart2.Reg->SR.ErrorOverRun)
  {
    Usart2.Reg->SR;
    Usart2.Reg->SR.ErrorOverRun = false;
  }
  
  //if(Ports.B.Out->USART1_DE && !Usart2.Reg->CR1.TxCompleteInterruptEnable && !Usart2.Tx.State.TxDisableDelay){ Ports.B.Out->USART1_DE = false; }
  //if(!Usart2.Reg->CR1.TxCompleteInterruptEnable && Usart2.Reg->SR.TxComplete && Ports.B.Out->USART1_DE){ Ports.B.Out->USART1_DE = false; return; }
}
//==============================================================================
inline void Usart2_Init()
{
  Usart2.Tx.Rx = &Usart2.Rx;
  Usart2.Rx.Tx = &Usart2.Tx;

  Usart2.Reg->CR1.ReceiverEnable = true;
  Usart2.Reg->CR1.RxNotEmptyInterruptEnable = true;

  Ports.B.Out->USART1_DE = false;
  Usart2.Reg->CR1.TransmitterEnable = true;
  Usart2.Reg->CR1.TxCompleteInterruptEnable = false;
  Usart2.Reg->CR1.TxEmptyInterruptEnable = false;
}
//==============================================================================
bool usart2_transmit_action(xObject context, uint8_t* ptr, uint16_t size)
{
  Ports.B.Out->USART1_DE = true;
  Usart2.Reg->CR1.TxEmptyInterruptEnable = true;
  return true;
}
//==============================================================================
Usart2T Usart2 =
{
  .Description = "Usart2T",
  
  .Rx =
  {
    .Description = "xRxT",
    .Attachment = &Usart2,
    
    RX_OBJECT_RECEIVER_INIT(USART2, rx_endline),
    RX_CIRCLE_RECEIVER_INIT(USART2)
  },

  .Tx =
  {
    .Description = "Tx",
    .Attachment = &Usart2,
    
    TX_BINDING(USART2, usart2_transmit_action),
  },

  .Reg = (UsartRegT*)USART2
};
//==============================================================================
