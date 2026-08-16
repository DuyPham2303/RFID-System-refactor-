/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "fatfs_sd.h"
#include "string.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypedef hspi1;

USART_HandleTypedef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_USART_Config(void);
static void MX_SPI1_USART_Config(void);
static void MX_USART1_UART_USART_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

FATFS fs; // file system
FIL fil;  // File
FILINFO fno;
FRESULT fresult; // result
UINT br, bw;     // File read/write count

/**** capacity related *****/
FATFS *pfs;
DWORD fre_clust;
uint32_t total, free_space;

#define BUFFER_SIZE 128
char buffer[BUFFER_SIZE]; // to store strings..

int i = 0;

int bufsize(char *buf)
{
  int i = 0;
  while (*buf++ != '\0')
    i++;
  return i;
}

void clear_buffer(void)
{
  for (int i = 0; i < BUFFER_SIZE; i++)
    buffer[i] = '\0';
}

void send_uart(char *string)
{
  uint8_t len = strlen(string);
  HAL_UART_Transmit(&huart1, (uint8_t *)string, len, HAL_MAX_DELAY); // transmit in blocking mode
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

  /* Reset of all peripherals, USART_Configializes the Flash interface and the Systick. */
  HAL_USART_Config();

  /* USER CODE BEGIN USART_Config */

  /* USER CODE END USART_Config */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysUSART_Config */

  /* USER CODE END SysUSART_Config */

  /* USART_Configialize all configured peripherals */
  MX_GPIO_USART_Config();
  MX_SPI1_USART_Config();
  MX_USART1_UART_USART_Config();
  MX_FATFS_USART_Config();
  /* USER CODE BEGIN 2 */

  HAL_Delay(500);

  fresult = f_mount(&fs, "/", 1);
  if (fresult != FR_OK)
    send_uart("ERROR!!! in mounting SD CARD...\n\n");
  else
    send_uart("SD CARD mounted successfully...\n\n");

  /*************** Card capacity details ********************/

  /* Check free space */
  f_getfree("", &fre_clust, &pfs);

  total = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
  sprintf(buffer, "SD CARD Total Size: \t%d\n", total);
  send_uart(buffer);
  clear_buffer();
  free_space = (uint32_t)(fre_clust * pfs->csize * 0.5);
  sprintf(buffer, "SD CARD Free Space: \t%d\n\n", free_space);
  send_uart(buffer);
  clear_buffer();

  /************* The following operation is using PUTS and GETS *********************/

  /* OPENING file to write/ create a file if it doesn't exist */
  fresult = f_open(&fil, "file1.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);

  /* Writing text */
  f_puts("This data is from the FILE1.txt. And it was written using ...f_puts... ", &fil);

  /*CLOSING file */
  fresult = f_close(&fil);

  if (fresult == FR_OK)
    send_uart("File1.txt created and the data is written \n");

  /* OPENING file to read */
  fresult = f_open(&fil, "file1.txt", FA_READ);

  /* Read string from the file */
  f_gets(buffer, f_size(&fil), &fil);

  send_uart("File1.txt is opened and it contains the data as shown below\n");
  send_uart(buffer);
  send_uart("\n\n");

  /*CLOSING file */
  f_close(&fil);

  clear_buffer();

  /**************** The following operation is using f_write and f_read **************************/

  /* Create second file with read write access and OPENING it */
  fresult = f_open(&fil, "file2.txt", FA_CREATE_ALWAYS | FA_WRITE);

  /* Writing text */
  strcpy(buffer, "This is File2.txt, written using ...f_write... and it says Hello from Controllerstech\n");

  fresult = f_write(&fil, buffer, bufsize(buffer), &bw);

  send_uart("File2.txt created and data is written\n");

  /*CLOSING file */
  f_close(&fil);

  // clearing buffer to show that result obtained is from the file
  clear_buffer();

  /* OPENING second file to read */
  fresult = f_open(&fil, "file2.txt", FA_READ);
  if (fresult == FR_OK)
    send_uart("file2.txt is OPENING and the data is shown below\n");

  /* Read data from the file
   * Please see the function details for the arguments */
  f_read(&fil, buffer, f_size(&fil), &br);
  send_uart(buffer);
  send_uart("\n\n");

  /*CLOSING file */
  f_close(&fil);

  clear_buffer();

  /*********************UPDATING an existing file ***************************/

  /* OPENING the file with write access */
  fresult = f_open(&fil, "file2.txt", FA_OPEN_EXISTING | FA_READ | FA_WRITE);

  /* Move to offset to the end of the file */
  fresult = f_lseek(&fil, f_size(&fil));

  if (fresult == FR_OK)
    send_uart("About to update the file2.txt\n");

  /* write the string to the file */
  fresult = f_puts("This is updated data and it should be in the end", &fil);

  f_close(&fil);

  clear_buffer();

  /* OPENING to read the file */
  fresult = f_open(&fil, "file2.txt", FA_READ);

  /* Read string from the file */
  fresult = f_read(&fil, buffer, f_size(&fil), &br);
  if (fresult == FR_OK)
    send_uart("Below is the data from updated file2.txt\n");
  send_uart(buffer);
  send_uart("\n\n");

  /*CLOSING file */
  f_close(&fil);

  clear_buffer();

  /*************************REMOVING FILES FROM THE DIRECTORY ****************************/

  fresult = f_unlink("/file1.txt");
  if (fresult == FR_OK)
    send_uart("file1.txt removed successfully...\n");

  fresult = f_unlink("/file2.txt");
  if (fresult == FR_OK)
    send_uart("file2.txt removed successfully...\n");

  /* Unmount SDCARD */
  fresult = f_mount(NULL, "/", 1);
  if (fresult == FR_OK)
    send_uart("SD CARD UNMOUNTED successfully...\n");

  /* USER CODE END 2 */

  /* InfUSART_Confige loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */

/**
 * @brief SPI1 USART_Configialization Function
 * @param None
 * @retval None
 */
static void MX_SPI1_USART_Config(void)
{

  /* USER CODE BEGIN SPI1_USART_Config 0 */

  /* USER CODE END SPI1_USART_Config 0 */

  /* USER CODE BEGIN SPI1_USART_Config 1 */

  /* USER CODE END SPI1_USART_Config 1 */
  /* SPI1 parameter configuration*/
  hspi1.pSPIx = SPI1;
  hspi1.Init.SPI_Mode = SPI_Mode_Master;
  hspi1.Init.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  hspi1.Init.SPI_DataSize = SPI_DataSize_8b;
  hspi1.Init.SPI_CPOL = SPI_CPOL_Low;
  hspi1.Init.SPI_CPHA = SPI_CPOL_High;
  hspi1.Init.SPI_NSS = SPI_NSS_Soft;
  hspi1.Init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  hspi1.Init.SPI_FirstBit = SPI_FirstBit_MSB;

  SPI_Init(SPI1, &hspi1.Init);
  /* USER CODE BEGIN SPI1_USART_Config 2 */

  /* USER CODE END SPI1_USART_Config 2 */
}

/**
 * @brief USART1 USART_Configialization Function
 * @param None
 * @retval None
 */
static void USART1_Config(void)
{

  /* USER CODE BEGIN USART1_USART_Config 0 */

  /* USER CODE END USART1_USART_Config 0 */

  /* USER CODE BEGIN USART1_USART_Config 1 */

  /* USER CODE END USART1_USART_Config 1 */
  huart1.pUSARTx = USART1;
  huart1.USART_Config.USART_BaudRate = 115200;
  huart1.USART_Config.USART_WordLength = USART_WordLength_8b;
  huart1.USART_Config.USART_StopBits = USART_StopBits_1;
  huart1.USART_Config.USART_Parity = USART_Parity_No;
  huart1.USART_Config.USART_Mode = USART_Mode_Tx | USART_Mode_Tx;
  huart1.USART_Config.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_OverSampling8Cmd(USART1, ENABLE);

  USART_Init(USART1, &huart1.USART_Config);
  /* USER CODE BEGIN USART1_USART_Config 2 */

  /* USER CODE END USART1_USART_Config 2 */
}

/**
 * @brief GPIO USART_Configialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_USART_Config(void)
{
  GPIO_InitTypeDef GPIO_USART_ConfigStruct = {0};

  /* GPIO Ports Clock Enable */

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);

  /*Configure GPIO pin Output Level */

  GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_RESET);
  /*Configure GPIO pin : PB0 */
  GPIO_USART_ConfigStruct.GPIO_Pin = GPIO_Pin_0;
  GPIO_USART_ConfigStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_USART_ConfigStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_USART_ConfigStruct);
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
