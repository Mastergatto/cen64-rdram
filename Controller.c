/* ============================================================================
 *  Controller.c: RDRAM controller.
 *
 *  RDRAMSIM: RDRAM SIMulator.
 *  Copyright (C) 2013, Tyler J. Stachecki.
 *  All rights reserved.
 *
 *  This file is subject to the terms and conditions defined in
 *  file 'LICENSE', which is part of this source code package.
 * ========================================================================= */
#include "Address.h"
#include "Common.h"
#include "Controller.h"
#include "Externs.h"

#ifdef __cplusplus
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#else
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#endif

static void InitRDRAM(struct RDRAMController *);

/* ============================================================================
 *  Mnemonics tables.
 * ========================================================================= */
#ifndef NDEBUG
const char *RIRegisterMnemonics[NUM_RI_REGISTERS] = {
#define X(reg) #reg,
#include "Registers.md"
#undef X
};
const char *RDRAMRegisterMnemonics[NUM_RDRAM_REGISTERS] = {
#define X(reg) #reg,
#include "RDRAMRegisters.md"
#undef X
};
#endif

/* ============================================================================
 *  ConnectRDRAMToBus: Connects a RDRAM instance to a Bus instance.
 * ========================================================================= */
void
ConnectRDRAMToBus(struct RDRAMController *rdram, struct BusController *bus){
  rdram->bus = bus;
}

/* ============================================================================
 *  CopyFromDRAM: Copies a hunk of memory from DRAM.
 * ========================================================================= */
void CopyFromDRAM(struct RDRAMController *rdram,
  void *dest, uint32_t source, size_t size) {
  memcpy(dest, rdram->memory + source, size);
}

/* ============================================================================
 *  CopyToDRAM: Copies a hunk of memory to DRAM.
 * ========================================================================= */
void CopyToDRAM(struct RDRAMController *rdram,
  uint32_t dest, const void *source, size_t size) {
  memcpy(rdram->memory + dest, source, size);
}

/* ============================================================================
 *  CreateRDRAM: Creates and initializes an RDRAM instance.
 * ========================================================================= */
struct RDRAMController *
CreateRDRAM(void) {
  size_t allocSize = sizeof(struct RDRAMController) + RDRAM_ADDRESS_LEN;
  struct RDRAMController *controller;

  if ((controller = (struct RDRAMController*) malloc(allocSize)) == NULL) {
    debug("Failed to allocate memory.");
    return NULL;
  }

  InitRDRAM(controller);
  return controller;
}

/* ============================================================================
 *  DestroyRDRAM: Releases any resources allocated for an RDRAM instance.
 * ========================================================================= */
void
DestroyRDRAM(struct RDRAMController *controller) {
  free(controller);
}

/* ============================================================================
 *  GetRDRAMMemoryPointer: Hack for video system.
 * ========================================================================= */
const uint8_t *
GetRDRAMMemoryPointer(const struct RDRAMController *controller) {
  return controller->memory;
}

/* ============================================================================
 *  InitRDRAM: Initializes the RDRAM controller.
 * ========================================================================= */
static void
InitRDRAM(struct RDRAMController *controller) {
  debug("Initializing RDRAM.");
  memset(controller, 0, sizeof(*controller));

  /* Controller's memory should follow it's state/structure. */
  controller->memory = (uint8_t*) controller + sizeof(*controller);

  /* Prevent valgrind from complaining. */
  memset(controller->memory, 0, RDRAM_ADDRESS_LEN);

  /* Apply temporary hack for RDP. */
  RDPSetRDRAMPointer(controller->memory);

  /* Initialize register values. */
  controller->regs[RI_MODE_REG] = 0xE;
  controller->regs[RI_CONFIG_REG] = 0x40;
  controller->regs[RI_SELECT_REG] = 0x14;
  controller->regs[RI_REFRESH_REG] = 0x63634;
}

/* ============================================================================
 *  RDRAMReadByte: Read a byte from RDRAM.
 * ========================================================================= */
int
RDRAMReadByte(void *_controller, uint32_t address, void *_data) {
  struct RDRAMController *controller = (struct RDRAMController*) _controller;
  uint8_t *data = (uint8_t*) _data, byte;

  address = address - RDRAM_BASE_ADDRESS;

  memcpy(&byte, controller->memory + address, sizeof(byte));
  *data= byte;

  return 0;
}

/* ============================================================================
 *  RDRAMReadDWord: Read a doubleword from RDRAM.
 * ========================================================================= */
int
RDRAMReadDWord(void *_controller, uint32_t address, void *_data) {
  struct RDRAMController *controller = (struct RDRAMController*) _controller;
  uint64_t *data = (uint64_t*) _data, dword;

  address = address - RDRAM_BASE_ADDRESS;

  memcpy(&dword, controller->memory + address, sizeof(dword));
  *data = ByteOrderSwap64(dword);

  return 0;
}

/* ============================================================================
 *  RDRAMReadHWord: Read a halfword from RDRAM.
 * ========================================================================= */
int
RDRAMReadHWord(void *_controller, uint32_t address, void *_data) {
  struct RDRAMController *controller = (struct RDRAMController*) _controller;
  uint16_t *data = (uint16_t*) _data, hword;

  address = address - RDRAM_BASE_ADDRESS;

  memcpy(&hword, controller->memory + address, sizeof(hword));
  *data = ByteOrderSwap16(hword);

  return 0;
}

/* ============================================================================
 *  RDRAMReadWord: Read a word from RDRAM.
 * ========================================================================= */
