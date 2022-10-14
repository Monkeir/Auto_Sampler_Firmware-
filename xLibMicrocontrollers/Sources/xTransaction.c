//==============================================================================
#include <string.h>
#include "xTransaction.h"
//==============================================================================
xCommandT* xCommandIdentify(xObject context, xCommandT commands[], uint8_t data[], uint16_t len)
{
  uint8_t i = 0;
  while(commands[i].Header)
  {
    if(len >= commands[i].HeaderLength)
    {
      if(memcmp(data, commands[i].Header, commands[i].HeaderLength) == 0)
      {
        ((xEventBaseT*)context)->Context = &commands[i];
        switch(commands[i].Mode)
        {
          case TRANSACTION_MODE_OBJECT:
          data += commands[i].HeaderLength;
          len -= commands[i].HeaderLength;
          break;
        }		
        if (commands[i].Request) { commands[i].Request(context, data, len); }
        return &commands[i];
      }
    }
    i++;
  }
  return 0;
}
//==============================================================================
xTransactionT* xTransactionIdentify(xObject context, xTransactionT* transaction, uint16_t key)
{
  while(transaction && transaction->Id != (uint16_t)-1)
  {
    if(transaction->Id == key)
    {
      return transaction;
    }
    transaction++;
  }
  
  return 0;
}
//==============================================================================
