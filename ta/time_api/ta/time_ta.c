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

#define STR_TRACE_USER_TA "TIME"

#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>

#include "ta_time.h"
#include <string.h>

static TEE_Result get_externsion_time_property(uint32_t param_types __unused,TEE_Param params[4])
{
	TEE_Result res = TEE_ERROR_GENERIC;
	bool a;

	res = TEE_GetPropertyAsBool(TEE_PROPSET_TEE_IMPLEMENTATION,
			(char *)"gpd.tee.Time.extension", &a);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_GetPropertyAsBool returned 0x%x\n",
				(unsigned int)res);
		return res;
	}
	params[0].value.a = 0;
	if (a)
		params[0].value.a = 1;

	return res;
}
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
TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types,
		TEE_Param  params[4], void **sess_ctx)
{
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);
	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	/* Unused parameters */
	(void)&params;
	(void)&sess_ctx;

	/*
	 * The IMSG() macro is non-standard, TEE Internal API doesn't
	 * specify any means to logging from a TA.
	 */
	IMSG("TIME API Test!\n");
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
	IMSG("Goodbye!\n");
}

static TEE_Result get_time(uint32_t param_types,TEE_Param params[4])
{
	TEE_Time time;
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INOUT,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);

	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	TEE_GetSystemTime(&time);
	params[0].value.a = time.seconds;
	params[0].value.b = time.millis;

	return TEE_SUCCESS;
}

static TEE_Result get_tee_timeout(uint32_t param_types, TEE_Param params[4])
{
	TEE_Result res = TEE_SUCCESS;
	(void)param_types;

	DMSG("ta_entry_wait: waiting %d\n", (unsigned int)params[0].value.a);
	/* Wait */
	res = TEE_Wait(params[0].value.a);

		return res;
}

static TEE_Result get_ree_time(uint32_t param_types,TEE_Param params[4])
{
	TEE_Time time;
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_OUTPUT,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);

	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	TEE_GetREETime(&time);
	params[0].value.a = time.seconds;
	params[0].value.b = time.millis;

	return TEE_SUCCESS;
}

static TEE_Result get_alarm_time(uint32_t param_types,TEE_Param params[4])
{
	TEE_Time time;
	TEE_Result res;
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_OUTPUT,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);

	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	res = TEE_GetAlarmTime(&time);
	if (res == TEE_SUCCESS)
		params[0].value.a = time.seconds;
	return res;
}

static TEE_Result set_alarm_time(uint32_t param_types,TEE_Param params[4])
{
	TEE_Time time;
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);

	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	time.seconds = params[0].value.a;
	return TEE_SetAlarmTime(&time);
}

static TEE_Result set_system_time(uint32_t param_types,TEE_Param params[4])
{
	TEE_Time time;
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);

	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	time.seconds = params[0].value.a;
	return TEE_SetSystemTime(&time);
}

/*
 * Called when a TA is invoked. sess_ctx hold that value that was
 * assigned by TA_OpenSessionEntryPoint(). The rest of the paramters
 * comes from normal world.
 */
TEE_Result TA_InvokeCommandEntryPoint(void *sess_ctx, uint32_t cmd_id,
			uint32_t param_types, TEE_Param params[4])
{
	(void)&sess_ctx; /* Unused parameter */

	switch (cmd_id) {
	case TA_TIME_CMD_GET_SYSTEM_TIME:
		return get_time(param_types,params);
	case TA_TIME_CMD_GET_TEE_TIMEOUT:
		return get_tee_timeout(param_types,params);
	case TA_TIME_CMD_GET_REE_TIME:
		return get_ree_time(param_types,params);
	case TA_TIME_CMD_GET_ALARM_TIME:
		return get_alarm_time(param_types,params);
	case TA_TIME_CMD_SET_ALARM_TIME:
		return set_alarm_time(param_types,params);
	case TA_TIME_CMD_SET_SYSTEM_TIME:
		return set_system_time(param_types,params);
	case TA_TIME_CMD_GET_EXTENSION_PROPERTY:
		return get_externsion_time_property(param_types,params);
	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}
}
