/*
 * Description
 *
 * - Apply DCT Type-II to 8x8 heatmap image to extract feature
 * - Remove DC and higher frequency coefficients (to avoid over-fitting in the learning phase)
 * - Run inference
 */

#include <ai.h>
#include <bsp_ai.h>
#include "ai_platform.h"
#include "math.h"
#include "dct.h"

// Data shape
const uint16_t width_cutoff = (int)(sqrt(AI_MNETWORK_IN_1_SIZE));
const uint16_t height_cutoff = (int)(sqrt(AI_MNETWORK_IN_1_SIZE));

#define AI_BUFFER_NULL(ptr_)  \
  AI_BUFFER_OBJ_INIT( \
    AI_BUFFER_FORMAT_NONE|AI_BUFFER_FMT_FLAG_CONST, \
    0, 0, 0, 0, \
    AI_HANDLE_PTR(ptr_))

static ai_u8 activations[AI_MNETWORK_DATA_ACTIVATIONS_SIZE];
ai_handle handle;
ai_network_report report;

ai_buffer ai_input[1];
ai_buffer ai_output[1];
ai_float* output_;
ai_float normalized_data[AI_MNETWORK_IN_1_SIZE];
ai_float feature[WIDTH*HEIGHT];

// DCT Type-II instance
dct2_instance_f32 S;

// Standard normalization
void normalize(ai_float *in_data, ai_float *normalized_data, int len) {

  float mean = 0.0;
  float std = 0.0;

  // Sum and mean
  for (int i = 0; i < len; i++) {
    mean += in_data[i];
  }
  mean = mean / len;

  // Standard deviation
  for (int i = 0; i < len; i++) {
    std += pow(in_data[i] - mean, 2);
  }
  std = sqrt(std / len);

  // Normalization
  for (int i = 0; i < len; i++) {
    normalized_data[i] = (in_data[i] - mean) / std;
  }

}

// Initialize the neural network
int ai_init(void) {

  ai_error err;
  const char *nn_name;
  const ai_network_params params = {
  AI_BUFFER_NULL(NULL),
  AI_BUFFER_NULL(activations) };

  // Find a network
  nn_name = ai_mnetwork_find(NULL, 0);
  if (nn_name) {
    printf("\nFound network: \"%s\"\n", nn_name);
  } else {
    printf("E: ai_mnetwork_find\n");
    return -1;
  }

  // Create the network
  err = ai_mnetwork_create(nn_name, &handle, NULL);
  if (err.type) {
    printf("E: ai_mnetwork_create\n");
    return -1;
  }

  // Initialize the network
  if (!ai_mnetwork_init(handle, &params)) {
    printf("E: ai_mnetwork_init\n");
    return -1;
  }

  dct2_2d_init_f32(&S, HEIGHT, WIDTH);

  return 0;
}

// Run inference
void ai_infer(ai_float *input_data, ai_float *output_data) {

  // DCT Type-II 2D for extracting feature
  dct2_2d_f32(&S, (float32_t *)input_data, (float32_t *)feature, 0);

  // Remove DC and higher frequency coefficients
  feature[0] = 0.0;  // remove DC
  for (int j=0; j<height_cutoff; j++) {
    for (int i=0; i<width_cutoff; i++) {
      feature[j*height_cutoff+i] = feature[j*HEIGHT+i];
    }
  }

  // Normalize the feature data
  normalize(feature, normalized_data, AI_MNETWORK_IN_1_SIZE);

  // Input parameters for running inference
  ai_input[0] = report.inputs;
  ai_output[0] = report.outputs;
  ai_input[0].n_batches = 1;
  ai_input[0].data = AI_HANDLE_PTR(normalized_data);
  ai_output[0].n_batches = 1;
  ai_output[0].data = AI_HANDLE_PTR(output_data);

  // Run inference
  ai_mnetwork_run(handle, &ai_input[0], &ai_output[0]);

  // Copy inference result
  for (int i = 0; i < AI_NETWORK_OUT_1_SIZE; i++) {
    output_data[i] = ((ai_float *) (ai_output[0].data))[i];
  }
}
