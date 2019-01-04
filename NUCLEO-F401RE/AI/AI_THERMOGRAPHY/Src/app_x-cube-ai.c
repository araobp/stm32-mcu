#ifdef __cplusplus
extern "C" {
#endif
/**
 ******************************************************************************
 * @file           : app_x-cube-ai.c
 * @brief          : AI program body
 ******************************************************************************
 * This notice applies to any and all portions of this file
 * that are not between comment pairs USER CODE BEGIN and
 * USER CODE END. Other portions of this file, whether
 * inserted by the user or by software development tools
 * are owned by their respective copyright owners.
 *
 * Copyright (c) 2018 STMicroelectronics International N.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted, provided that the following conditions are met:
 *
 * 1. Redistribution of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of other
 *    contributors to this software may be used to endorse or promote products
 *    derived from this software without specific written permission.
 * 4. This software, including modifications and/or derivative works of this
 *    software, must execute solely and exclusively on microcontroller or
 *    microprocessor devices manufactured by or for STMicroelectronics.
 * 5. Redistribution and use of this software other than as permitted under
 *    this license is void and will automatically terminate your rights under
 *    this license.
 *
 * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
 * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
 * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "app_x-cube-ai.h"
#include "bsp_ai.h"
//#include "aiSystemPerformance.h"
#include "ai_datatypes_defines.h"

/* USER CODE BEGIN Includes */
#include "ai.h"
/* USER CODE END Includes */

/*************************************************************************
 *
 */
void MX_X_CUBE_AI_Init(void) {
	MX_UARTx_Init();
	ai_init();
	/* USER CODE BEGIN 0 */
	/* USER CODE END 0 */
}

void MX_X_CUBE_AI_Process(void) {
	//aiSystemPerformanceProcess();
	/* USER CODE BEGIN 1 */
	static ai_float in_data[AI_MNETWORK_IN_1_SIZE] = { 17.8, 17.8, 17.8, 17.8,
			17.7, 17.8, 17.8, 17.8, 17.8, 17.7, 17.7, 17.7, 17.6, 17.5, 17.4,
			17.3, 17.2, 17.2, 17.3, 17.5, 17.7, 18.0, 18.2, 18.3, 18.3, 18.2,
			18.1, 18.0, 17.8, 17.7, 17.5, 17.2, 17.9, 17.9, 18.0, 18.0, 18.0,
			18.0, 18.0, 18.0, 18.0, 18.0, 17.9, 17.8, 17.8, 17.7, 17.7, 17.6,
			17.5, 17.5, 17.4, 17.5, 17.7, 17.9, 18.1, 18.1, 18.1, 18.0, 18.1,
			18.1, 18.0, 17.8, 17.6, 17.4, 17.9, 18.0, 18.1, 18.2, 18.2, 18.2,
			18.3, 18.3, 18.3, 18.3, 18.2, 18.0, 18.0, 18.0, 18.0, 17.9, 17.9,
			17.7, 17.7, 17.8, 17.8, 17.9, 18.1, 18.0, 18.0, 18.1, 18.3, 18.2,
			18.1, 18.0, 17.8, 17.6, 18.0, 18.1, 18.2, 18.3, 18.4, 18.4, 18.4,
			18.5, 18.6, 18.6, 18.6, 18.5, 18.4, 18.4, 18.5, 18.5, 18.4, 18.2,
			18.1, 18.2, 18.3, 18.2, 18.2, 18.1, 18.2, 18.3, 18.5, 18.4, 18.3,
			18.1, 17.9, 17.8, 18.0, 18.1, 18.2, 18.4, 18.5, 18.5, 18.6, 18.7,
			18.8, 18.9, 18.9, 19.0, 19.0, 19.1, 19.2, 19.2, 19.2, 19.1, 19.0,
			18.9, 18.9, 18.8, 18.7, 18.6, 18.6, 18.7, 18.7, 18.6, 18.5, 18.3,
			18.1, 17.9, 18.0, 18.1, 18.2, 18.4, 18.5, 18.5, 18.6, 18.8, 19.0,
			19.1, 19.3, 19.5, 19.7, 20.0, 20.3, 20.4, 20.4, 20.4, 20.3, 20.2,
			20.1, 19.9, 19.6, 19.3, 19.1, 19.0, 18.9, 18.7, 18.5, 18.3, 18.1,
			18.1, 17.9, 18.1, 18.2, 18.4, 18.4, 18.5, 18.6, 18.8, 19.0, 19.4,
			19.7, 20.0, 20.5, 21.3, 21.7, 22.1, 22.2, 22.2, 22.1, 22.0, 21.9,
			21.4, 20.9, 20.2, 19.6, 19.3, 19.1, 18.8, 18.5, 18.3, 18.2, 18.4,
			17.9, 18.0, 18.2, 18.3, 18.4, 18.5, 18.7, 18.9, 19.2, 19.7, 20.2,
			20.8, 21.8, 22.8, 23.4, 23.8, 24.1, 24.2, 24.1, 24.1, 23.6, 23.0,
			22.4, 21.6, 20.5, 19.8, 19.4, 18.9, 18.5, 18.4, 18.5, 18.6, 17.9,
			18.1, 18.2, 18.3, 18.5, 18.7, 18.9, 19.2, 19.7, 20.3, 21.1, 22.1,
			23.3, 24.3, 25.0, 25.6, 25.9, 26.1, 26.1, 25.9, 25.4, 24.7, 23.9,
			23.0, 21.8, 20.7, 19.8, 19.2, 18.8, 18.7, 18.7, 18.9, 18.0, 18.2,
			18.3, 18.5, 18.7, 19.0, 19.4, 19.9, 20.5, 21.3, 22.3, 23.4, 24.6,
			25.7, 26.5, 27.1, 27.5, 27.7, 27.7, 27.4, 26.9, 26.2, 25.3, 24.3,
			23.0, 21.7, 20.6, 19.8, 19.3, 19.1, 19.0, 19.0, 18.2, 18.4, 18.6,
			18.8, 19.1, 19.6, 20.1, 20.8, 21.6, 22.7, 23.4, 24.6, 25.8, 26.8,
			27.6, 28.1, 28.5, 28.7, 28.6, 28.3, 27.8, 27.1, 26.5, 25.5, 24.1,
			22.7, 21.5, 20.6, 19.8, 19.4, 19.2, 19.0, 18.5, 18.7, 19.0, 19.3,
			19.7, 20.3, 21.1, 21.9, 23.4, 24.5, 25.1, 25.7, 26.8, 27.8, 28.4,
			28.9, 29.2, 29.3, 29.1, 28.8, 28.4, 28.0, 27.6, 26.6, 25.2, 23.9,
			22.6, 21.5, 20.4, 19.7, 19.3, 19.0, 18.8, 19.2, 19.5, 20.0, 20.6,
			21.3, 22.2, 23.7, 25.2, 26.3, 26.9, 27.2, 27.7, 28.5, 29.0, 29.3,
			29.4, 29.4, 29.3, 29.1, 29.0, 28.9, 28.5, 27.5, 26.3, 25.0, 23.6,
			22.3, 21.2, 20.2, 19.5, 19.0, 19.2, 19.6, 20.2, 20.8, 21.5, 22.5,
			23.9, 25.4, 26.7, 27.8, 28.4, 28.7, 28.8, 29.0, 29.3, 29.4, 29.4,
			29.4, 29.5, 29.6, 29.6, 29.6, 29.2, 28.3, 27.2, 25.8, 24.4, 23.0,
			21.8, 20.8, 19.8, 19.0, 19.5, 20.2, 20.9, 21.7, 22.7, 23.6, 24.9,
			26.3, 27.7, 28.7, 29.1, 29.2, 29.2, 29.4, 29.6, 29.7, 29.7, 29.6,
			29.7, 29.9, 30.1, 30.1, 29.6, 28.7, 27.5, 26.2, 24.8, 23.5, 22.2,
			21.1, 20.0, 19.1, 19.9, 20.8, 21.7, 22.8, 24.2, 25.1, 25.7, 26.8,
			28.1, 29.0, 29.2, 29.2, 29.3, 29.5, 29.7, 29.9, 29.9, 29.8, 29.9,
			30.2, 30.4, 30.3, 29.8, 28.8, 27.6, 26.4, 25.1, 23.8, 22.6, 21.3,
			20.2, 19.2, 20.3, 21.4, 22.7, 24.3, 25.8, 26.6, 27.0, 27.3, 28.3,
			29.0, 29.1, 29.2, 29.3, 29.5, 29.7, 29.9, 30.0, 30.0, 30.1, 30.4,
			30.5, 30.3, 29.8, 28.8, 27.7, 26.6, 25.3, 24.0, 22.7, 21.5, 20.3,
			19.3, 20.8, 22.3, 24.1, 25.8, 27.1, 28.0, 28.4, 28.5, 28.5, 29.0,
			29.1, 29.2, 29.3, 29.5, 29.6, 29.8, 30.0, 30.1, 30.3, 30.4, 30.4,
			30.2, 29.7, 28.8, 27.7, 26.5, 25.2, 23.9, 22.7, 21.6, 20.5, 19.4,
			21.4, 23.2, 25.0, 26.7, 28.0, 28.8, 29.1, 29.1, 29.0, 29.1, 29.2,
			29.3, 29.4, 29.5, 29.6, 29.8, 30.0, 30.1, 30.3, 30.4, 30.4, 30.2,
			29.6, 28.7, 27.5, 26.2, 24.9, 23.6, 22.5, 21.5, 20.5, 19.5, 22.1,
			23.7, 25.3, 27.0, 28.4, 29.2, 29.3, 29.2, 29.2, 29.2, 29.4, 29.5,
			29.5, 29.6, 29.8, 29.9, 30.1, 30.2, 30.3, 30.5, 30.4, 30.2, 29.6,
			28.6, 27.3, 26.0, 24.5, 23.1, 22.2, 21.3, 20.4, 19.6, 22.8, 24.3,
			25.7, 27.1, 28.5, 29.2, 29.2, 29.4, 29.4, 29.4, 29.5, 29.7, 29.8,
			29.8, 29.9, 30.1, 30.2, 30.1, 30.2, 30.4, 30.5, 30.1, 29.5, 28.4,
			27.2, 25.6, 23.8, 22.5, 21.7, 21.1, 20.4, 19.7, 23.2, 24.8, 26.2,
			27.4, 28.5, 29.2, 29.5, 29.6, 29.6, 29.6, 29.6, 29.8, 30.0, 30.0,
			30.1, 30.2, 30.2, 30.1, 30.1, 30.2, 30.3, 30.0, 29.3, 28.2, 26.7,
			24.8, 23.0, 21.7, 21.0, 20.6, 20.2, 19.8, 23.5, 25.0, 26.4, 27.7,
			28.6, 29.3, 29.7, 29.8, 29.8, 29.7, 29.8, 29.9, 30.1, 30.2, 30.3,
			30.2, 30.1, 30.0, 30.0, 30.0, 30.0, 29.7, 29.0, 27.7, 25.9, 24.1,
			22.4, 21.1, 20.4, 20.0, 19.9, 19.8, 23.4, 25.1, 26.5, 27.8, 28.8,
			29.5, 29.8, 29.9, 29.9, 29.9, 29.9, 30.1, 30.2, 30.3, 30.3, 30.2,
			30.1, 30.0, 30.0, 29.9, 29.8, 29.3, 28.4, 27.2, 25.5, 23.6, 21.9,
			20.7, 20.1, 19.9, 19.8, 19.7, 23.1, 25.0, 26.6, 27.9, 29.0, 29.7,
			30.0, 30.2, 30.1, 30.0, 30.0, 30.1, 30.2, 30.3, 30.3, 30.2, 30.1,
			30.1, 29.9, 29.8, 29.5, 28.7, 27.6, 26.3, 25.0, 23.4, 21.6, 20.6,
			20.1, 20.0, 19.7, 19.7, 22.7, 24.6, 26.5, 27.9, 29.0, 29.8, 30.2,
			30.3, 30.1, 30.0, 30.1, 30.1, 30.2, 30.3, 30.3, 30.2, 30.2, 30.1,
			29.8, 29.5, 28.8, 27.8, 26.6, 25.4, 24.1, 22.9, 21.4, 20.5, 20.1,
			19.8, 19.7, 19.7, 22.2, 24.2, 26.1, 27.7, 29.0, 29.7, 30.1, 30.2,
			30.1, 30.0, 30.0, 30.1, 30.2, 30.3, 30.3, 30.3, 30.3, 30.0, 29.6,
			28.9, 27.9, 26.8, 25.7, 24.5, 23.2, 22.1, 21.2, 20.4, 20.0, 19.8,
			19.7, 19.7, 21.9, 23.9, 25.8, 27.5, 28.7, 29.6, 30.0, 30.1, 30.0,
			30.0, 30.0, 30.1, 30.1, 30.2, 30.3, 30.2, 30.1, 29.7, 29.1, 28.2,
			27.2, 26.0, 24.8, 23.7, 22.6, 21.6, 20.7, 20.2, 19.8, 19.7, 19.7,
			19.8, 21.7, 23.7, 25.6, 27.2, 28.3, 29.2, 29.8, 30.0, 29.9, 29.9,
			30.0, 30.0, 30.1, 30.1, 30.1, 30.1, 29.9, 29.4, 28.7, 27.6, 26.6,
			25.3, 24.0, 22.9, 21.9, 21.1, 20.5, 20.1, 19.7, 19.6, 19.7, 19.8,
			21.5, 23.5, 25.2, 26.6, 27.8, 28.7, 29.4, 29.7, 29.8, 29.8, 29.9,
			30.0, 30.0, 30.0, 30.0, 29.9, 29.6, 29.2, 28.4, 27.1, 25.9, 24.6,
			23.3, 22.3, 21.3, 20.6, 20.3, 20.0, 19.8, 19.7, 19.7, 19.8, 21.4,
			23.2, 24.7, 26.0, 27.2, 28.1, 28.8, 29.3, 29.6, 29.7, 29.8, 29.9,
			29.9, 29.9, 29.9, 29.7, 29.6, 29.1, 28.2, 26.7, 25.2, 23.9, 22.7,
			21.6, 20.8, 20.4, 20.1, 20.0, 19.9, 19.8, 19.8, 19.9, 21.2, 22.8,
			24.2, 25.5, 26.6, 27.5, 28.3, 28.9, 29.3, 29.5, 29.6, 29.7, 29.7,
			29.7, 29.7, 29.7, 29.4, 28.9, 27.9, 26.5, 24.9, 23.3, 21.9, 21.0,
			20.4, 20.1, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0 };
	ai_infer(in_data);
	HAL_Delay(1000); /* delay 1s */
	/* USER CODE END 1 */
}

/* Multiple network support --------------------------------------------------*/

#include <string.h>
#include "ai_datatypes_defines.h"

static const ai_network_entry_t networks[AI_MNETWORK_NUMBER] = { { .name =
		(const char *) AI_NETWORK_MODEL_NAME, .config = AI_NETWORK_DATA_CONFIG,
		.ai_get_info = ai_network_get_info, .ai_create = ai_network_create,
		.ai_destroy = ai_network_destroy, .ai_get_error = ai_network_get_error,
		.ai_init = ai_network_init, .ai_run = ai_network_run, .ai_forward =
				ai_network_forward, .ai_data_weights_get_default =
				ai_network_data_weights_get, .params = {
				AI_NETWORK_DATA_WEIGHTS(0),
		AI_NETWORK_DATA_ACTIVATIONS(0) }, }, };

struct network_instance {
	const ai_network_entry_t *entry;
	ai_handle handle;
	ai_network_params params;
};

/* Number of instance is aligned on the number of network */
AI_STATIC struct network_instance gnetworks[AI_MNETWORK_NUMBER] = { 0 };

AI_DECLARE_STATIC
ai_bool ai_mnetwork_is_valid(const char* name, const ai_network_entry_t *entry) {
	if (name && (strlen(entry->name) == strlen(name))
			&& (strncmp(entry->name, name, strlen(entry->name)) == 0))
		return true;
	return false;
}

AI_DECLARE_STATIC
struct network_instance *ai_mnetwork_handle(struct network_instance *inst) {
	for (int i = 0; i < AI_MNETWORK_NUMBER; i++) {
		if ((inst) && (&gnetworks[i] == inst))
			return inst;
		else if ((!inst) && (gnetworks[i].entry == NULL))
			return &gnetworks[i];
	}
	return NULL;
}

AI_DECLARE_STATIC
void ai_mnetwork_release_handle(struct network_instance *inst) {
	for (int i = 0; i < AI_MNETWORK_NUMBER; i++) {
		if ((inst) && (&gnetworks[i] == inst)) {
			gnetworks[i].entry = NULL;
			return;
		}
	}
}

AI_API_ENTRY
const char* ai_mnetwork_find(const char *name, ai_int idx) {
	const ai_network_entry_t *entry;

	for (int i = 0; i < AI_MNETWORK_NUMBER; i++) {
		entry = &networks[i];
		if (ai_mnetwork_is_valid(name, entry))
			return entry->name;
		else {
			if (!idx--)
				return entry->name;
		}
	}
	return NULL;
}

AI_API_ENTRY
ai_error ai_mnetwork_create(const char *name, ai_handle* network,
		const ai_buffer* network_config) {
	const ai_network_entry_t *entry;
	const ai_network_entry_t *found = NULL;
	ai_error err;
	struct network_instance *inst = ai_mnetwork_handle(NULL);

	if (!inst) {
		err.type = AI_ERROR_ALLOCATION_FAILED;
		err.code = AI_ERROR_CODE_NETWORK;
		return err;
	}

	for (int i = 0; i < AI_MNETWORK_NUMBER; i++) {
		entry = &networks[i];
		if (ai_mnetwork_is_valid(name, entry)) {
			found = entry;
			break;
		}
	}

	if (!found) {
		err.type = AI_ERROR_INVALID_PARAM;
		err.code = AI_ERROR_CODE_NETWORK;
		return err;
	}

	if (network_config == NULL)
		err = found->ai_create(network, found->config);
	else
		err = found->ai_create(network, network_config);
	if ((err.code == AI_ERROR_CODE_NONE) && (err.type == AI_ERROR_NONE)) {
		inst->entry = found;
		inst->handle = *network;
		*network = (ai_handle*) inst;
	}

	return err;
}

AI_API_ENTRY
ai_handle ai_mnetwork_destroy(ai_handle network) {
	struct network_instance *inn;
	inn = ai_mnetwork_handle((struct network_instance *) network);
	if (inn) {
		ai_handle hdl = inn->entry->ai_destroy(inn->handle);
		if (hdl != inn->handle) {
			ai_mnetwork_release_handle(inn);
			network = AI_HANDLE_NULL;
		}
	}
	return network;
}

AI_API_ENTRY
ai_bool ai_mnetwork_get_info(ai_handle network, ai_network_report* report) {
	struct network_instance *inn;
	inn = ai_mnetwork_handle((struct network_instance *) network);
	if (inn)
		return inn->entry->ai_get_info(inn->handle, report);
	else
		return false;
}

AI_API_ENTRY
ai_error ai_mnetwork_get_error(ai_handle network) {
	struct network_instance *inn;
	ai_error err;
	err.type = AI_ERROR_INVALID_PARAM;
	err.code = AI_ERROR_CODE_NETWORK;

	inn = ai_mnetwork_handle((struct network_instance *) network);
	if (inn)
		return inn->entry->ai_get_error(inn->handle);
	else
		return err;
}

AI_API_ENTRY
ai_bool ai_mnetwork_init(ai_handle network, const ai_network_params* params) {
	struct network_instance *inn;
	ai_network_params par;

	/* TODO: adding check ai_buffer activations/weights shape coherence */

	inn = ai_mnetwork_handle((struct network_instance *) network);
	if (inn) {
		par = inn->entry->params;
		if (params->activations.n_batches)
			par.activations = params->activations;
		else
			par.activations.data = params->activations.data;
		if (params->params.n_batches)
			par.params = params->params;
		else
			par.params.data = inn->entry->ai_data_weights_get_default();
		return inn->entry->ai_init(inn->handle, &par);
	} else
		return false;
}

AI_API_ENTRY
ai_i32 ai_mnetwork_run(ai_handle network, const ai_buffer* input,
		ai_buffer* output) {
	struct network_instance* inn;
	inn = ai_mnetwork_handle((struct network_instance *) network);
	if (inn)
		return inn->entry->ai_run(inn->handle, input, output);
	else
		return 0;
}

AI_API_ENTRY
ai_i32 ai_mnetwork_forward(ai_handle network, const ai_buffer* input) {
	struct network_instance *inn;
	inn = ai_mnetwork_handle((struct network_instance *) network);
	if (inn)
		return inn->entry->ai_forward(inn->handle, input);
	else
		return 0;
}

AI_API_ENTRY
int ai_mnetwork_get_private_handle(ai_handle network, ai_handle *phandle,
		ai_network_params *pparams) {
	struct network_instance* inn;
	inn = ai_mnetwork_handle((struct network_instance *) network);
	if (inn && phandle && pparams) {
		*phandle = inn->handle;
		*pparams = inn->params;
		return 0;
	} else
		return -1;
}
#ifdef __cplusplus
}
#endif
