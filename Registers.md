/* ============================================================================
 *  RIRegisters.md: RDRAM interface registers.
 *
 *  RDRAMSIM: RDRAM SIMulator.
 *  Copyright (C) 2013, Tyler J. Stachecki.
 *  All rights reserved.
 *
 *  This file is subject to the terms and conditions defined in
 *  file 'LICENSE', which is part of this source code package.
 * ========================================================================= */
#ifndef RI_REGISTER_LIST
#define RI_REGISTER_LIST \
  X(RI_MODE_REG) \
  X(RI_CONFIG_REG) \
  X(RI_CURRENT_LOAD_REG) \
  X(RI_SELECT_REG) \
  X(RI_REFRESH_REG) \
  X(RI_LATENCY_REG) \
  X(RI_RERROR_REG) \
  X(RI_WERROR_REG)
#endif

RI_REGISTER_LIST

