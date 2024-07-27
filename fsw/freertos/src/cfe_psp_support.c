/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
 *
 * Copyright (c) 2020 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

#include "cmsis_os.h"
#include "target_config.h"

#include <zlib.h>

#define CHUNK 16384

#define CFE_PSP_CPU_ID (GLOBAL_CONFIGDATA.CpuId)
#define CFE_PSP_CPU_NAME (GLOBAL_CONFIGDATA.CpuName)
#define CFE_PSP_SPACECRAFT_ID (GLOBAL_CONFIGDATA.SpacecraftId)

void CFE_PSP_FlushCaches(uint32 type, void *address, uint32 size) {
  if (type != 1) {
    return;
  }
  uint32_t startAddr = (uint32_t)address;
  uint32_t endAddr = startAddr + size;

  startAddr &= ~31;
  endAddr = (endAddr + 31) & ~31;

  SCB_CleanInvalidateDCache_by_Addr((uint32_t *)startAddr, endAddr - startAddr);

  __DSB();
  __ISB();
}

uint32 CFE_PSP_GetProcessorId(void) { return CFE_PSP_CPU_ID; }

uint32 CFE_PSP_GetSpacecraftId(void) { return CFE_PSP_SPACECRAFT_ID; }

const char *CFE_PSP_GetProcessorName(void) { return CFE_PSP_CPU_NAME; }

void CFE_PSP_Decompress(char *srcFileName, char *dstFileName) {
  char buffer[CHUNK];
  int bytes_read;

  // Open the gzip file
  gzFile gz = gzopen(input_file, "rb");
  FILE *out = fopen(output_file, "wb");

  // Decompress the file
  while ((bytes_read = gzread(gz, buffer, CHUNK)) > 0) {
    fwrite(buffer, 1, bytes_read, out);
  }

  // Close the files
  gzclose(gz);
  fclose(out);
}
