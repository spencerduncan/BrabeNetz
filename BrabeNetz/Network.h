#pragma once
#include "NetworkTopology.h"
using namespace std;

// Specifies the value each weight gets adjusted by on training
#define ADJUST_WEIGHTS_BY 0.002
// Maximum delta of actual and expected values until stop training
#define MAX_DELTA 0.01
// Learning rate for the backpropagation weights/bias adjusting
#define LEARNING_RATE 0.01
// Filename for the state file
#define STATE_FILE "state.nn"

class network
{
public:
	////////////////
	//    ctor    //
	////////////////
	// initializerList: { 2, 3, 4, 1 }: 2 Input, 3 Hidden, 4 Hidden, 1 Output
	network(initializer_list<int> initializer_list);
	// initializerList: { 2, 3, 4, 1 }: 2 Input, 3 Hidden, 4 Hidden, 1 Output
	explicit network(network_topology& topology);
	// initializerList: { 2, 3, 4, 1 }: 2 Input, 3 Hidden, 4 Hidden, 1 Output
	explicit network(string state_path);
	~network();

	////////////////
	// functions  //
	////////////////
	// Feed the network information and train it to adjust to the expected output (returns output error)
	double train(double* input_values, int length, double* expected_output) const;
	// Feed the network information and return the output layer with it's length "out_length"
	double* feed(double* input_values, int length, int& out_length) const;
	// Save the network's state to disk by serializing weights
	void save(string path = STATE_FILE);
	// Set the network's learning rate (should be 1/i, where i = train iterations so far)
	void set_learnrate(double value);
private:
	////////////////
	// properties //
	////////////////
	// Learning rate of this network (1/i, where i = train iterations so far)
	double learn_rate_ = LEARNING_RATE;
	// Count of Layers in this network
	int layers_count_;
	// Count of Neurons in Layers (Left to right in Topology)
	int* neurons_count_;
	// Actual layers that contain nodes with their values
	double** layers_;
	// Each neuron's bias
	double** biases_;
	// Each neuron's error values
	double** errors_;
	// Weight of each neuron's connection, 3D Array: [layer][neuron][connection]
	double*** weights_;
	// The network topology, only for logic representation and weights initialization
	network_topology topology_;


	////////////////
	// functions  //
	////////////////
	// Init Network
	void init(network_topology& topology);
	// Load the network's state from disk by deserializing and loading weights/biases
	void load(string path = STATE_FILE);
	// Forwards Propagate given input_values by one layer and return the new output layer with length of out_length
	double* to_next_layer(double* input_values, int input_length, int layer_index, int& out_length) const;
	// Backwards Propagate through the network, adjust weights and biases and return total output error
	double adjust(double* expected_output, double* actual_output, const int length) const;
	// Fill neuron weights with this topology
	void fill_weights();
	// Delete weights array
	void delete_weights() const;
};
