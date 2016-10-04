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

#define STR_TRACE_USER_TA "CAAM_SM_TEST"

#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>

#include "ta_caam_sm.h"
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
/*
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);
	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;
*/

	/* Unused parameters */
	(void)&params;
	(void)&sess_ctx;

	/*
	 * The IMSG() macro is non-standard, TEE Internal API doesn't
	 * specify any means to logging from a TA.
	 */
	printf("TA: TA_CAAM_SM!\n");
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
	printf("TA: TA_CAAM_SM->Goodbye!\n");
}

static TEE_Result secure_caam_sm_ddr_alloc_free(uint32_t param_types __unused,
					TEE_Param params[4] __unused)
{
	TEE_Result res = TEE_SUCCESS;
	void *out = NULL;
	void *out2 = NULL;
	uint32_t *pOut;
	uint32_t out_len = 3 * 1024;
	uint32_t hint = TEE_USER_MEM_HINT_CAAM_SM_ALLOC;

	out = TEE_Malloc(out_len, hint);
	if (!out) {
		res = TEE_ERROR_OUT_OF_MEMORY;
		printf("TA: ERROR: TEE_Malloc\n");
		goto exit;
	}
	pOut = (uint32_t *) out;
	pOut[0] = 0x11223344;

	out_len = 10 * 1024;
	hint = TEE_USER_MEM_HINT_NO_FILL_ZERO;

	out2 = TEE_Malloc(out_len, hint);
	if (!out2) {
		printf("TA: ERROR2: TEE_Malloc\n");
		res = TEE_ERROR_OUT_OF_MEMORY;
		goto exit;
	}
	pOut = (uint32_t *) out2;
	pOut[0] = 0xaabbccdd;
exit:
	if (out)
		TEE_Free(out);
	if (out2)
		TEE_Free(out2);

	return res;
}
static TEE_Result secure_caam_sm_tee_out_of_memory(uint32_t param_types,
					TEE_Param params[4])
{
	static const TEE_UUID test_uuid = TA_TEST_TEE_NO_CAAM_SM_MEMORY_UUID;
	TEE_Result res;
	uint32_t ret_orig;
	TEE_TASessionHandle sess;

	res = TEE_OpenTASession(&test_uuid, 0, 0, NULL, &sess, &ret_orig);
	if (res != TEE_SUCCESS) {
		printf("ERROR: TEE_OpenTASession failed\n");
		res = TEE_ERROR_GENERIC;
		goto exit;
	}

	res = TEE_InvokeTACommand(sess, 0,
			TA_CMD_TEST_FAILURE_CAAM_SM_MEMORY_ALLOC,
			param_types, params, &ret_orig);
	if (res != TEE_SUCCESS) {
		printf("ERROR:TEE_InvokeTACommand\n");
		goto exit;
	}

	res = TEE_SUCCESS;
	return res;
exit:
	TEE_CloseTASession(sess);
	return res;
}

