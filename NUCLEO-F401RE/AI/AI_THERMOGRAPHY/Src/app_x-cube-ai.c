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
#include "ai_datatypes_defines.h"

#include <stdlib.h>
#include "main.h"
#include "stm32f4xx_hal.h"
#include "ai.h"
#include "amg8833.h"
#include "lcd.h"
/*************************************************************************
 *
 */
void MX_X_CUBE_AI_Init(void) {
  MX_UARTx_Init();
  /* USER CODE BEGIN 0 */
  ai_init();
  lcd_init(&hi2c1);
  /* USER CODE END 0 */
}

void MX_X_CUBE_AI_Process(void) {
  /* USER CODE BEGIN 1 */

  ai_float in_data[AI_MNETWORK_IN_1_SIZE] = { 0.0f };
  ai_float out_data[NUM_CLASS_LABELS] = { 0.0 };
  uint8_t buffer[AMG8833_PIXEL_DATA_LENGTH] = { 0 };

  char class_labels[NUM_CLASS_LABELS][12] = { "PAPER", "ROCK", "SCISSORS" };
#ifdef GAME_MODE
  int me, you;
  char start_msg[2][16] = { "/// ROCK - PAPER", "- SCISSORS  ///" };
  int game[3][3] = { { 0, 1, -1 }, { -1, 0, 1 }, { 1, -1, 0 } };
  char rock_paper_scissors[3][2][16] = { { "!!!!", "(  // ...PAPER" }, { ".ooo",
      "(__O  ...ROCK" }, { " ii", ".o<  ...SCISSORS" } };
#endif

#ifndef GAME_MODE
  read_registors(AMG8833_T01L_ADDR, buffer, AMG8833_PIXEL_DATA_LENGTH);

  for (int i = 0; i < AI_MNETWORK_IN_1_SIZE; i++) {
    in_data[i] = (ai_float)(buffer[i*2+1]*256 + buffer[i*2]) * 0.25;
  }

  ai_infer(in_data, out_data);

  // Output to console
  printf("\n--- Inference ---\n");
  for (int i=0; i<NUM_CLASS_LABELS; i++) {
    printf(" %-12s%3d%%\n", class_labels[i], (int) (out_data[i] * 100));
  }

#else
  // Show start message
  lcd_clear();
  lcd_string(start_msg[0], sizeof(start_msg[0]));
  lcd_newline();
  lcd_string(start_msg[1], sizeof(start_msg[1]));
  HAL_Delay(2000);

  // Count three
  lcd_clear();
  lcd_string(".", 1);
  HAL_Delay(800);
  lcd_clear();
  lcd_string("..", 2);
  HAL_Delay(800);

  // My hand
  me = rand() % 3;
  printf("\nme: %d\n", me);
  printf("%s\n", rock_paper_scissors[me][0]);
  printf("%s\n", rock_paper_scissors[me][1]);
  lcd_clear();
  lcd_string(rock_paper_scissors[me][0], sizeof(rock_paper_scissors[me][0]));
  lcd_newline();
  lcd_string(rock_paper_scissors[me][1], sizeof(rock_paper_scissors[me][1]));

  HAL_Delay(500);
  // Recognize your hand with AI
  read_registors(AMG8833_T01L_ADDR, buffer, AMG8833_PIXEL_DATA_LENGTH);

  for (int i = 0; i < AI_MNETWORK_IN_1_SIZE; i++) {
    in_data[i] = (ai_float) (buffer[i * 2 + 1] * 256 + buffer[i * 2]) * AMG8833_RESOLUTION;
  }

  ai_infer(in_data, out_data);

  you = 0;
  for (int i = 0; i < NUM_CLASS_LABELS; i++) {
    if (out_data[you] <= out_data[i])
      you = i;
  }

  HAL_Delay(1500);

  lcd_clear();
  printf("\nME: %s\n", class_labels[me]);
  printf("YOU: %s\n", class_labels[you]);
  lcd_string("ME: ", 4);
  lcd_string(class_labels[me], sizeof(class_labels[me]));
  lcd_newline();
  lcd_string("YOU: ", 5);
  lcd_string(class_labels[you], sizeof(class_labels[you]));

  HAL_Delay(2000);

  // Show game result
  lcd_clear();
  switch (game[me][you]) {
  case 0:  // Draw
    lcd_string("*** DRAW *******", 16);
    break;
  case 1:  // Win
    lcd_string("*** I WIN! *****", 16);
    break;
  case -1: // Lose
    lcd_string("*** YOU WIN! ***", 16);
    break;
  }
  lcd_newline();
  lcd_string("****************", 16);
  HAL_Delay(3000);

#endif

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
        ai_network_data_weights_get, .params = { AI_NETWORK_DATA_WEIGHTS(0),
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
