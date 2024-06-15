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

//#define LE_ATT_PIN_A 8
//#define LE_ATT_PIN_B 8
//#define LE_ATT_PIN_C 9
//#define LE_ATT_PIN_D 12
//#define LE_ATT_PIN_E 8
//#define LE_ATT_PIN_F 14
//#define LE_ATT_PIN_G 11
//#define LE_ATT_PIN_H 1
//
//#define LE_ATT_PORT_A GPIOC
//#define LE_ATT_PORT_B GPIOB
//#define LE_ATT_PORT_C GPIOB
//#define LE_ATT_PORT_D GPIOF
//#define LE_ATT_PORT_E GPIOE
//#define LE_ATT_PORT_F GPIOE
//#define LE_ATT_PORT_G GPIOB
//#define LE_ATT_PORT_H GPIOG
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
void parseString(const char *input, double *vector1, int *vector2, int size);
void removeLeadingChars(char *str, int numCharsToRemove);
void calculate_phase_shifts(double angle_of_attack, double wavelength, double distance, int num_antennas, double phase_shifts[]);
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

  GPIO_InitTypeDef GPIO_InitStruct_Si = {
		  .Pin = SI_Pin_Pin,
		  .Mode = GPIO_MODE_IT_FALLING,
		  .Pull = GPIO_NOPULL
  };
  GPIO_InitTypeDef GPIO_InitStruct_Le = {
		  .Pin = LE_Pin_Pin,
		  .Mode = GPIO_MODE_IT_FALLING,
		  .Pull = GPIO_NOPULL
  };
  GPIO_InitTypeDef GPIO_InitStruct_Clk = {
		  .Pin = CLK_Pin_Pin,
		  .Mode = GPIO_MODE_IT_FALLING,
		  .Pull = GPIO_NOPULL
  };
  GPIO_InitTypeDef GPIO_InitStruct_Le_Att_A = {
		  .Pin = LE_ATT_PIN_A_Pin,
		  .Mode = GPIO_MODE_IT_FALLING,
		  .Pull = GPIO_NOPULL
  };
  GPIO_InitTypeDef GPIO_InitStruct_Le_Att_B = {
		  .Pin = LE_ATT_PIN_B_Pin,
		  .Mode = GPIO_MODE_IT_FALLING,
		  .Pull = GPIO_NOPULL
  };
  GPIO_InitTypeDef GPIO_InitStruct_Le_Att_C = {
		  .Pin = LE_ATT_PIN_C_Pin,
		  .Mode = GPIO_MODE_IT_FALLING,
		  .Pull = GPIO_NOPULL
  };
  GPIO_InitTypeDef GPIO_InitStruct_Le_Att_D = {
		  .Pin = LE_ATT_PIN_D_Pin,
		  .Mode = GPIO_MODE_IT_FALLING,
		  .Pull = GPIO_NOPULL
  };
  GPIO_InitTypeDef GPIO_InitStruct_Le_Att_E = {
		  .Pin = LE_ATT_PIN_E_Pin,
		  .Mode = GPIO_MODE_IT_FALLING,
		  .Pull = GPIO_NOPULL
  };
  GPIO_InitTypeDef GPIO_InitStruct_Le_Att_F = {
		  .Pin = LE_ATT_PIN_F_Pin,
		  .Mode = GPIO_MODE_IT_FALLING,
		  .Pull = GPIO_NOPULL
  };
  GPIO_InitTypeDef GPIO_InitStruct_Le_Att_G = {
		  .Pin = LE_ATT_PIN_G_Pin,
		  .Mode = GPIO_MODE_IT_FALLING,
		  .Pull = GPIO_NOPULL
  };
  GPIO_InitTypeDef GPIO_InitStruct_Le_Att_H = {
		  .Pin = LE_ATT_PIN_H_Pin,
		  .Mode = GPIO_MODE_IT_FALLING,
		  .Pull = GPIO_NOPULL
  };

