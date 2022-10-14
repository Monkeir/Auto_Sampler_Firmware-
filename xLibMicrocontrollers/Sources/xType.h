/*
 * xType.h
 *
 * Created: 16.05.2019 14:29:38
 *  Author: rekuts
 */ 
//==============================================================================
#ifndef XTYPE_H_
#define XTYPE_H_
//==============================================================================
#include <stdint.h>
#include <stdbool.h>
//==============================================================================
#ifdef patterns_stm32f4xx

#include "patterns_stm32f4xx/pattern_stm32f4xx_uart.h"
#include "patterns_stm32f4xx/pattern_stm32f4xx_timer.h"
#include "patterns_stm32f4xx/pattern_stm32f4xx_spi.h"
#include "patterns_stm32f4xx/pattern_stm32f4xx_i2c.h"

#endif
//==============================================================================
#if patterns_stm32f1xx

#include "patterns_stm32f1xx/pattern_stm32f1xx_uart.h"
#include "patterns_stm32f1xx/pattern_stm32f1xx_timer.h"
#include "patterns_stm32f1xx/pattern_stm32f1xx_spi.h"
#include "patterns_stm32f1xx/pattern_stm32f1xx_i2c.h"

#endif
//==============================================================================
typedef union{
  struct{
    uint64_t ExponentSign : 1;
    uint64_t Exponent : 11;
    uint64_t Mantisa : 52;
  };
  double Value;
}xDoubleT;
//==============================================================================
enum ERRORS
{
  ACCEPT = 0,
  ERROR_DATA,
  ERROR_CONTENT_SIZE,
  ERROR_REQUEST,
  ERROR_RESOLUTION,
  UNKNOWN_COMMAND,
  BUSY,
  OUTSIDE,
  ERROR_ACTION,
  //ERROR_POSITION
};
//==============================================================================
typedef void* xObject;
//==============================================================================
typedef xObject (*xEvt)(xObject Obj);
typedef struct { xObject obj; uint16_t key; } xContext;
typedef struct { xObject obj; uint16_t size; } xContent;
//==============================================================================
typedef struct{ uint8_t Start; uint8_t Ch1; uint8_t Ch2; uint8_t Ch3; uint8_t Ch4; uint8_t End; } RequestHeaderT;
typedef struct{ uint16_t Key; uint16_t Size; } RequestInfoT;
typedef struct{ RequestHeaderT Header; RequestInfoT Info; } RequestT;

typedef struct{ uint8_t Start; uint8_t Ch1; uint8_t Ch2; uint8_t Ch3; uint8_t Ch4; uint8_t End; } ResponseHeaderT;
typedef struct{ uint16_t Key; uint16_t Size; } ResponseInfoT;
typedef struct{ ResponseHeaderT Header; ResponseInfoT Info; } ResponseT;
	
typedef struct{ uint16_t Action; uint16_t Error; } ErrorResponseT;
//==============================================================================
#define REQUEST_START_CHARACTER '#'
#define REQUEST_END_CHARACTER ':'
#define RESPONSE_START_CHARACTER '#'
#define RESPONSE_END_CHARACTER ':'

#define REQUEST_PREFIX_SIZE sizeof(RequestPrefixT)
static char REQUEST_PREFIX_GET[] = "#GET::";
static char REQUEST_PREFIX_SET[] = "#SET::";
static char REQUEST_PREFIX_TRY[] = "#TRY::";
static char REQUEST_PREFIX_REQ[] = "#REQ::";
#define REQUEST_PACKET_SIZE (sizeof(RequestT))
static char REQUEST_END[] = "\r";

static char REQUEST_PREFIX_BOOT[] = "#BREQ:";
static char RESPONSE_BOOT_HEADER[] = "#BRES:";
static char RESPONSE_BOOT_ACCEPT[] = "#BACC:";

static char RESPONSE_HEADER[] = "#RES::";
static char RESPONSE_ACCEPT[] = "#ACC::";
static char RESPONSE_HEADER_NOTE[] = "#NOTE:";
//static char RESPONSE_END[] = "\r";
//==============================================================================
#define RESPONSE_END_SIZE (sizeof(RESPONSE_END) / sizeof(RESPONSE_END[0]) - 1)
#define SIZE_STRING(str)(sizeof(str) / sizeof(str[0]) - 1)
#define SIZE_ARRAY(array)(sizeof(array) / sizeof(array[0]))

#define sizeof_str(str)(sizeof(str) / sizeof(str[0]) - 1)
#define sizeof_array(array)(sizeof(array) / sizeof(array[0]))
//==============================================================================
#define OBJECT_DESCRIPTION\
  char* Description

#define OBJECT_ATTACHMENT(TAttachment)\
  OBJECT_DESCRIPTION;\
  TAttachment Attachment
    
#define OBJECT_ATTACHMENT_PATTERN(TAttachment)\
  OBJECT_DESCRIPTION;\
  TAttachment
//==============================================================================
#define EVENT_TYPEDEF(Name, TAttachment, TContext)\
typedef struct{\
  OBJECT_ATTACHMENT(TAttachment);\
  TContext Context;\
}xEvent##Name##T

#define EVENT_PATTERN(Name, TAttachment, TContext)\
typedef struct{\
  OBJECT_ATTACHMENT_PATTERN(TAttachment);\
  TContext;\
}xEvent##Name##T
//==============================================================================
#define OBJECT_DEF(Name, TAttachment)\
typedef struct{\
  OBJECT_ATTACHMENT(TAttachment);\
  xObject Context;\
}xObject##Name##T
//==============================================================================
EVENT_TYPEDEF(Base,
              xObject,
              xObject);
//==============================================================================
OBJECT_DEF(Base, xObject);
//==============================================================================
static inline bool IS(xObject total, xObject request, uint16_t size)
{
  while(size)
  {
    size--;
    if(((uint8_t*)total)[size] != ((uint8_t*)request)[size]) { return false; }
  }
  return true;
}
//==============================================================================
#endif /* XTYPE_H_ */
