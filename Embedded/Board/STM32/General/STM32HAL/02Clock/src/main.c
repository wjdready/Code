#include <stdio.h>
#include "stm32h7xx.h"

extern void initialise_monitor_handles(void);
int test_command(char *input_string);
void SystemClock_Config(void);

int main(int argc, char const *argv[])
{
    initialise_monitor_handles();
    char buf[256];

    HAL_Init();
    SystemClock_Config();
    
    while (1)
    {
        gets(buf);
        // printf("you input is %s\n", buf);
        test_command(buf);
    }
    return 0;
}

/* __TEST_COMMAND__START__ */
#include "test_command.h"
static int asdasdasd(int argc, char *argv[])
{
    // printf("clk is %d\n", HAL_RCC_GetHCLKFreq());

    return 0;
}
EXPORT_TEST_COMMAND(asdasdasd, "s", "test Command");
/* __TEST_COMMAND__END__ */


#define print_val(value, fmt)   printf("%-20s: "fmt"\n", #value, value)

#define print_attr(value) print_val(value, "%lu")

/* __TEST_COMMAND__START__ */
#include "stm32h7xx.h"
#include "test_command.h"
static int system_clock_init_test(int argc, char *argv[])
{
    uint32_t hclk_freq = HAL_RCC_GetHCLKFreq();
    uint32_t sys_clock_freq = HAL_RCC_GetSysClockFreq();
    uint32_t pclk1_freq = HAL_RCC_GetPCLK1Freq();

    printf("hclk_freq is        :  %lu Hz\n", hclk_freq);
    printf("sys_clock_freq is   :  %lu Hz\n", sys_clock_freq);
    printf("pclk1_freq is       :  %lu Hz\n", pclk1_freq);

    // 获取时钟源配置
    RCC_OscInitTypeDef osc_init_type;
    HAL_RCC_GetOscConfig(&osc_init_type);

    print_attr(osc_init_type.CSICalibrationValue);
    print_attr(osc_init_type.CSIState);
    print_attr(osc_init_type.HSEState);
    print_attr(osc_init_type.HSI48State);
    print_attr(osc_init_type.HSICalibrationValue);
    print_attr(osc_init_type.HSIState);
    print_attr(osc_init_type.LSEState);
    print_attr(osc_init_type.LSIState);
    print_attr(osc_init_type.OscillatorType);
    print_attr(osc_init_type.PLL.PLLFRACN);
    print_attr(osc_init_type.PLL.PLLM);
    print_attr(osc_init_type.PLL.PLLN);
    print_attr(osc_init_type.PLL.PLLP);
    print_attr(osc_init_type.PLL.PLLQ);
    print_attr(osc_init_type.PLL.PLLR);
    print_attr(osc_init_type.PLL.PLLRGE);
    print_attr(osc_init_type.PLL.PLLSource);
    print_attr(osc_init_type.PLL.PLLSource);
    print_attr(osc_init_type.PLL.PLLState);
    print_attr(osc_init_type.PLL.PLLVCOSEL);

    // 获取时钟配置
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    uint32_t pFLatency;
    HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &pFLatency);

    return 0;
}
EXPORT_TEST_COMMAND(system_clock_init_test, "system_clock_init_test", "Test system clock init");
/* __TEST_COMMAND__END__ */


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Macro to configure the PLL clock source
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSI);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 27;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    printf("CONFIG  OSC ERR\n");
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    printf("CONFIG CLK ERR\n");
  }
}
TEST_FUN_ADD(SystemClock_Config, "sys_clock_config_test", "system clock config test");


void myprint(void)
{
    printf("Now we call some\n");
}