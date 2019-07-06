#define USE_HAL_DRIVER
#include "stm32f0xx_hal.h"
#include "ac_synth.h"
#include <stdbool.h>

extern void error_handler();

struct ac_synth *synth;

void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    /* No HSE Oscillator on Nucleo, Activate PLL with HSI as source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PREDIV     = RCC_PREDIV_DIV2;
    RCC_OscInitStruct.PLL.PLLMUL     = RCC_PLL_MUL6; // We will have 48MHz clock
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        error_handler();
    }

    /* Select PLL as system clock source and configure the HCLK, PCLK1 clocks
     * dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
        error_handler();
    }
}

void RCC_Config(void)
{
    /* -- Enable GPIO port A clock -- */
    __HAL_RCC_GPIOA_CLK_ENABLE();
}

void GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* -- Configure the GPIO of MOTOR IN 1 pins -- */
    GPIO_InitStruct.Pin       = GPIO_PIN_6;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* -- Configure the GPIO of MOTOR IN 2 pins -- */
    GPIO_InitStruct.Pin       = GPIO_PIN_7;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* -- Configure the GPIO of FLYBACK pins -- */
    GPIO_InitStruct.Pin       = GPIO_PIN_10;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // LED
    GPIO_InitStruct.Pin   = GPIO_PIN_0;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 1);

}

TIM_OC_InitTypeDef s_config;
TIM_HandleTypeDef  tim_handle_motor;
TIM_HandleTypeDef *tim_handle_motor_ptr = &tim_handle_motor;

TIM_HandleTypeDef  tim_handle_flyback;
TIM_HandleTypeDef *tim_handle_flyback_ptr = &tim_handle_flyback;


extern uint32_t SystemCoreClock;

#define PWM_FREQ_FB (250e3)
#define PWM_FREQ_MOTOR (60e3)
#define CPU_FREQ (SystemCoreClock)

void pwm_init_motor()
{
    /* TIM1 clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();

    /* Time Base configuration */
    tim_handle_motor.Instance               = TIM3;
    tim_handle_motor.Init.Prescaler         = 1;
    tim_handle_motor.Init.CounterMode       = TIM_COUNTERMODE_UP;
    tim_handle_motor.Init.Period            = (uint32_t)((CPU_FREQ / PWM_FREQ_MOTOR) - 1);
    tim_handle_motor.Init.ClockDivision     = 0;
    tim_handle_motor.Init.RepetitionCounter = 0;
    tim_handle_motor.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_PWM_Init(&tim_handle_motor) != HAL_OK) {
        error_handler();
    }

    HAL_TIM_Base_Init(&tim_handle_motor);
    HAL_TIM_Base_Start(&tim_handle_motor);

    /*##-2- Configure the PWM channels #########################################*/
    /* Common configuration for all channels */
    s_config.OCMode       = TIM_OCMODE_PWM1;
    s_config.OCPolarity   = TIM_OCPOLARITY_HIGH;
    s_config.OCFastMode   = TIM_OCFAST_DISABLE;
    s_config.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
    s_config.OCNIdleState = TIM_OCNIDLESTATE_RESET;

    s_config.OCIdleState = TIM_OCIDLESTATE_RESET;

    /* Set the pulse value for channel 1 */
    s_config.Pulse = tim_handle_motor.Init.Period / 2; // 50%
    if (HAL_TIM_PWM_ConfigChannel(&tim_handle_motor, &s_config, TIM_CHANNEL_1) != HAL_OK) {
        /* Configuration Error */
        error_handler();
    }

    /*##-3- Start PWM signals generation #######################################*/
    /* Start channel 1 */
    if (HAL_TIM_PWM_Start_IT(&tim_handle_motor, TIM_CHANNEL_1) != HAL_OK) {
        /* PWM Generation Error */
        error_handler();
    }

    /* Set the pulse value for channel 1 */
    s_config.Pulse = tim_handle_motor.Init.Period / 2; // 50%
    if (HAL_TIM_PWM_ConfigChannel(&tim_handle_motor, &s_config, TIM_CHANNEL_2) != HAL_OK) {
        /* Configuration Error */
        error_handler();
    }

    /*##-3- Start PWM signals generation #######################################*/
    /* Start channel 1 */
    if (HAL_TIM_PWM_Start(&tim_handle_motor, TIM_CHANNEL_2) != HAL_OK) {
        /* PWM Generation Error */
        error_handler();
    }

    HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
}

