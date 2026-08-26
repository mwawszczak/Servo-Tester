/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mode_fsm.h"
#include "buttons.h"
#include "led.h"
#include "pos_input.h"
#include "servo.h"
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct {
	led_pattern_t led_auto_pattern;
	led_pattern_t led_manual_pattern;
} led_pattern_pair_t;


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static const led_pattern_pair_t led_table[SERVO_MODE_COUNT][ARM_STATUS_COUNT] = {
		[SERVO_MODE_MANUAL] = {
				[ARMED] = {
						.led_auto_pattern = LED_OFF,
						.led_manual_pattern = LED_ON
				},
				[DISARMED] = {
						.led_auto_pattern = LED_OFF,
						.led_manual_pattern = LED_SLOW_BLINK
				}
		},
		[SERVO_MODE_AUTO_CENTER] = {
				[ARMED] = {
						.led_auto_pattern = LED_ON,
						.led_manual_pattern = LED_OFF
				},
				[DISARMED] = {
						.led_auto_pattern = LED_SLOW_BLINK,
						.led_manual_pattern = LED_OFF
				}
		},
		[SERVO_MODE_AUTO_MIN] = {
				[ARMED] = {
						.led_auto_pattern = LED_FAST_BLINK_x1,
						.led_manual_pattern = LED_OFF
				},
				[DISARMED] = {
						.led_auto_pattern = LED_SLOW_BLINK,
						.led_manual_pattern = LED_OFF
				}
			},
		[SERVO_MODE_AUTO_MAX] = {
				[ARMED] = {
						.led_auto_pattern = LED_FAST_BLINK_x2,
						.led_manual_pattern = LED_OFF
				},
				[DISARMED] = {
						.led_auto_pattern = LED_SLOW_BLINK,
						.led_manual_pattern = LED_OFF
				}
		},
		[SERVO_MODE_AUTO_SWEEP] = {
				[ARMED] = {
						.led_auto_pattern = LED_FAST_BLINK,
						.led_manual_pattern = LED_OFF
				},
				[DISARMED] = {
						.led_auto_pattern = LED_SLOW_BLINK,
						.led_manual_pattern = LED_OFF
				}
		}
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  fsm_init();
  arm_status_t arm_status = fsm_is_armed();
  arm_status_t arm_status_previous = arm_status;
  servo_mode_t current_mode = fsm_get_mode();
  servo_mode_t previous_mode = current_mode;

  led_pattern_select(LED_MANUAL, led_table[current_mode][arm_status].led_manual_pattern);
  led_pattern_select(LED_AUTO, led_table[current_mode][arm_status].led_auto_pattern);

  position_input_init();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (button_get(BTN_MODE)) fsm_handle_event(EVT_SW_MODE);
	  if (button_get(BTN_CENTER)) fsm_handle_event(EVT_SW_CENTER);
	  if (button_get(BTN_ARM)) fsm_handle_event(EVT_SW_ARM);

	  arm_status = fsm_is_armed();
	  current_mode = fsm_get_mode();


	  bool mode_changed;
	  bool arm_changed;

	  mode_changed = (current_mode != previous_mode);
	  previous_mode = current_mode;

	  arm_changed = (arm_status != arm_status_previous);
	  arm_status_previous = arm_status;

	  // LED pattern select, only once in loop (it resets time position)
	  if (arm_changed || mode_changed) {
		  led_pattern_select(LED_MANUAL, led_table[current_mode][arm_status].led_manual_pattern);
		  led_pattern_select(LED_AUTO, led_table[current_mode][arm_status].led_auto_pattern);
	  }

	  switch(current_mode) {
		  case SERVO_MODE_MANUAL:
			  servo_position_set(position_input_get());
			  break;
		  case SERVO_MODE_AUTO_CENTER:
			  servo_center();
			  break;
		  case SERVO_MODE_AUTO_MIN:
			  servo_min();
			  break;
		  case SERVO_MODE_AUTO_MAX:
			  servo_max();
			  break;
		  case SERVO_MODE_AUTO_SWEEP:
			  servo_sweep();
			  break;
		  default:
			  break;
	  }

	  if (arm_changed) {
		  if (arm_status == ARMED) servo_arm();
		  else servo_disarm();
	  }


	  led_update();

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
