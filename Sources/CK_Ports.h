/*
 * _CK_PORTS_H_.h
 *
 *  Created on: 27.09.2021
 *      Author: rekuts
 */

#ifndef _CK_PORTS_H_
#define _CK_PORTS_H_
//=================================================================================================================================
#include "xType.h"
#include "gpio.h"
//=================================================================================================================================
typedef union{
  struct{
    /*Pin0*/ uint32_t WKUP: 1;
    /*Pin1*/ uint32_t Pin1: 1;
    /*Pin2*/ uint32_t USART2_TX: 1;
    /*Pin3*/ uint32_t USART2_RX: 1;

    /*Pin4*/ uint32_t Pin4: 1;
    /*Pin5*/ uint32_t Ext0: 1;
    /*Pin6*/ uint32_t Int0: 1;
    /*Pin7*/ uint32_t X8D: 1;

    /*Pin8*/ uint32_t X8C: 1;
    /*Pin9*/ uint32_t X8B: 1;
    /*Pin10*/ uint32_t X8A: 1;
    /*Pin11*/ uint32_t Pin11: 1;
    
    /*Pin12*/ uint32_t Pin12: 1;
    /*Pin13*/ uint32_t SWDIO: 1;
    /*Pin14*/ uint32_t SWDCLK: 1;
    /*Pin15*/ uint32_t Pin15: 1;
  };
  uint32_t Value;
} CK_PortA;
//=================================================================================================================================
typedef union{
  struct{
    /*Pin0*/ uint32_t Pin0: 1;
    /*Pin1*/ uint32_t Pin1: 1;
    /*Pin2*/ uint32_t BOOT1: 1;
    /*Pin3*/ uint32_t Pin3: 1;

    /*Pin4*/ uint32_t Pin4: 1;
    /*Pin5*/ uint32_t USART1_DE: 1;
    /*Pin6*/ uint32_t I2C1_SCL: 1;
    /*Pin7*/ uint32_t I2C1_SDA: 1;

    /*Pin8*/ uint32_t Pin8: 1;
    /*Pin9*/ uint32_t Pin9: 1;
    /*Pin10*/ uint32_t Pin10: 1;
    /*Pin11*/ uint32_t Pin11: 1;
    
    /*Pin12*/ uint32_t Flash_CE: 1;
    /*Pin13*/ uint32_t Flash_SCK: 1;	
    /*Pin14*/ uint32_t Flash_SO: 1;	
    /*Pin15*/ uint32_t Flash_SI: 1;
  };
  uint32_t Value;
} CK_PortB;
//=================================================================================================================================
typedef union{
  struct{
    /*Pin0*/ uint32_t DrvEnable: 1;
    /*Pin1*/ uint32_t DrvDivider: 3;
    /*Pin2*/ //uint32_t DRV_M1: 1;
    /*Pin3*/ //uint32_t DRV_M2: 1;

    /*Pin4*/ uint32_t DrvReset: 1;
    /*Pin5*/ uint32_t DrvDir: 1;
    /*Pin6*/ uint32_t DrvStep: 1;	
    /*Pin7*/ uint32_t DrvFault: 1;
	
    /*Pin8*/ uint32_t Flash_WP: 1;
    /*Pin9*/ uint32_t Flash_HOLD: 1;	
    /*Pin10*/ uint32_t X6C: 1;
    /*Pin11*/ uint32_t X6B: 1;
    
    /*Pin12*/ uint32_t X7C: 1;
    /*Pin13*/ uint32_t SensorUp: 1;//SensorUp
    /*Pin14*/ uint32_t SensorDown: 1;
    /*Pin15*/ uint32_t Pin14: 1;
  };
  volatile uint32_t Value;
} CK_PortC;
//=================================================================================================================================
typedef union{
  struct{
    /*Pin0*/ uint32_t Pin0: 1;
    /*Pin1*/ uint32_t Pin1: 1;
    /*Pin2*/ uint32_t Pin2: 1;
    /*Pin3*/ uint32_t Pin3: 1;

    /*Pin4*/ uint32_t Pin4: 1;
    /*Pin5*/ uint32_t Pin5: 1;
    /*Pin6*/ uint32_t Pin6: 1;	
    /*Pin7*/ uint32_t Pin7: 1;
	
    /*Pin8*/ uint32_t Pin8: 1;
    /*Pin9*/ uint32_t Pin9: 1;	
    /*Pin10*/ uint32_t Pin10: 1;
    /*Pin11*/ uint32_t Pin11: 1;
    
    /*Pin12*/ uint32_t Pin12: 1;
    /*Pin13*/ uint32_t Pin13: 1;
    /*Pin14*/ uint32_t Pin14: 1;
    /*Pin15*/ uint32_t Pin15: 1;
  };
  uint32_t Value;
} CK_PortD;
//=================================================================================================================================
typedef struct CK_Ports
{
  struct
  {
    volatile CK_PortA *In;
    volatile CK_PortA *Out;
  } A;

  struct
  {
    volatile CK_PortB *In;
    volatile CK_PortB *Out;
  } B;

  struct
  {
    volatile CK_PortC *In;
    volatile CK_PortC *Out;
  } C;

  struct
  {
    volatile CK_PortD *In;
    volatile CK_PortD *Out;
  } D;
} CK_PortsT;
//=================================================================================================================================
extern CK_PortsT Ports;
//=================================================================================================================================
#endif /* _CK_PORTS_H_ */
