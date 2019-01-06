#include <ai.h>
#include <bsp_ai.h>
#include "ai_platform.h"
#include "math.h"

#define AI_BUFFER_NULL(ptr_)  \
  AI_BUFFER_OBJ_INIT( \
    AI_BUFFER_FORMAT_NONE|AI_BUFFER_FMT_FLAG_CONST, \
    0, 0, 0, 0, \
    AI_HANDLE_PTR(ptr_))

static ai_u8 activations[AI_MNETWORK_DATA_ACTIVATIONS_SIZE];
ai_handle handle;
ai_network_report report;

// Standard normalization
void normalize(ai_float *in_data, ai_float *normalized_data) {

	float mean = 0.0;
	float std = 0.0;

	// Sum and mean
	for (int i = 0; i < AI_MNETWORK_IN_1_SIZE; i++) {
		mean += in_data[i];
	}
	mean = mean / AI_MNETWORK_IN_1_SIZE;

	// Standard deviation
	for (int i = 0; i < AI_MNETWORK_IN_1_SIZE; i++) {
		std += pow(in_data[i] - mean, 2);
	}
	std = sqrt(std / AI_MNETWORK_IN_1_SIZE);

	// Normalization
	for (int i = 0; i < AI_MNETWORK_IN_1_SIZE; i++) {
		normalized_data[i] = (in_data[i] - mean) / std;
	}

}

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

	return 0;
}

int ai_infer(ai_float *input_data, const char *label) {

	ai_buffer ai_input[1];
	ai_buffer ai_output[1];
	ai_float* output_;
	ai_float normalized_data[AI_MNETWORK_IN_1_SIZE];
	ai_float output_data[AI_MNETWORK_OUT_1_SIZE];

	// Normalize the input data
	normalize(input_data, normalized_data);

	ai_input[0] = report.inputs;
	ai_output[0] = report.outputs;

	ai_input[0].n_batches = 1;
	ai_input[0].data = AI_HANDLE_PTR(normalized_data);
	ai_output[0].n_batches = 1;
	ai_output[0].data = AI_HANDLE_PTR(output_data);

	ai_mnetwork_run(handle, &ai_input[0], &ai_output[0]);

	output_ = (ai_float *) (ai_output[0].data);
	if (label) {
		printf("[Inference] input: \"%s\",  result: [%d%%, %d%%, %d%%]\n",
				label, (int) (output_[0] * 100), (int) (output_[1] * 100),
				(int) (output_[2] * 100));
	} else {
		printf(
				"\n--- Inference ---\n rock:        %3d%%\n paper:       %3d%%\n scissors:    %3d%%\n",
				(int) (output_[1] * 100), (int) (output_[0] * 100),
				(int) (output_[2] * 100));
	}
	return 0;
}
