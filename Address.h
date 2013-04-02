/* ============================================================================
 *  Address.h: Device address list.
 *
 *  RDRAMSIM: RDRAM SIMulator.
 *  Copyright (C) 2013, Tyler J. Stachecki.
 *  All rights reserved.
 *
 *  This file is subject to the terms and conditions defined in
 *  file 'LICENSE', which is part of this source code package.
 * ========================================================================= */
#ifndef __RDRAM__ADDRESS_H__
#define __RDRAM__ADDRESS_H__

/* RDRAM (Memory). */
#define RDRAM_BASE_ADDRESS        0x00000000
#define RDRAM_ADDRESS_LEN         0x00800000

/* RDRAM Registers. */
#define RDRAM_REGS_BASE_ADDRESS   0x03F00000
#define RDRAM_REGS_ADDRESS_LEN    0x00000028

/* RDRAM Interface Registers. */
#define RI_REGS_BASE_ADDRESS      0x04700000
#define RI_REGS_ADDRESS_LEN       0x00000020

#endif

