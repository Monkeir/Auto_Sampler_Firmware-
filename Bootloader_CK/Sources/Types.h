/*
 * Types.h
 *
 * Created: 16.05.2019 14:29:38
 *  Author: rekuts
 */ 
//==============================================================================
#ifndef TYPES_H_
#define TYPES_H_
//==============================================================================
#include "xType.h"
#include "xMacroces.h"
#include "xRx.h"
#include "xTx.h"
#include "CK_Ports.h"
//==============================================================================
typedef struct
{
  OBJECT_DESCRIPTION;
  
  xRxT Rx;
  xTxT Tx;
  
  volatile UsartRegT *Reg;
}UsartT;
//==============================================================================
#endif /* TYPES_H_ */
