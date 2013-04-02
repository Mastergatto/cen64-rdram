/* ============================================================================
 *  Controller.h: RDRAM controller.
 *
 *  RDRAMSIM: RDRAM SIMulator.
 *  Copyright (C) 2013, Tyler J. Stachecki.
 *  All rights reserved.
 *
 *  This file is subject to the terms and conditions defined in
 *  file 'LICENSE', which is part of this source code package.
 * ========================================================================= */
#ifndef __RDRAM__CONTROLLER_H__
#define __RDRAM__CONTROLLER_H__
#ifdef __cplusplus
#include <cstddef>
#else
#include <stddef.h>
#endif

enum RIRegister {
#define X(reg) reg,
#include "Registers.md"
#undef X
  NUM_RI_REGISTERS
};

enum RDRAMRegister {
#define X(reg) reg,
#include "RDRAMRegisters.md"
#undef X
  NUM_RDRAM_REGISTERS
};

#ifndef NDEBUG
extern const char *RIRegisterMnemonics[NUM_RI_REGISTERS];
extern const char *RDRAMRegisterMnemonics[NUM_RDRAM_REGISTERS];
#endif

struct BusController;

struct RDRAMController {
  struct BusController *bus;
  uint8_t *memory;

  uint32_t regs[NUM_RI_REGISTERS];
  uint32_t rdramregs[NUM_RDRAM_REGISTERS];
};

struct UnalignedData {
  uint32_t data;
  size_t size;
};

struct RDRAMController *CreateRDRAM();
void DestroyRDRAM(struct RDRAMController *);

#endif