//  HAL_GPIO_Init(SI_Pin_GPIO_Port, &GPIO_InitStruct_Si);
//  HAL_GPIO_Init(LE_ATT_PORT_A, &GPIO_InitStruct_Le_Att_A);
//  HAL_GPIO_Init(LE_ATT_PORT_B, &GPIO_InitStruct_Le_Att_B);
//  HAL_GPIO_Init(LE_ATT_PORT_C, &GPIO_InitStruct_Le_Att_C);
//  HAL_GPIO_Init(LE_ATT_PORT_D, &GPIO_InitStruct_Le_Att_D);
//  HAL_GPIO_Init(LE_ATT_PORT_E, &GPIO_InitStruct_Le_Att_E);
//  HAL_GPIO_Init(LE_ATT_PORT_F, &GPIO_InitStruct_Le_Att_F);
//  HAL_GPIO_Init(LE_ATT_PORT_G, &GPIO_InitStruct_Le_Att_G);
//  HAL_GPIO_Init(LE_ATT_PORT_H, &GPIO_InitStruct_Le_Att_H);

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

  antenna_distance = 0.025;

  PhaseShifter pe44820_A = PhaseShifter(SI_Pin_GPIO_Port, GPIO_InitStruct_Si, LE_Pin_GPIO_Port, GPIO_InitStruct_Le, CLK_Pin_GPIO_Port, GPIO_InitStruct_Clk, (uint8_t) 0x000, MODE_OPT_2600);
  PhaseShifter pe44820_B = PhaseShifter(SI_Pin_GPIO_Port, GPIO_InitStruct_Si, LE_Pin_GPIO_Port, GPIO_InitStruct_Le, CLK_Pin_GPIO_Port, GPIO_InitStruct_Clk, (uint8_t) 0x001, MODE_OPT_2600);
  PhaseShifter pe44820_C = PhaseShifter(SI_Pin_GPIO_Port, GPIO_InitStruct_Si, LE_Pin_GPIO_Port, GPIO_InitStruct_Le, CLK_Pin_GPIO_Port, GPIO_InitStruct_Clk, (uint8_t) 0x002, MODE_OPT_2600);
  PhaseShifter pe44820_D = PhaseShifter(SI_Pin_GPIO_Port, GPIO_InitStruct_Si, LE_Pin_GPIO_Port, GPIO_InitStruct_Le, CLK_Pin_GPIO_Port, GPIO_InitStruct_Clk, (uint8_t) 0x003, MODE_OPT_2600);
  PhaseShifter pe44820_E = PhaseShifter(SI_Pin_GPIO_Port, GPIO_InitStruct_Si, LE_Pin_GPIO_Port, GPIO_InitStruct_Le, CLK_Pin_GPIO_Port, GPIO_InitStruct_Clk, (uint8_t) 0x004, MODE_OPT_2600);
  PhaseShifter pe44820_F = PhaseShifter(SI_Pin_GPIO_Port, GPIO_InitStruct_Si, LE_Pin_GPIO_Port, GPIO_InitStruct_Le, CLK_Pin_GPIO_Port, GPIO_InitStruct_Clk, (uint8_t) 0x005, MODE_OPT_2600);
  PhaseShifter pe44820_G = PhaseShifter(SI_Pin_GPIO_Port, GPIO_InitStruct_Si, LE_Pin_GPIO_Port, GPIO_InitStruct_Le, CLK_Pin_GPIO_Port, GPIO_InitStruct_Clk, (uint8_t) 0x006, MODE_OPT_2600);
  PhaseShifter pe44820_H = PhaseShifter(SI_Pin_GPIO_Port, GPIO_InitStruct_Si, LE_Pin_GPIO_Port, GPIO_InitStruct_Le, CLK_Pin_GPIO_Port, GPIO_InitStruct_Clk, (uint8_t) 0x007, MODE_OPT_2600);


  PE43xx pe4312_A = PE43xx(SI_Pin_GPIO_Port, GPIO_InitStruct_Si, LE_ATT_PIN_A_GPIO_Port, GPIO_InitStruct_Le_Att_A, CLK_Pin_GPIO_Port, GPIO_InitStruct_Clk, (uint8_t) 0x000, 0);
  PE43xx pe4312_B = PE43xx(SI_Pin_GPIO_Port, GPIO_InitStruct_Si, LE_ATT_PIN_B_GPIO_Port, GPIO_InitStruct_Le_Att_B, CLK_Pin_GPIO_Port, GPIO_InitStruct_Clk, (uint8_t) 0x000, 0);
  PE43xx pe4312_C = PE43xx(SI_Pin_GPIO_Port, GPIO_InitStruct_Si, LE_ATT_PIN_C_GPIO_Port, GPIO_InitStruct_Le_Att_C, CLK_Pin_GPIO_Port, GPIO_InitStruct_Clk, (uint8_t) 0x000, 0);
  PE43xx pe4312_D = PE43xx(SI_Pin_GPIO_Port, GPIO_InitStruct_Si, LE_ATT_PIN_D_GPIO_Port, GPIO_InitStruct_Le_Att_D, CLK_Pin_GPIO_Port, GPIO_InitStruct_Clk, (uint8_t) 0x000, 0);
  PE43xx pe4312_E = PE43xx(SI_Pin_GPIO_Port, GPIO_InitStruct_Si, LE_ATT_PIN_E_GPIO_Port, GPIO_InitStruct_Le_Att_E, CLK_Pin_GPIO_Port, GPIO_InitStruct_Clk, (uint8_t) 0x000, 0);
  PE43xx pe4312_F = PE43xx(SI_Pin_GPIO_Port, GPIO_InitStruct_Si, LE_ATT_PIN_F_GPIO_Port, GPIO_InitStruct_Le_Att_F, CLK_Pin_GPIO_Port, GPIO_InitStruct_Clk, (uint8_t) 0x000, 0);
  PE43xx pe4312_G = PE43xx(SI_Pin_GPIO_Port, GPIO_InitStruct_Si, LE_ATT_PIN_G_GPIO_Port, GPIO_InitStruct_Le_Att_G, CLK_Pin_GPIO_Port, GPIO_InitStruct_Clk, (uint8_t) 0x000, 0);
  PE43xx pe4312_H = PE43xx(SI_Pin_GPIO_Port, GPIO_InitStruct_Si, LE_ATT_PIN_H_GPIO_Port, GPIO_InitStruct_Le_Att_H, CLK_Pin_GPIO_Port, GPIO_InitStruct_Clk, (uint8_t) 0x000, 0);

  HAL_GPIO_WritePin(LE_Pin_GPIO_Port, LE_Pin_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LE_ATT_PIN_A_GPIO_Port, LE_ATT_PIN_A_Pin, GPIO_PIN_RESET);
  HAL_Delay(1000);
  HAL_GPIO_WritePin(LE_ATT_PIN_A_GPIO_Port, LE_ATT_PIN_A_Pin, GPIO_PIN_SET);
  HAL_Delay(1000);
  HAL_GPIO_WritePin(LE_ATT_PIN_A_GPIO_Port, LE_ATT_PIN_A_Pin, GPIO_PIN_RESET);

  // Define the size of the vectors
  int size = 8;
  double vectorD[8];
  int vectorA[8];

  double test = 0;

  // Initialize the vectors to zero
  memset(vectorD, 0, sizeof(vectorD));
  memset(vectorA, 0, sizeof(vectorA));

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
	  			  // Commande pour orienter le faisceau dans un angle spécifique
	  			  strncpy(serial_output, "Command Received\n\r", 21);
	  			  int value = strtol(arg, NULL, 10);
	  			  if(value){
					// Convertir l'angle d'attaque en radians
					angle_of_attack = value * PI / 180.0;

					// Calculer les déphasages
					calculate_phase_shifts(angle_of_attack, 0.03, antenna_distance, 8, vectorD);
					pe44820_A.setAngle(vectorD[0]);
					pe44820_B.setAngle(vectorD[1]);
	  				pe44820_C.setAngle(vectorD[2]);
	  				pe44820_D.setAngle(vectorD[3]);
	  				pe44820_E.setAngle(vectorD[4]);
	  				pe44820_F.setAngle(vectorD[5]);
	  				pe44820_G.setAngle(vectorD[6]);
	  				pe44820_H.setAngle(vectorD[7]);
	  				pe4312_A.setLevel(vectorA[0]);
	  				pe4312_B.setLevel(vectorA[1]);
	  				pe4312_C.setLevel(vectorA[2]);
	  				pe4312_D.setLevel(vectorA[3]);
	  				pe4312_E.setLevel(vectorA[4]);
	  				pe4312_F.setLevel(vectorA[5]);
	  				pe4312_G.setLevel(vectorA[6]);
	  				pe4312_H.setLevel(vectorA[7]);

	  				// Réponse TCP
		  			const char *test_message = "BEAM Command Received";
		  			tcp_server_send_message(server_pcb, test_message);
	  			  }

	  		  }
	  		  else if(!strcmp(cmd, "DISTANCE")){
	  			  // Défini la valeur entre les antennes pour calculer le déphasage
	  			  float value = strtof(arg, NULL);
	  			  antenna_distance = value;

	  			  // Réponse TCP
	  			  const char *test_message = "Antenna distance set ";
	  			  tcp_server_send_message(server_pcb, test_message);

	  			  // Serial
	  			  strncpy(serial_output, "Antenna distance set to ", 25);
	  			  strncat(serial_output, arg, 5);
	  			  strncat(serial_output, " meter \n\r", 11);
	  		  }
	  		  else if(!strcmp(cmd, "MANUEL")){
	  			  // Permet de contôler la valeur de déphasage et d'atténuation de chaque puces
	  			  removeLeadingChars(input, sizeof("MANUAL"));
	  			  parseString(input, vectorD, vectorA, size);

	  				pe4312_A.setLevel(vectorA[0]);
	  				pe4312_B.setLevel(vectorA[1]);
	  				pe4312_C.setLevel(vectorA[2]);
	  				pe4312_D.setLevel(vectorA[3]);
	  				pe4312_E.setLevel(vectorA[4]);
	  				pe4312_F.setLevel(vectorA[5]);
	  				pe4312_G.setLevel(vectorA[6]);
	  				pe4312_H.setLevel(vectorA[7]);

					pe44820_A.setAngle(vectorD[0]);
					pe44820_B.setAngle(vectorD[1]);
	  				pe44820_C.setAngle(vectorD[2]);
	  				pe44820_D.setAngle(vectorD[3]);
	  				pe44820_E.setAngle(vectorD[4]);
	  				pe44820_F.setAngle(vectorD[5]);
	  				pe44820_G.setAngle(vectorD[6]);
	  				pe44820_H.setAngle(vectorD[7]);

	  				// Réponse TCP
		  			const char *test_message = "All values are set ";
		  			tcp_server_send_message(server_pcb, test_message);
	  		  }
	  		  else{
	  			  // Commande unconnue
	  			  strncpy(serial_output, "Unknown Command\n\r", 20);

	  			  // Réponse TCP
	  			  const char *test_message = "Unknown command Received";
	  			  tcp_server_send_message(server_pcb, test_message);
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
	  __HAL_RCC_GPIOG_CLK_ENABLE();
	  __HAL_RCC_GPIOE_CLK_ENABLE();
	  __HAL_RCC_GPIOD_CLK_ENABLE();

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LE_ATT_PIN_G_Pin|LD3_Pin|LD2_Pin
	                          |LE_ATT_PIN_B_Pin|LE_ATT_PIN_C_Pin, GPIO_PIN_RESET);

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(GPIOF, LE_ATT_PIN_D_Pin|LE_Pin_Pin, GPIO_PIN_RESET);

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(GPIOG, LE_ATT_PIN_H_Pin|USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(GPIOE, LE_ATT_PIN_E_Pin|SI_Pin_Pin|CLK_Pin_Pin|LE_ATT_PIN_F_Pin, GPIO_PIN_RESET);

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(GPIOD, SID14_Pin|CLKD15_Pin, GPIO_PIN_RESET);

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(LE_ATT_PIN_A_GPIO_Port, LE_ATT_PIN_A_Pin, GPIO_PIN_RESET);

	  /*Configure GPIO pin : USER_Btn_Pin */
	  GPIO_InitStruct.Pin = USER_Btn_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

	  /*Configure GPIO pins : LD1_Pin LE_ATT_PIN_G_Pin LD3_Pin LD2_Pin
	                           LE_ATT_PIN_B_Pin LE_ATT_PIN_C_Pin */
	  GPIO_InitStruct.Pin = LD1_Pin|LE_ATT_PIN_G_Pin|LD3_Pin|LD2_Pin
	                          |LE_ATT_PIN_B_Pin|LE_ATT_PIN_C_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	  /*Configure GPIO pins : LE_ATT_PIN_D_Pin LE_Pin_Pin */
	  GPIO_InitStruct.Pin = LE_ATT_PIN_D_Pin|LE_Pin_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	  /*Configure GPIO pins : LE_ATT_PIN_H_Pin USB_PowerSwitchOn_Pin */
	  GPIO_InitStruct.Pin = LE_ATT_PIN_H_Pin|USB_PowerSwitchOn_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	  /*Configure GPIO pins : LE_ATT_PIN_E_Pin SI_Pin_Pin CLK_Pin_Pin LE_ATT_PIN_F_Pin */
	  GPIO_InitStruct.Pin = LE_ATT_PIN_E_Pin|SI_Pin_Pin|CLK_Pin_Pin|LE_ATT_PIN_F_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	  /*Configure GPIO pins : SID14_Pin CLKD15_Pin */
	  GPIO_InitStruct.Pin = SID14_Pin|CLKD15_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	  /*Configure GPIO pin : USB_OverCurrent_Pin */
	  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

	  /*Configure GPIO pin : LE_ATT_PIN_A_Pin */
	  GPIO_InitStruct.Pin = LE_ATT_PIN_A_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(LE_ATT_PIN_A_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

// Fonction pour calculer le déphasage de chaque antenne
void calculate_phase_shifts(double angle_of_attack, double wavelength, double distance, int num_antennas, double phase_shifts[]) {
    for (int n = 0; n < num_antennas; n++) {
        phase_shifts[n] = (2 * PI * distance / wavelength) * n * sin(angle_of_attack) * 57.3;
    }
}

void removeLeadingChars(char *str, int numCharsToRemove) {
    for (int i = 0; i < numCharsToRemove; i++){
    	str[i] = '\0';
    }
}

// Function to parse the input string and store values into two arrays
void parseString(const char *input, double *vector1, int *vector2, int size) {
    const char *p = input;
    int index1 = 0, index2 = 0;
    int isVector1 = 1; // Start with the first vector
    char temp[4]; // Assuming single-digit integers with possible '\0' padding

    while (*p != 'S') {
        // Move to the start of the first vector
        if (*p == '[') {
            p++;
            while (*p != ']') {
                if (*p >= '0' && *p <= '9') {
                    // Find the length of the current number
                    int len = 0;
                    while (p[len] >= '0' && p[len] <= '9' && len < 3) {
                        temp[len] = p[len];
                        len++;
                    }
                    temp[len] = '\0'; // Null-terminate the number string

                    // Convert to integer and store in the appropriate vector
                    if (isVector1 && index1 < size) {
                        vector1[index1++] = atoi(temp);
                    } else if (!isVector1 && index2 < size) {
                        vector2[index2++] = atoi(temp);
                    }
                    p += len; // Move the pointer to the end of the current number
                } else {
                    p++;
                }
            }
        }
        if (*p == ']') {
            // Move to the start of the second vector
            if (isVector1) {
                isVector1 = 0; // Switch to the second vector
            }
        }
        p++;
    }
}
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
