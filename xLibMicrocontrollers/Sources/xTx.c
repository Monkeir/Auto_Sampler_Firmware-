//=================================================================================================================================
#include <string.h>
#include "xTx.h"
//=================================================================================================================================
void xTxAdd(xTxT *Tx, xObject obj, uint16_t obj_size)
{
  uint8_t *ptr = (uint8_t*)obj;
  while(obj_size)
  {
    Tx->Buffer[Tx->State.TotalIndex] = *ptr++;
    Tx->State.TotalIndex++;
    Tx->State.TotalIndex &= Tx->State.SizeMask;
    obj_size--;
  }
}
//=================================================================================================================================
xPacketT xTxGetPacket(xTxT *Tx)
{
  xPacketT packet;
  packet.size = Tx->State.TotalIndex;
  packet.ptr = Tx->Buffer + Tx->State.HandlerIndex;
  
  if(packet.size < Tx->State.HandlerIndex)
  {
    packet.size = Tx->State.SizeMask - Tx->State.HandlerIndex + 1;
    Tx->State.HandlerIndex = 0;
    return packet;
  }
  
  packet.size -= Tx->State.HandlerIndex;
  Tx->State.HandlerIndex += packet.size;
  return packet;
}
//=================================================================================================================================
inline void xTxPutByte(xTxT *Tx, uint8_t byte)
{
  Tx->Buffer[Tx->State.TotalIndex] = byte;
  Tx->State.TotalIndex++;
  Tx->State.TotalIndex &= Tx->State.SizeMask;
}
//=================================================================================================================================
inline void xTxAddStr(xTxT *Tx, char* str)
{
  xTxAdd(Tx, str, strlen(str));
}
//=================================================================================================================================
