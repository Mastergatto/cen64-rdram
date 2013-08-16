/* ============================================================================
 *  Externs.h: External definitions for the RDRAM plugin.
 *
 *  BusSIM: Reality Co-Processor Bus SIMulator.
 *  Copyright (C) 2013, Tyler J. Stachecki.
 *  All rights reserved.
 *
 *  This file is subject to the terms and conditions defined in
 *  file 'LICENSE', which is part of this source code package.
 * ========================================================================= */
#ifndef __RDRAM__EXTERNS_H__
#define __RDRAM__EXTERNS_H__
#include "Controller.h"

#ifdef __cplusplus
#include <cstddef>
#else
#include <stddef.h>
#endif

void RDPSetRDRAMPointer(uint8_t *memory);

#endif

