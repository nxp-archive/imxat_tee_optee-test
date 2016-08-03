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
#ifndef TA_TIME_H
#define TA_TIME_H

/* This UUID is generated with uuidgen
   the ITU-T UUID generator at http://www.itu.int/ITU-T/asn1/uuid.html */
#define TA_TIME_UUID { 0x6f04bde0, 0x2301, 0x11e6, \
		{ 0x95, 0xe8, 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b} }

/* The TAFs ID implemented in this TA */

#define TA_TIME_CMD_GET_SYSTEM_TIME	0
#define TA_TIME_CMD_GET_TEE_TIMEOUT	1
#define TA_TIME_CMD_GET_REE_TIME	2
#define TA_TIME_CMD_GET_ALARM_TIME	3
#define TA_TIME_CMD_SET_ALARM_TIME	4
#define TA_TIME_CMD_SET_SYSTEM_TIME	5
#define TA_TIME_CMD_GET_EXTENSION_PROPERTY	6

#endif /*TA_TIME_H*/