void pwm_init_flyback()
{
    /* TIM1 clock enable */
    __HAL_RCC_TIM1_CLK_ENABLE();

    /* Time Base configuration */
    tim_handle_flyback.Instance               = TIM1;
    tim_handle_flyback.Init.Prescaler         = 1;
    tim_handle_flyback.Init.CounterMode       = TIM_COUNTERMODE_UP;
    tim_handle_flyback.Init.Period            = (uint32_t)((CPU_FREQ / PWM_FREQ_FB) - 1);
    tim_handle_flyback.Init.ClockDivision     = 0;
    tim_handle_flyback.Init.RepetitionCounter = PWM_FREQ_FB / 2e3;
    tim_handle_flyback.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_PWM_Init(&tim_handle_flyback) != HAL_OK) {
        error_handler();
    }

    HAL_TIM_Base_Init(&tim_handle_flyback);
    HAL_TIM_Base_Start_IT(&tim_handle_flyback);

    /*##-2- Configure the PWM channels #########################################*/
    /* Common configuration for all channels */
    s_config.OCMode       = TIM_OCMODE_PWM1;
    s_config.OCPolarity   = TIM_OCPOLARITY_HIGH;
    s_config.OCFastMode   = TIM_OCFAST_DISABLE;
    s_config.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
    s_config.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    s_config.OCIdleState  = TIM_OCIDLESTATE_RESET;

    /* Set the pulse value for channel 1 */
    s_config.Pulse = tim_handle_flyback.Init.Period / 40; // 4%
    if (HAL_TIM_PWM_ConfigChannel(&tim_handle_flyback, &s_config, TIM_CHANNEL_3) != HAL_OK) {
        /* Configuration Error */
        error_handler();
    }

    /*##-3- Start PWM signals generation #######################################*/
    /* Start channel 1 */
    if (HAL_TIM_PWM_Start_IT(&tim_handle_flyback, TIM_CHANNEL_3) != HAL_OK) {
        /* PWM Generation Error */
        error_handler();
    }
    HAL_NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
}


int main(void)
{
    uint32_t cnt    = 0;
    bool         out_on = false;
    uint16_t     duty   = s_config.Pulse;

    static struct ac_synth _synth = {
        .nco =
            {
                .sample_rate = PWM_FREQ_MOTOR / 2,
            },
    };
    synth = &_synth;

    ac_synth_init(synth, 48.45f); // 33.3
//    ac_synth_init(synth, 65.6f); // 45

    HAL_Init();
    SystemClock_Config();
    RCC_Config();
    pwm_init_motor();
    pwm_init_flyback();
    GPIO_Config();


    for (;;) {
        // if (cnt++ % 500 == 0) {
        //    duty = (duty + 1) % tim_handle_motor.Init.Period;
        //    __HAL_TIM_SET_COMPARE(&tim_handle, TIM_CHANNEL_1, duty);
        //}
        //if (cnt % 1000L == 0) {
        //    out_on = !out_on;
        //    if (out_on) {
        //        __HAL_TIM_SET_COMPARE(&tim_handle_flyback, TIM_CHANNEL_3,
        //                              tim_handle_flyback.Init.Period / 40);
        //    } else {
        //        __HAL_TIM_SET_COMPARE(&tim_handle_flyback, TIM_CHANNEL_3, 0);
        //    }
        //}
        if (cnt++ == 100000L) {
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
            ac_synth_update_freq(synth, 65.6f);
            cnt == 0;
        }
    }
}
