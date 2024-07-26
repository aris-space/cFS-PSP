#include "FreeRTOS.h"
#include "cfe_psp.h"
#include "cfe_psp_config.h"
#include "stm32h7xx_hal.h"
#include "task.h"

bool watchdog_enabled = false;
IWDG_HandleTypeDef IwdgHandle;

// TODO: relies on already called in main HAL_Init() and SystemClock_Config()
void CFE_PSP_WatchdogInit(void) {
  __HAL_RCC_WWDG_CLK_ENABLE();

  // Set up the IWDG handle
  IwdgHandle.Instance = IWDG1;
  IwdgHandle.Init.Prescaler = IWDG_PRESCALER_64;
  IwdgHandle.Init.Reload = CFE_PSP_WATCHDOG_MAX; // Maximum timeout ms

  // Initialize the IWDG
  if (HAL_IWDG_Init(&IwdgHandle) != HAL_OK) {
    return CFE_PSP_ERROR;
  }
}

void WatchdogTask(void *pvParameters) {
  for (;;) {
    if (watchdog_enabled) {
      HAL_IWDG_Refresh(&IwdgHandle);
    }
    // Delay for a period shorter than the watchdog timeout
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void CFE_PSP_WatchdogEnable(void) {
  xTaskCreate(WatchdogTask, "WatchdogTask", configMINIMAL_STACK_SIZE, NULL,
              tskIDLE_PRIORITY, NULL);
}

void CFE_PSP_WatchdogDisable(void) { watchdog_enabled = true; }

void CFE_PSP_WatchdogService(void) { /* TODO: What to do when a timer expires */
}

uint32 CFE_PSP_WatchdogGet(void) { return IwdgHandle->SR; }

void CFE_PSP_WatchdogSet(uint32 WatchdogValue) {
  CFE_PSP_WatchdogDisable();

  // Reinitialize the IWDG with new configuration
  IwdgHandle.Init.Reload = WatchdogValue;
  if (HAL_IWDG_Init(&IwdgHandle) != HAL_OK) {
    return CFE_PSP_ERROR;
  }

  Enable_Watchdog();
}