int
RDRAMReadWord(void *_controller, uint32_t address, void *_data) {
  struct RDRAMController *controller = (struct RDRAMController*) _controller;
  uint32_t *data = (uint32_t*) _data, word;

  address = address - RDRAM_BASE_ADDRESS;

  memcpy(&word, controller->memory + address, sizeof(word));
  *data = ByteOrderSwap32(word);

  return 0;
}

/* ============================================================================
 *  RDRAMRegRead: Read from RDRAM registers.
 * ========================================================================= */
int
RDRAMRegRead(void *_controller, uint32_t address, void *_data) {
  struct RDRAMController *controller = (struct RDRAMController*) _controller;
  uint32_t *data = (uint32_t*) _data;

  address -= RDRAM_REGS_BASE_ADDRESS;
  enum RDRAMRegister reg = (enum RDRAMRegister) (address / 4);

  debugarg("RDRAMRegRead: Reading from register [%s].",
    RDRAMRegisterMnemonics[reg]);

  *data = controller->rdramregs[reg];
  return 0;
}

/* ============================================================================
 *  RDRAMRegWrite: Write to RDRAM registers.
 * ========================================================================= */
int
RDRAMRegWrite(void *_controller, uint32_t address, void *_data) {
  struct RDRAMController *controller = (struct RDRAMController*) _controller;
  uint32_t *data = (uint32_t*) _data;

  address -= RDRAM_REGS_BASE_ADDRESS;
  enum RDRAMRegister reg = (enum RDRAMRegister) (address / 4);

  debugarg("RDRAMRegWrite: Writing to register [%s].",
    RDRAMRegisterMnemonics[reg]);

  controller->rdramregs[reg] = *data;
  return 0;
}

/* ============================================================================
 *  RDRAMWriteByte: Write a byte to RDRAM.
 * ========================================================================= */
int
RDRAMWriteByte(void *_controller, uint32_t address, void *_data) {
  struct RDRAMController *controller = (struct RDRAMController*) _controller;
  uint8_t *data = (uint8_t*) _data;

  address = address - RDRAM_BASE_ADDRESS;

  uint8_t byte = *data;
  memcpy(controller->memory + address, &byte, sizeof(byte));

  return 0;
}

/* ============================================================================
 *  RDRAMWriteDWord: Write a doubleword to RDRAM.
 * ========================================================================= */
int
RDRAMWriteDWord(void *_controller, uint32_t address, void *_data) {
  struct RDRAMController *controller = (struct RDRAMController*) _controller;
  uint64_t *data = (uint64_t*) _data;

  address = address - RDRAM_BASE_ADDRESS;

  uint64_t dword = ByteOrderSwap64(*data);
  memcpy(controller->memory + address, &dword, sizeof(dword));

  return 0;
}

/* ============================================================================
 *  RDRAMWriteHWord: Write a halfword to RDRAM.
 * ========================================================================= */
int
RDRAMWriteHWord(void *_controller, uint32_t address, void *_data) {
  struct RDRAMController *controller = (struct RDRAMController*) _controller;
  uint16_t *data = (uint16_t*) _data;

  address = address - RDRAM_BASE_ADDRESS;

  uint16_t hword = ByteOrderSwap16(*data);
  memcpy(controller->memory + address, &hword, sizeof(hword));

  return 0;
}

/* ============================================================================
 *  RDRAMWriteWord: Write a word to RDRAM.
 * ========================================================================= */
int
RDRAMWriteWord(void *_controller, uint32_t address, void *_data) {
  struct RDRAMController *controller = (struct RDRAMController*) _controller;
  uint32_t *data = (uint32_t*) _data;

  address = address - RDRAM_BASE_ADDRESS;

  uint32_t word = ByteOrderSwap32(*data);
  memcpy(controller->memory + address, &word, sizeof(word));

  return 0;
}
/* ============================================================================
 *  RDRAMWriteWordUnaligned: Write a word to RDRAM.
 * ========================================================================= */
int
RDRAMWriteWordUnaligned(void *_controller, uint32_t address, void *_data) {
  struct RDRAMController *controller = (struct RDRAMController*) _controller;
  struct UnalignedData *data = (struct UnalignedData*) _data;

  address = address - RDRAM_BASE_ADDRESS;

  memcpy(controller->memory + address, data->data, data->size);
  return 0;
}

/* ============================================================================
 *  RIRegRead: Read from RI registers.
 * ========================================================================= */
int
RIRegRead(void *_controller, uint32_t address, void *_data) {
  struct RDRAMController *controller = (struct RDRAMController*) _controller;
  uint32_t *data = (uint32_t*) _data;

  address -= RI_REGS_BASE_ADDRESS;
  enum RIRegister reg = (enum RIRegister) (address / 4);

  debugarg("RIRegRead: Reading from register [%s].", RIRegisterMnemonics[reg]);
  *data = controller->regs[reg];

  return 0;
}

/* ============================================================================
 *  RIRegWrite: Write to RI registers.
 * ========================================================================= */
int
RIRegWrite(void *_controller, uint32_t address, void *_data) {
  struct RDRAMController *controller = (struct RDRAMController*) _controller;
  uint32_t *data = (uint32_t*) _data;

  address -= RI_REGS_BASE_ADDRESS;
  enum RIRegister reg = (enum RIRegister) (address / 4);

  debugarg("RIRegWrite: Writing to register [%s].", RIRegisterMnemonics[reg]);
  controller->regs[reg] = *data;

  return 0;
}

