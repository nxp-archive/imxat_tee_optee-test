/*
 * Copyright (C) 2016 NXP Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef TA_OCRAM_H
#define TA_OCRAM_H

// UUID:: d7bd7ac0-3215-11e6-adcc-0002a5d5c51b
#define TA_TEST_OCRAM_UUID { 0xd7bd7ac0, 0x3215, 0x11e6, \
		{ 0xad, 0xcc, 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b} }

#define TA_TEST_TEE_NO_OCRAM_MEMORY_UUID { 0xd1578b46, 0x53b0, 0x11e6, \
		{ 0xbe, 0xb8, 0x9e, 0x71, 0x12, 0x8c, 0xae, 0x77} }


/* The TAFs ID implemented in this TA */
#define TA_CMD_TEST_OCRAM_MEMORY_ALLOC		0
#define TA_CMD_TEST_OCRAM_MEMORY_FREE		1
#define TA_CMD_TEST_OCRAM_MEMORY_REALLOC	2
#define TA_CMD_TEST_OCRAM_MEMORY_ACCESS_RIGHTS	3
#define TA_CMD_TEST_OCRAM_AND_DDR_MEMORY	4
#define TA_CMD_TEST_OUT_OF_MEMORY_TA	5
#define TA_CMD_TEST_OUT_OF_MEMORY_IN_OPTEE	6

/* The TAFs ID implemented in this failure-ocram-memory TA */
#define TA_CMD_TEST_FAILURE_OCRAM_MEMORY_OCRAM_MEMORY_ALLOC		7

#define TA_OCRAM_DATA_SIZE                (64 * 1024)
#endif /*TA_OCRAM_H*/
