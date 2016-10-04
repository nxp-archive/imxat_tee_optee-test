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

#define STR_TRACE_USER_TA "FAILURE_CAAM_SM_TEST"

#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>

#include "failure-caam-smemory-ta.h"
/*
 * Called when the instance of the TA is created. This is the first call in
 * the TA.
 */
TEE_Result TA_CreateEntryPoint(void)
{
	return TEE_SUCCESS;
}

/*
 * Called when the instance of the TA is destroyed if the TA has not
 * crashed or panicked. This is the last call in the TA.
 */
void TA_DestroyEntryPoint(void)
{
}

/*
 * Called when a new session is opened to the TA. *sess_ctx can be updated
 * with a value to be able to identify this session in subsequent calls to the
 * TA.
 */
TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types __unused,
		TEE_Param  params[4], void **sess_ctx)
{
	/* Unused parameters */
	(void)&params;
	(void)&sess_ctx;

	/*
	 * The IMSG() macro is non-standard, TEE Internal API doesn't
	 * specify any means to logging from a TA.
	 */
	printf("TA: TA_FAILURE_CAAM_SM!\n");
	/* If return value != TEE_SUCCESS the session will not be created. */
	return TEE_SUCCESS;
}

/*
 * Called when a session is closed, sess_ctx hold the value that was
 * assigned by TA_OpenSessionEntryPoint().
 */
void TA_CloseSessionEntryPoint(void *sess_ctx)
{
	(void)&sess_ctx; /* Unused parameter */
	printf("TA: TA_FAILURE_CAAM_SM->Goodbye!\n");
}

static TEE_Result failure_caam_sm_memory_secure_caam_sm_alloc(
		uint32_t param_types __unused,
		TEE_Param params[4] __unused)
{
	TEE_Result res = TEE_SUCCESS;
	void *out = NULL;
	uint32_t out_len = 1 * 1024;
	uint32_t hint = TEE_USER_MEM_HINT_CAAM_SM_ALLOC;

	out = TEE_Malloc(out_len, hint);
	if (out) {
		res = TEE_ERROR_GENERIC;
		goto exit;
	}
exit:
	if (out)
		TEE_Free(out);

	return res;
}
/*
 * Called when a TA is invoked. sess_ctx hold that value that was
 * assigned by TA_OpenSessionEntryPoint(). The rest of the parameters
 * comes from normal world.
 */
TEE_Result TA_InvokeCommandEntryPoint(void *sess_ctx,
	uint32_t cmd_id, uint32_t param_types __unused,
	TEE_Param params[4] __unused)
{
	(void)&sess_ctx; /* Unused parameter */

	switch (cmd_id) {
	case TA_CMD_TEST_FAILURE_CAAM_SM_MEMORY_ALLOC:
		return failure_caam_sm_memory_secure_caam_sm_alloc(
					param_types, params);
	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}
}

