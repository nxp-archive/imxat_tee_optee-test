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

enum p_type {
	P_TYPE_BOOL,
	P_TYPE_INT,
	P_TYPE_UUID,
	P_TYPE_IDENTITY,
	P_TYPE_STRING,
	P_TYPE_BINARY_BLOCK,
};

struct p_attr {
	const char *str;
	enum p_type type;
	bool retrieved;
	bool v;
};
static TEE_Result check_returned_prop(
		int line __maybe_unused, char *prop_name __maybe_unused,
		TEE_Result return_res, TEE_Result expected_res,
		uint32_t return_len, uint32_t expected_len)
{
	if (return_res != expected_res) {
		EMSG("From line %d (property name=%s): return_res=0x%x  vs  expected_res=0x%x",
		     line, (prop_name ? prop_name : "unknown"),
		     (unsigned int)return_res, (unsigned int)expected_res);
		return TEE_ERROR_GENERIC;
	}
	if (return_len != expected_len) {
		EMSG("From line %d (property name=%s): return_len=%u  vs  expected_res=%u",
		     line, (prop_name ? prop_name : "unknown"),
		     return_len, expected_len);
		return TEE_ERROR_GENERIC;
	}
	return TEE_SUCCESS;
}
static TEE_Result get_properties(TEE_PropSetHandle h,
				   TEE_PropSetHandle prop_set,
				   struct p_attr *p_attrs, size_t num_p_attrs)
{
	TEE_Result res;
	size_t n;

	TEE_StartPropertyEnumerator(h, prop_set);

	while (true) {
		char nbuf[256];
		char nbuf_small[256];
		char vbuf[256];
		char vbuf2[256];
		uint32_t nblen = sizeof(nbuf);
		uint32_t nblen_small;
		uint32_t vblen = sizeof(vbuf);
		uint32_t vblen2 = sizeof(vbuf2);

		res = TEE_GetPropertyName(h, nbuf, &nblen);
		if (res != TEE_SUCCESS) {
			EMSG("TEE_GetPropertyName returned 0x%x\n",
				 (unsigned int)res);
			return res;
		}
		if (nblen != strlen(nbuf) + 1) {
			EMSG("Name has wrong size: %u vs %zu", nblen, strlen(nbuf) + 1);
			return TEE_ERROR_GENERIC;
		}


		/* Get the property name with a very small buffer */
		nblen_small = 2;
		res = TEE_GetPropertyName(h, nbuf_small, &nblen_small);
		res = check_returned_prop(__LINE__, nbuf, res, TEE_ERROR_SHORT_BUFFER,
					  nblen_small, nblen);
		if (res != TEE_SUCCESS)
			return res;

		/* Get the property name with almost the correct buffer */
		nblen_small = nblen - 1;
		res = TEE_GetPropertyName(h, nbuf_small, &nblen_small);
		res = check_returned_prop(__LINE__, nbuf, res, TEE_ERROR_SHORT_BUFFER,
					  nblen_small, nblen);
		if (res != TEE_SUCCESS)
			return res;

		/* Get the property name with the exact buffer length */
		nblen_small = nblen;
		res = TEE_GetPropertyName(h, nbuf_small, &nblen_small);
		res = check_returned_prop(__LINE__, nbuf, res, TEE_SUCCESS,
					  nblen_small, nblen);
		if (res != TEE_SUCCESS)
			return res;

		/* Get the property value */
		res = TEE_GetPropertyAsString(h, NULL, vbuf, &vblen);
		res = check_returned_prop(__LINE__, nbuf, res, TEE_SUCCESS,
					  vblen, strlen(vbuf) + 1);
		if (res != TEE_SUCCESS)
			return res;

		res = TEE_GetPropertyAsString(prop_set, nbuf, vbuf2, &vblen2);
		res = check_returned_prop(__LINE__, nbuf, res, TEE_SUCCESS,
					  vblen2, strlen(vbuf2) + 1);
		if (res != TEE_SUCCESS)
			return res;

		if (strcmp(vbuf, vbuf2) != 0) {
			EMSG("String of \"%s\" differs\n", nbuf);
			return TEE_ERROR_GENERIC;
		}

		/* Get the property with a very small buffer */
		vblen2 = 1;
		res = TEE_GetPropertyAsString(prop_set, nbuf, vbuf2, &vblen2);
		res = check_returned_prop(__LINE__, nbuf, res, TEE_ERROR_SHORT_BUFFER,
					  vblen2, vblen);
		if (res != TEE_SUCCESS)
			return res;

		/* Get the property with almost the correct buffer */
		vblen2 = vblen - 1;
		res = TEE_GetPropertyAsString(prop_set, nbuf, vbuf2, &vblen2);
		res = check_returned_prop(__LINE__, nbuf, res, TEE_ERROR_SHORT_BUFFER,
					  vblen2, vblen);
		if (res != TEE_SUCCESS)
			return res;

		/* Get the property name with the exact buffer length */
		vblen2 = vblen;
		res = TEE_GetPropertyAsString(prop_set, nbuf, vbuf2, &vblen2);
		res = check_returned_prop(__LINE__, nbuf, res, TEE_SUCCESS, vblen2, vblen);
		if (res != TEE_SUCCESS)
			return res;

		DMSG("Found \"%s\" value \"%s\"\n", nbuf, vbuf);

		for (n = 0; n < num_p_attrs; n++) {
			if (strcmp(nbuf, p_attrs[n].str) != 0)
				continue;

			if (p_attrs[n].retrieved) {
				EMSG("Value \"%s\" already retrieved\n",
					 p_attrs[n].str);
				return TEE_ERROR_GENERIC;
			}
			p_attrs[n].retrieved = true;

			switch (p_attrs[n].type) {
			case P_TYPE_BOOL:
				{
					bool v;

					res =
						TEE_GetPropertyAsBool(h, NULL, &v);
					if (res != TEE_SUCCESS) {
						EMSG(
						"TEE_GetPropertyAsBool(\"%s\") returned 0x%x\n",
						nbuf, (unsigned int)res);
						return res;
					}
					p_attrs[n].v = v;
				}
				break;

			default:
				EMSG("Unknown type (%d) for \"%s\"\n",
					 p_attrs[n].type, p_attrs[n].str);
				return TEE_ERROR_GENERIC;
			}
		}

		res = TEE_GetNextProperty(h);
		if (res != TEE_SUCCESS) {
			if (res == TEE_ERROR_ITEM_NOT_FOUND)
				return TEE_SUCCESS;
			return res;
		}
	}
}
static TEE_Result get_externsion_time_property(uint32_t param_types __unused,TEE_Param params[4])
{
	TEE_Result res = TEE_ERROR_GENERIC;
	TEE_PropSetHandle h;
	struct p_attr p_attrs[] = {
		{"gpd.tee.Time.extension", P_TYPE_BOOL},
	};
	const size_t num_p_attrs = sizeof(p_attrs) / sizeof(p_attrs[0]);
	res = TEE_AllocatePropertyEnumerator(&h);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_AllocatePropertyEnumerator: returned 0x%x\n",
		     (unsigned int)res);
		return TEE_ERROR_GENERIC;
	}

	printf("Getting properties for implementation\n");
	res = get_properties(h, TEE_PROPSET_TEE_IMPLEMENTATION, p_attrs,
			       num_p_attrs);
	if (res != TEE_SUCCESS)
		goto cleanup_return;

	params[0].value.a = 0;
	if(p_attrs[0].retrieved && p_attrs[0].v)
		params[0].value.a = 1;

cleanup_return:
	TEE_FreePropertyEnumerator(h);
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
