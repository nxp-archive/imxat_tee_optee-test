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
#ifndef TA_OCOTP_H
#define TA_OCOTP_H

/* This UUID is generated with uuidgen
   the ITU-T UUID generator at http://www.itu.int/ITU-T/asn1/uuid.html */
#define TA_OCOTP_UUID { 0xcb356460, 0x3789, 0x11e6, \
		{ 0x8b, 0xdf, 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b} }

/* The TAFs ID implemented in this TA */

#define TA_OCOTP_CMD_GET_FUSE	0
#define TA_OCOTP_CMD_SET_FUSE	1

/* enum for fusebank and fuse words which is passed as a argument in Global platform APIs and system Call API */
enum fusebank {
	BANK0 = 0,
	BANK1,
	BANK3 = 3,
	BANK4,
	BANK5,
};

enum fuseword {
	WORD0 = 0,
	WORD1,
	WORD2,
	WORD3,
	WORD4,
	WORD5,
	WORD6,
	WORD7,
};

enum fuse_category {
	TEE_SHADOW_CACHE = 0,
	TEE_FUSE_BOX,
};

enum operations {
	TEE_SHADOW_READ = 0,
	TEE_FUSE_BOX_SENSE,
	TEE_SHADOW_OVERRIDE,
	TEE_FUSE_BOX_PROG,
};


/* structure in TA which mapped the entered char string into particular fusebank and fuseword */
struct otp_fuse_mapping {
	char *fuse_name;
	unsigned int fusebank_index;
	unsigned int fuseword_index;
};
#endif /*TA_OCOTP_H*/
