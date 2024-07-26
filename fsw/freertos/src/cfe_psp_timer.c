#include "common_types.h"

#define CFE_PSP_TIMER_TICKS_PER_SECOND ((int32)configTICK_RATE_HZ)

uint32 CFE_PSP_GetTimerTicksPerSecond(void) {
  return CFE_PSP_TIMER_TICKS_PER_SECOND;
}

uint32 CFE_PSP_GetTimerLow32Rollover(void) { return 0; }

void CFE_PSP_Get_Timebase(uint32 *Tbu, uint32 *Tbl) {
  *Tbu = 0;
  *Tbl = HAL_GetTick();
}

uint32 CFE_PSP_Get_Timer_Tick(void) { return HAL_GetTick(); }

void CFE_PSP_GetTime(OS_time_t *LocalTime) {
  uint64 NormalizedTicks;
  uint32 RegUpper, RegLower;

  CFE_PSP_Get_Timebase(&RegUpper, &RegLower);

  NormalizedTicks = RegUpper;
  NormalizedTicks <<= 32;
  NormalizedTicks |= RegLower;

  NormalizedTicks *= portTICK_PERIOD_MS;

  *LocalTime = (OS_time_t){NormalizedTicks};
}
