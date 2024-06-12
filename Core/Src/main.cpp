/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "lwip.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tcpServerRAW.h"
#include "math.h"
#include "string.h"
#include "lwip/apps/httpd.h"
#include "PhaseShifter.h"
#include "PE43xx.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define PI 3.141592653589793

#define LE_ATT_PIN_A 8
#define LE_ATT_PIN_B 8
#define LE_ATT_PIN_C 9
#define LE_ATT_PIN_D 12
#define LE_ATT_PIN_E 8
#define LE_ATT_PIN_F 14
#define LE_ATT_PIN_G 11
#define LE_ATT_PIN_H 1

#define LE_ATT_PORT_A GPIOC
#define LE_ATT_PORT_B GPIOB
#define LE_ATT_PORT_C GPIOB
#define LE_ATT_PORT_D GPIOF
#define LE_ATT_PORT_E GPIOE
#define LE_ATT_PORT_F GPIOE
#define LE_ATT_PORT_G GPIOB
#define LE_ATT_PORT_H GPIOG
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */
extern struct netif gnetif;
char input[100];
char output[100];
double angle_of_attack;
float antenna_distance;

struct tcp_pcb *server_pcb;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// Fonction pour calculer le déphasage de chaque antenne
void calculate_phase_shifts(double angle_of_attack, double wavelength, double distance, int num_antennas, double phase_shifts[]) {
    for (int n = 0; n < num_antennas; n++) {
        phase_shifts[n] = (2 * PI * distance / wavelength) * n * sin(angle_of_attack) * 57.3;
    }
}
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

  GPIO_InitTypeDef GPIO_InitStruct_Si = {
		  .Pin = SI_Pin,
		  .Mode = GPIO_MODE_IT_FALLING,
		  .Pull = GPIO_NOPULL
  };
  GPIO_InitTypeDef GPIO_InitStruct_Le = {
		  .Pin = LE_Pin,
		  .Mode = GPIO_MODE_IT_FALLING,
		  .Pull = GPIO_NOPULL
  };
  GPIO_InitTypeDef GPIO_InitStruct_Clk = {
		  .Pin = CLK_Pin,
		  .Mode = GPIO_MODE_IT_FALLING,
		  .Pull = GPIO_NOPULL
  };
  GPIO_InitTypeDef GPIO_InitStruct_Si_Att = {
		  .Pin = SDI_Att_Pin,
		  .Mode = GPIO_MODE_IT_FALLING,
		  .Pull = GPIO_NOPULL
  };
  GPIO_InitTypeDef GPIO_InitStruct_Clk_Att = {
		  .Pin = CLK_Att_Pin,
		  .Mode = GPIO_MODE_IT_FALLING,
		  .Pull = GPIO_NOPULL
  };
  GPIO_InitTypeDef GPIO_InitStruct_Le_Att_A = {
		  .Pin = LE_ATT_PIN_A,
		  .Mode = GPIO_MODE_IT_FALLING,
		  .Pull = GPIO_NOPULL
  };
  GPIO_InitTypeDef GPIO_InitStruct_Le_Att_B = {
		  .Pin = LE_ATT_PIN_B,
		  .Mode = GPIO_MODE_IT_FALLING,
		  .Pull = GPIO_NOPULL
  };
  GPIO_InitTypeDef GPIO_InitStruct_Le_Att_C = {
		  .Pin = LE_ATT_PIN_C,
		  .Mode = GPIO_MODE_IT_FALLING,
		  .Pull = GPIO_NOPULL
  };
  GPIO_InitTypeDef GPIO_InitStruct_Le_Att_D = {
		  .Pin = LE_ATT_PIN_D,
		  .Mode = GPIO_MODE_IT_FALLING,
		  .Pull = GPIO_NOPULL
  };
  GPIO_InitTypeDef GPIO_InitStruct_Le_Att_E = {
		  .Pin = LE_ATT_PIN_E,
		  .Mode = GPIO_MODE_IT_FALLING,
		  .Pull = GPIO_NOPULL
  };
  GPIO_InitTypeDef GPIO_InitStruct_Le_Att_F = {
		  .Pin = LE_ATT_PIN_F,
		  .Mode = GPIO_MODE_IT_FALLING,
		  .Pull = GPIO_NOPULL
  };
  GPIO_InitTypeDef GPIO_InitStruct_Le_Att_G = {
		  .Pin = LE_ATT_PIN_G,
		  .Mode = GPIO_MODE_IT_FALLING,
		  .Pull = GPIO_NOPULL
  };
  GPIO_InitTypeDef GPIO_InitStruct_Le_Att_H = {
		  .Pin = LE_ATT_PIN_H,
		  .Mode = GPIO_MODE_IT_FALLING,
		  .Pull = GPIO_NOPULL
  };


  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  //MX_ETH_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_LWIP_Init();
  //MX_LWIP_Init();
  /* USER CODE BEGIN 2 */

  server_pcb = tcp_server_init();

  char *cmd, *arg, *option;
  char serial_output[100] = "";

  antenna_distance = 0.05;

  PhaseShifter pe44820_A = PhaseShifter(SI_GPIO_Port, GPIO_InitStruct_Si, LE_GPIO_Port, GPIO_InitStruct_Le, CLK_GPIO_Port, GPIO_InitStruct_Clk, (uint8_t) 0x000, MODE_BINARY_WEIGHTED);
  PhaseShifter pe44820_B = PhaseShifter(SI_GPIO_Port, GPIO_InitStruct_Si, LE_GPIO_Port, GPIO_InitStruct_Le, CLK_GPIO_Port, GPIO_InitStruct_Clk, (uint8_t) 0x001, MODE_BINARY_WEIGHTED);
  PhaseShifter pe44820_C = PhaseShifter(SI_GPIO_Port, GPIO_InitStruct_Si, LE_GPIO_Port, GPIO_InitStruct_Le, CLK_GPIO_Port, GPIO_InitStruct_Clk, (uint8_t) 0x002, MODE_BINARY_WEIGHTED);
  PhaseShifter pe44820_D = PhaseShifter(SI_GPIO_Port, GPIO_InitStruct_Si, LE_GPIO_Port, GPIO_InitStruct_Le, CLK_GPIO_Port, GPIO_InitStruct_Clk, (uint8_t) 0x003, MODE_BINARY_WEIGHTED);
  PhaseShifter pe44820_E = PhaseShifter(SI_GPIO_Port, GPIO_InitStruct_Si, LE_GPIO_Port, GPIO_InitStruct_Le, CLK_GPIO_Port, GPIO_InitStruct_Clk, (uint8_t) 0x004, MODE_BINARY_WEIGHTED);
  PhaseShifter pe44820_F = PhaseShifter(SI_GPIO_Port, GPIO_InitStruct_Si, LE_GPIO_Port, GPIO_InitStruct_Le, CLK_GPIO_Port, GPIO_InitStruct_Clk, (uint8_t) 0x005, MODE_BINARY_WEIGHTED);
  PhaseShifter pe44820_G = PhaseShifter(SI_GPIO_Port, GPIO_InitStruct_Si, LE_GPIO_Port, GPIO_InitStruct_Le, CLK_GPIO_Port, GPIO_InitStruct_Clk, (uint8_t) 0x006, MODE_BINARY_WEIGHTED);
  PhaseShifter pe44820_H = PhaseShifter(SI_GPIO_Port, GPIO_InitStruct_Si, LE_GPIO_Port, GPIO_InitStruct_Le, CLK_GPIO_Port, GPIO_InitStruct_Clk, (uint8_t) 0x007, MODE_BINARY_WEIGHTED);


  PE43xx pe4312_A = PE43xx(SDI_Att_GPIO_Port, GPIO_InitStruct_Si_Att, LE_ATT_PORT_A, GPIO_InitStruct_Le_Att_A, CLK_Att_GPIO_Port, GPIO_InitStruct_Clk_Att, (uint8_t) 0x000, 0);
  PE43xx pe4312_B = PE43xx(SDI_Att_GPIO_Port, GPIO_InitStruct_Si_Att, LE_ATT_PORT_B, GPIO_InitStruct_Le_Att_B, CLK_Att_GPIO_Port, GPIO_InitStruct_Clk_Att, (uint8_t) 0x000, 0);
  PE43xx pe4312_C = PE43xx(SDI_Att_GPIO_Port, GPIO_InitStruct_Si_Att, LE_ATT_PORT_C, GPIO_InitStruct_Le_Att_C, CLK_Att_GPIO_Port, GPIO_InitStruct_Clk_Att, (uint8_t) 0x000, 0);
  PE43xx pe4312_D = PE43xx(SDI_Att_GPIO_Port, GPIO_InitStruct_Si_Att, LE_ATT_PORT_D, GPIO_InitStruct_Le_Att_D, CLK_Att_GPIO_Port, GPIO_InitStruct_Clk_Att, (uint8_t) 0x000, 0);
  PE43xx pe4312_E = PE43xx(SDI_Att_GPIO_Port, GPIO_InitStruct_Si_Att, LE_ATT_PORT_E, GPIO_InitStruct_Le_Att_E, CLK_Att_GPIO_Port, GPIO_InitStruct_Clk_Att, (uint8_t) 0x000, 0);
  PE43xx pe4312_F = PE43xx(SDI_Att_GPIO_Port, GPIO_InitStruct_Si_Att, LE_ATT_PORT_F, GPIO_InitStruct_Le_Att_F, CLK_Att_GPIO_Port, GPIO_InitStruct_Clk_Att, (uint8_t) 0x000, 0);
  PE43xx pe4312_G = PE43xx(SDI_Att_GPIO_Port, GPIO_InitStruct_Si_Att, LE_ATT_PORT_G, GPIO_InitStruct_Le_Att_G, CLK_Att_GPIO_Port, GPIO_InitStruct_Clk_Att, (uint8_t) 0x000, 0);
  PE43xx pe4312_H = PE43xx(SDI_Att_GPIO_Port, GPIO_InitStruct_Si_Att, LE_ATT_PORT_H, GPIO_InitStruct_Le_Att_H, CLK_Att_GPIO_Port, GPIO_InitStruct_Clk_Att, (uint8_t) 0x000, 0);

  HAL_GPIO_WritePin(GPIOC, LE_Att_Pin, GPIO_PIN_RESET);


  //struct tcp_server_struct *esTx;


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  MX_LWIP_Process();
	  if(input[0] != '\0'){

	  		  cmd = strtok(input, " ");
	  		  arg = strtok(NULL, " ");
	  		  option = strtok(NULL, " ");

	  		  if(!strcmp(cmd, "BEAM")){
	  			  const char *test_message = "Hello, this is a test message!";
	  			  tcp_server_send_message(server_pcb, test_message);
	  			  strncpy(serial_output, "Command Received\n\r", 21);
	  			  int value = strtol(arg, NULL, 10);
	  			  if(value){
					// Convertir l'angle d'attaque en radians
					angle_of_attack = value * PI / 180.0;

					// Tableau pour stocker les déphasages
					double phase_shifts[8];

					// Calculer les déphasages
					calculate_phase_shifts(angle_of_attack, 0.03, antenna_distance, 8, phase_shifts);
					pe44820_A.setAngle(phase_shifts[0]);
					pe44820_B.setAngle(phase_shifts[1]);
	  				pe44820_C.setAngle(phase_shifts[2]);
	  				pe44820_D.setAngle(phase_shifts[3]);
	  				pe44820_E.setAngle(phase_shifts[4]);
	  				pe44820_F.setAngle(phase_shifts[5]);
	  				pe44820_G.setAngle(phase_shifts[6]);
	  				pe44820_H.setAngle(phase_shifts[7]);
	  				pe4312_A.setLevel(30);
	  				pe4312_B.setLevel(30);
	  				pe4312_C.setLevel(30);
	  				pe4312_D.setLevel(30);
	  				pe4312_E.setLevel(30);
	  				pe4312_F.setLevel(30);
	  				pe4312_G.setLevel(30);
	  				pe4312_H.setLevel(30);
	  			  }

	  		  }
	  		  else if(!strcmp(cmd, "ANT_DIS")){
	  			  float value = strtof(arg, NULL);
	  			  antenna_distance = value;
	  			strncpy(serial_output, "Antenna distance set to ", 25);
	  			strncat(serial_output, arg, 5);
	  			strncat(serial_output, " meter \n\r", 11);
	  		  }
	  		  else{
	  			  strncpy(serial_output, "Unknown Command\n\r", 20);
	  		  }

	  		  HAL_UART_Transmit(&huart3, (const uint8_t*)serial_output, sizeof(serial_output), 10);
	  		  memset (serial_output, '\0', 100);
	  		  memset (input, '\0', 100);
	  	  }
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_OTG_FS_PCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_FS_Init 0 */

  /* USER CODE END USB_OTG_FS_Init 0 */

  /* USER CODE BEGIN USB_OTG_FS_Init 1 */

  /* USER CODE END USB_OTG_FS_Init 1 */
  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hpcd_USB_OTG_FS.Init.dev_endpoints = 4;
  hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_OTG_FS.Init.Sof_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_OTG_FS_Init 2 */

  /* USER CODE END USB_OTG_FS_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LE_GPIO_Port, LE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, SDI_Att_Pin|CLK_Att_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, SI_Pin|CLK_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LE_Att_GPIO_Port, LE_Att_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : LE_Pin */
  GPIO_InitStruct.Pin = LE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SDI_Att_Pin CLK_Att_Pin */
  GPIO_InitStruct.Pin = SDI_Att_Pin|CLK_Att_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : SI_Pin CLK_Pin */
  GPIO_InitStruct.Pin = SI_Pin|CLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LE_Att_Pin */
  GPIO_InitStruct.Pin = LE_Att_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LE_Att_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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

#ifdef  USE_FULL_ASSERT
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
