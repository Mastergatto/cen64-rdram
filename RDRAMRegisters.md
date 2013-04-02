/* ============================================================================
 *  RDRAMRegisters.md: RDRAM registers.
 *
 *  RDRAMSIM: RDRAM SIMulator.
 *  Copyright (C) 2013, Tyler J. Stachecki.
 *  All rights reserved.
 *
 *  This file is subject to the terms and conditions defined in
 *  file 'LICENSE', which is part of this source code package.
 * ========================================================================= */
#ifndef RDRAM_REGISTER_LIST
#define RDRAM_REGISTER_LIST \
  X(RDRAM_CONFIG_REG) \
  X(RDRAM_DEVICE_ID_REG) \
  X(RDRAM_DELAY_REG) \
  X(RDRAM_MODE_REG) \
  X(RDRAM_REF_INTERVAL_REG) \
  X(RDRAM_REF_ROW_REG) \
  X(RDRAM_RAS_INTERVAL_REG) \
  X(RDRAM_MIN_INTERVAL_REG) \
  X(RDRAM_ADDR_SELECT_REG)
#endif

RDRAM_REGISTER_LIST

