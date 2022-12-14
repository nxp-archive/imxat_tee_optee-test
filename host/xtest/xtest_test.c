/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License Version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include "xtest_test.h"
#include <tee_client_api.h>
#include <ta_create_fail_test.h>
#include <ta_crypt.h>
#include <ta_os_test.h>
#include <ta_rpc_test.h>
#include <ta_sims_test.h>
#include <ta_storage.h>
#include <ta_concurrent.h>
#include <ta_concurrent_large.h>
#include <enc_fs_key_manager_test.h>
#include <ta_storage_benchmark.h>
#include <ta_ocram.h>
#include <ta_time.h>
#include <ta_ocotp.h>
#include <tee_api_defines.h>
#ifdef WITH_GP_TESTS
#include <tee_api_types.h>
#include <TTA_DS_protocol.h>
#endif
#ifdef CFG_SNVS_MC
#include <ta_mc.h>
#endif

ADBG_ENUM_TABLE_DEFINE_BEGIN(TEEC_Result)
ADBG_ENUM_TABLE_ENTRY(TEEC_SUCCESS),
ADBG_ENUM_TABLE_ENTRY(TEE_ERROR_CORRUPT_OBJECT),
ADBG_ENUM_TABLE_ENTRY(TEE_ERROR_CORRUPT_OBJECT_2),
ADBG_ENUM_TABLE_ENTRY(TEE_ERROR_STORAGE_NOT_AVAILABLE),
ADBG_ENUM_TABLE_ENTRY(TEE_ERROR_STORAGE_NOT_AVAILABLE_2),
ADBG_ENUM_TABLE_ENTRY(TEEC_ERROR_GENERIC),
ADBG_ENUM_TABLE_ENTRY(TEEC_ERROR_ACCESS_DENIED),
ADBG_ENUM_TABLE_ENTRY(TEEC_ERROR_CANCEL),
ADBG_ENUM_TABLE_ENTRY(TEEC_ERROR_ACCESS_CONFLICT),
ADBG_ENUM_TABLE_ENTRY(TEEC_ERROR_EXCESS_DATA),
ADBG_ENUM_TABLE_ENTRY(TEEC_ERROR_BAD_FORMAT),
ADBG_ENUM_TABLE_ENTRY(TEEC_ERROR_BAD_PARAMETERS),
ADBG_ENUM_TABLE_ENTRY(TEEC_ERROR_BAD_STATE),
ADBG_ENUM_TABLE_ENTRY(TEEC_ERROR_ITEM_NOT_FOUND),
ADBG_ENUM_TABLE_ENTRY(TEEC_ERROR_NOT_IMPLEMENTED),
ADBG_ENUM_TABLE_ENTRY(TEEC_ERROR_NOT_SUPPORTED),
ADBG_ENUM_TABLE_ENTRY(TEEC_ERROR_NO_DATA),
ADBG_ENUM_TABLE_ENTRY(TEEC_ERROR_OUT_OF_MEMORY),
ADBG_ENUM_TABLE_ENTRY(TEEC_ERROR_BUSY),
ADBG_ENUM_TABLE_ENTRY(TEEC_ERROR_COMMUNICATION),
ADBG_ENUM_TABLE_ENTRY(TEEC_ERROR_SECURITY),
ADBG_ENUM_TABLE_ENTRY(TEEC_ERROR_SHORT_BUFFER),
ADBG_ENUM_TABLE_ENTRY(TEEC_ERROR_EXTERNAL_CANCEL),
ADBG_ENUM_TABLE_ENTRY(TEE_ERROR_OVERFLOW),
ADBG_ENUM_TABLE_ENTRY(TEE_ERROR_TARGET_DEAD),
ADBG_ENUM_TABLE_ENTRY(TEE_ERROR_STORAGE_NO_SPACE),
ADBG_ENUM_TABLE_ENTRY(TEE_ERROR_MAC_INVALID),
ADBG_ENUM_TABLE_ENTRY(TEE_ERROR_SIGNATURE_INVALID),
ADBG_ENUM_TABLE_ENTRY(TEE_ERROR_TIME_NOT_SET),
ADBG_ENUM_TABLE_ENTRY(TEE_ERROR_TIME_NEEDS_RESET)
ADBG_ENUM_TABLE_DEFINE_END(TEEC_Result);

ADBG_ENUM_TABLE_DEFINE_BEGIN(TEEC_ErrorOrigin)
ADBG_ENUM_TABLE_ENTRY(TEEC_ORIGIN_API),
ADBG_ENUM_TABLE_ENTRY(TEEC_ORIGIN_COMMS),
ADBG_ENUM_TABLE_ENTRY(TEEC_ORIGIN_TEE),
ADBG_ENUM_TABLE_ENTRY(TEEC_ORIGIN_TRUSTED_APP)
ADBG_ENUM_TABLE_DEFINE_END(TEEC_ErrorOrigin);

#define ECC_SELF_TEST_UUID \
		{ 0xf34f4f3c, 0xab30, 0x4573,  \
		{ 0x91, 0xBF, 0x3C, 0x57, 0x02, 0x4D, 0x51, 0x99 } }

#define STA_SELF_TEST_UUID \
		{ 0xd96a5b40, 0xc3e5, 0x21e3, \
		{ 0x87, 0x94, 0x10, 0x02, 0xa5, 0xd5, 0xc6, 0x1b } }

#ifdef CFG_SNVS_MC
const TEEC_UUID snvs_mc_test_ta_uuid = TA_MC_UUID;
#endif
const TEEC_UUID crypt_user_ta_uuid = TA_CRYPT_UUID;
const TEEC_UUID os_test_ta_uuid = TA_OS_TEST_UUID;
const TEEC_UUID create_fail_test_ta_uuid = TA_CREATE_FAIL_TEST_UUID;
const TEEC_UUID ecc_test_ta_uuid = ECC_SELF_TEST_UUID;
const TEEC_UUID sta_test_ta_uuid = STA_SELF_TEST_UUID;
const TEEC_UUID rpc_test_ta_uuid = TA_RPC_TEST_UUID;
const TEEC_UUID sims_test_ta_uuid = TA_SIMS_TEST_UUID;
const TEEC_UUID storage_ta_uuid = TA_STORAGE_UUID;
const TEEC_UUID storage2_ta_uuid = TA_STORAGE2_UUID;
const TEEC_UUID enc_fs_key_manager_test_ta_uuid = ENC_FS_KEY_MANAGER_TEST_UUID;
const TEEC_UUID concurrent_ta_uuid = TA_CONCURRENT_UUID;
const TEEC_UUID concurrent_large_ta_uuid = TA_CONCURRENT_LARGE_UUID;
const TEEC_UUID storage_benchmark_ta_uuid = TA_STORAGE_BENCHMARK_UUID;
const TEEC_UUID ocram_test_ta_uuid = TA_TEST_OCRAM_UUID;
const TEEC_UUID snvs_test_ta_uuid = TA_TIME_UUID;
const TEEC_UUID ocotp_test_ta_uuid = TA_OCOTP_UUID;
#ifdef WITH_GP_TESTS
const TEEC_UUID gp_tta_ds_uuid = TA_TTA_DS_UUID;
#endif
