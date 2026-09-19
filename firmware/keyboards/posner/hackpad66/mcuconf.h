// Copyright 2026 Andrew Posner
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once
#include_next <mcuconf.h>

/* GP0/GP1 belong to the I2C0 peripheral, which ChibiOS leaves off by default
 * on RP2040 (it enables I2C1). Turn I2C0 on and I2C1 off. */
#undef  RP_I2C_USE_I2C0
#define RP_I2C_USE_I2C0 TRUE
#undef  RP_I2C_USE_I2C1
#define RP_I2C_USE_I2C1 FALSE
