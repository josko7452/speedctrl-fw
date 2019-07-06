/**
 ******************************************************************************
 * @file    GPIO/GPIO_IOToggle/Src/stm32f0xx_it.c
 * @author  MCD Application Team
 * @version V1.4.0
 * @date    11-September-2015
 * @brief   Main Interrupt Service Routines.
 *          This file provides template for all exceptions handler and
 *          peripherals interrupt service routine.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_it.h"
#include "stm32f0xx_hal.h"
#include "ac_synth.h"
#include <stdbool.h>

/** @addtogroup STM32F0xx_HAL_Examples
 * @{
 */

/** @addtogroup GPIO_IOToggle
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief  This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void)
{
}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1) {
    }
}


/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
void SVC_Handler(void)
{
}


/**
 * @brief  This function handles PendSVC exception.
 * @param  None
 * @retval None
 */
void PendSV_Handler(void)
{
}

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
    HAL_IncTick();
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    static uint16_t           cnt;
    static bool               neon_on = false;
    extern struct ac_synth *  synth;
    extern TIM_HandleTypeDef *tim_handle_motor_ptr;
    extern TIM_HandleTypeDef *tim_handle_flyback_ptr;
    if (htim == tim_handle_motor_ptr) {
        ac_synth_run(synth);
        __HAL_TIM_SET_COMPARE(tim_handle_motor_ptr, TIM_CHANNEL_1, synth->duty_pos);
        __HAL_TIM_SET_COMPARE(tim_handle_motor_ptr, TIM_CHANNEL_2, synth->duty_neg);
    }
    if (htim == tim_handle_flyback_ptr) {
        // This is 1kHz ISR
        if (cnt++ == (5 - 1)) {
            neon_on = !neon_on;
            cnt = 0;
        }
        if (neon_on) {
            __HAL_TIM_SET_COMPARE(tim_handle_flyback_ptr, TIM_CHANNEL_3,
                                  tim_handle_flyback_ptr->Init.Period / 30);
        } else {
            __HAL_TIM_SET_COMPARE(tim_handle_flyback_ptr, TIM_CHANNEL_3, 0);
        }
    }
}

void TIM3_IRQHandler(void)
{
    extern TIM_HandleTypeDef *tim_handle_motor_ptr;
    HAL_TIM_IRQHandler(tim_handle_motor_ptr);
}

void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
    extern TIM_HandleTypeDef *tim_handle_flyback_ptr;
    HAL_TIM_IRQHandler(tim_handle_flyback_ptr);
}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                               */
/******************************************************************************/

/**
 * @brief  This function handles PPP interrupt request.
 * @param  None
 * @retval None
 */
/*void PPP_IRQHandler(void)
{
}*/


/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