static TEE_Result secure_caam_sm_malloc_out_of_memory(
		uint32_t param_types __unused,
		TEE_Param params[4] __unused)
{
	TEE_Result res = TEE_SUCCESS;
	void *out = NULL;
	uint32_t out_len = 200 * 1024;
	uint32_t hint = TEE_USER_MEM_HINT_CAAM_SM_ALLOC;

	out = TEE_Malloc(out_len, hint);
	if (out) {
		res = TEE_ERROR_GENERIC;
		printf("ERROR: TEE_Malloc MUST FAIL for size: %d\n", out_len);
		goto exit;
	}
exit:
	if (out)
		TEE_Free(out);

	return res;
}
static TEE_Result secure_caam_sm_alloc(uint32_t param_types __unused,
					TEE_Param params[4] __unused)
{
	TEE_Result res = TEE_SUCCESS;
	void *out = NULL;
	uint32_t *pOut;
	uint32_t out_len = 1 * 1024;
	uint32_t hint = TEE_USER_MEM_HINT_CAAM_SM_ALLOC;

	out = TEE_Malloc(out_len, hint);
	if (!out) {
		res = TEE_ERROR_OUT_OF_MEMORY;
		goto exit;
	}
	pOut = (uint32_t *) out;
	pOut[0] = 0x11223344;
exit:
	if (out)
		TEE_Free(out);

	return res;
}
static TEE_Result secure_caam_sm_realloc(uint32_t param_types __unused,
					TEE_Param params[4] __unused)
{
	TEE_Result res = TEE_SUCCESS;
	void *out = NULL;
	uint32_t *pOut;
	uint32_t out_len = 1 * 1024;
	uint32_t hint = TEE_USER_MEM_HINT_CAAM_SM_ALLOC;

	/*	Malloc	*/
	out = TEE_Malloc(out_len, hint);
	if (!out) {
		res = TEE_ERROR_OUT_OF_MEMORY;
		goto exit;
	}
	pOut = (uint32_t *) out;
	pOut[0] = 0xaabbccdd;

	/*	Re-alloc	*/
	out_len = 2 * 1024;
	out = TEE_Realloc(out, out_len);
	if (!out) {
		res = TEE_ERROR_OUT_OF_MEMORY;
		goto exit;
	}
	pOut = (uint32_t *) out;
exit:
	if (out)
		TEE_Free(out);
	return res;
}
static TEE_Result secure_caam_sm_check_access_rights(
		uint32_t param_types __unused,
		TEE_Param params[4] __unused)
{
	TEE_Result res = TEE_SUCCESS;
	void *out = NULL;
	void *temp_out = NULL;
	uint32_t *pOut;
	uint32_t out_len = 1 * 1024;
	uint32_t hint = TEE_USER_MEM_HINT_CAAM_SM_ALLOC;

	/*	Malloc	*/
	out = TEE_Malloc(out_len, hint);
	if (!out) {
		res = TEE_ERROR_OUT_OF_MEMORY;
		goto exit;
	}
	pOut = (uint32_t *) out;
	pOut[0] = 0xaabbccdd;

	/*	Re-alloc	*/
	out_len = 2 * 1024;
	out = TEE_Realloc(out, out_len);
	if (!out) {
		res = TEE_ERROR_OUT_OF_MEMORY;
		goto exit;
	}
	pOut = (uint32_t *) out;

	/*	Success Test Case	*/
	res = TEE_CheckMemoryAccessRights(
			TEE_MEMORY_ACCESS_READ | TEE_MEMORY_ACCESS_ANY_OWNER,
			out, out_len);
	if (res != TEE_SUCCESS) {
		printf("ERROR: TEE_CheckMemoryAccessRights\n");
		goto exit;
	}
	res = TEE_CheckMemoryAccessRights(TEE_MEMORY_ACCESS_READ,
				out, out_len);
	if (res != TEE_SUCCESS) {
		printf("ERROR: TEE_MEMORY_ACCESS_READ *MUST PASS*]\n");
	    goto exit;
	}

	/*	Failure Test Case	*/
	temp_out = (unsigned char *)out + 32768;
	res = TEE_CheckMemoryAccessRights(TEE_MEMORY_ACCESS_READ,
			temp_out, out_len);
	if (res != TEE_ERROR_ACCESS_DENIED) {
		printf("ERROR: MUST return TEE_ERROR_ACCESS_DENIED*]\n");
		res = TEE_ERROR_GENERIC;
		goto exit;
	}

	res = TEE_SUCCESS;
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
TEE_Result TA_InvokeCommandEntryPoint(void *sess_ctx, uint32_t cmd_id,
			uint32_t param_types, TEE_Param params[4])
{
	(void)&sess_ctx; /* Unused parameter */

	switch (cmd_id) {
	case TA_CMD_TEST_CAAM_SM_MEMORY_ALLOC:
	case TA_CMD_TEST_CAAM_SM_MEMORY_FREE:
		return secure_caam_sm_alloc(param_types, params);
	case TA_CMD_TEST_CAAM_SM_MEMORY_REALLOC:
		return secure_caam_sm_realloc(param_types, params);
	case TA_CMD_TEST_CAAM_SM_MEMORY_ACCESS_RIGHTS:
		return secure_caam_sm_check_access_rights(param_types, params);
	case TA_CMD_TEST_CAAM_SM_AND_DDR_MEMORY:
		return secure_caam_sm_ddr_alloc_free(param_types, params);
	case TA_CMD_TEST_OUT_OF_SMEMORY_TA:
		return secure_caam_sm_malloc_out_of_memory(param_types, params);
	case TA_CMD_TEST_OUT_OF_SMEMORY_IN_OPTEE:
		return secure_caam_sm_tee_out_of_memory(param_types, params);
	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}
}

