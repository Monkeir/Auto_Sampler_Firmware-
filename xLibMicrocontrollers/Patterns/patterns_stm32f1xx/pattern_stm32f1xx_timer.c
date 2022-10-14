//==============================================================================
#include "patterns_stm32f1xx/pattern_stm32f1xx_timer.h"
#include "stm32f1xx_hal.h"
//==============================================================================
TimStm32_T *xTimer1 = (TimStm32_T*)TIM1;
TimStm32_T *xTimer2 = (TimStm32_T*)TIM2;
TimStm32_T *xTimer3 = (TimStm32_T*)TIM3;
TimStm32_T *xTimer4 = (TimStm32_T*)TIM4;
//==============================================================================
inline void PATTERN_TIMER_INIT()
{
  xTimer1->SR;
  xTimer2->SR;
  xTimer3->SR;
  xTimer4->SR;
}
//==============================================================================
