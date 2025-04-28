#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Aktiveringsfunktion (sigmoid)
double sigmoid(double x)
{
    return 1.0 / (1.0 + exp(-x));
}

// Derivata av sigmoid
double sigmoid_derivative(double x)
{
    return x * (1.0 - x);
}

// Struktur för neuralt nätverk
typedef struct
{
    double *input_layer;
    double *hidden_layer;
    double *output_layer;
    double **weights_input_hidden;
    double **weights_hidden_output;
    int input_size;
    int hidden_size;
    int output_size;
} NeuralNetwork;

// Initiera neuralt nätverk
NeuralNetwork *create_network(int input_size, int hidden_size, int output_size)
{
    NeuralNetwork *network = (NeuralNetwork *)malloc(sizeof(NeuralNetwork));

    network->input_size = input_size;
    network->hidden_size = hidden_size;
    network->output_size = output_size;

    // Allokera minne för lager
    network->input_layer = (double *)malloc(input_size * sizeof(double));
    network->hidden_layer = (double *)malloc(hidden_size * sizeof(double));
    network->output_layer = (double *)malloc(output_size * sizeof(double));

    // Allokera minne för vikter
    network->weights_input_hidden = (double **)malloc(input_size * sizeof(double *));
    network->weights_hidden_output = (double **)malloc(hidden_size * sizeof(double *));

    // Initiera vikter med slumpmässiga värden
    srand(time(NULL));
    for (int i = 0; i < input_size; i++)
    {
        network->weights_input_hidden[i] = (double *)malloc(hidden_size * sizeof(double));
        for (int j = 0; j < hidden_size; j++)
        {
            network->weights_input_hidden[i][j] = ((double)rand() / RAND_MAX) * 2 - 1;
        }
    }

    for (int i = 0; i < hidden_size; i++)
    {
        network->weights_hidden_output[i] = (double *)malloc(output_size * sizeof(double));
        for (int j = 0; j < output_size; j++)
        {
            network->weights_hidden_output[i][j] = ((double)rand() / RAND_MAX) * 2 - 1;
        }
    }

    return network;
}

// Feedforward
void feedforward(NeuralNetwork *network, double *input)
{
    // Kopiera input till input_layer
    for (int i = 0; i < network->input_size; i++)
    {
        network->input_layer[i] = input[i];
    }

    // Beräkna hidden layer
    for (int i = 0; i < network->hidden_size; i++)
    {
        double sum = 0;
        for (int j = 0; j < network->input_size; j++)
        {
            sum += network->input_layer[j] * network->weights_input_hidden[j][i];
        }
        network->hidden_layer[i] = sigmoid(sum);
    }

    // Beräkna output layer
    for (int i = 0; i < network->output_size; i++)
    {
        double sum = 0;
        for (int j = 0; j < network->hidden_size; j++)
        {
            sum += network->hidden_layer[j] * network->weights_hidden_output[j][i];
        }
        network->output_layer[i] = sigmoid(sum);
    }
}

// Backpropagation
void backpropagation(NeuralNetwork *network, double *target, double learning_rate)
{
    // Beräkna fel för output layer
    double *output_errors = (double *)malloc(network->output_size * sizeof(double));
    for (int i = 0; i < network->output_size; i++)
    {
        output_errors[i] = target[i] - network->output_layer[i];
    }

    // Beräkna fel för hidden layer
    double *hidden_errors = (double *)malloc(network->hidden_size * sizeof(double));
    for (int i = 0; i < network->hidden_size; i++)
    {
        double error = 0;
        for (int j = 0; j < network->output_size; j++)
        {
            error += output_errors[j] * network->weights_hidden_output[i][j];
        }
        hidden_errors[i] = error;
    }

    // Uppdatera vikter mellan hidden och output
    for (int i = 0; i < network->hidden_size; i++)
    {
        for (int j = 0; j < network->output_size; j++)
        {
            double delta = output_errors[j] * sigmoid_derivative(network->output_layer[j]);
            network->weights_hidden_output[i][j] += learning_rate * network->hidden_layer[i] * delta;
        }
    }

    // Uppdatera vikter mellan input och hidden
    for (int i = 0; i < network->input_size; i++)
    {
        for (int j = 0; j < network->hidden_size; j++)
        {
            double delta = hidden_errors[j] * sigmoid_derivative(network->hidden_layer[j]);
            network->weights_input_hidden[i][j] += learning_rate * network->input_layer[i] * delta;
        }
    }

    free(output_errors);
    free(hidden_errors);
}

// Träna nätverket
void train(NeuralNetwork *network, double **inputs, double **targets, int num_samples, int epochs, double learning_rate)
{
    for (int epoch = 0; epoch < epochs; epoch++)
    {
        for (int sample = 0; sample < num_samples; sample++)
        {
            feedforward(network, inputs[sample]);
            backpropagation(network, targets[sample], learning_rate);
        }
    }
}

// Frigör minne
void free_network(NeuralNetwork *network)
{
    free(network->input_layer);
    free(network->hidden_layer);
    free(network->output_layer);

    for (int i = 0; i < network->input_size; i++)
    {
        free(network->weights_input_hidden[i]);
    }
    free(network->weights_input_hidden);

    for (int i = 0; i < network->hidden_size; i++)
    {
        free(network->weights_hidden_output[i]);
    }
    free(network->weights_hidden_output);

    free(network);
}

int main()
{
    // Skapa ett enkelt XOR-problem
    double inputs[4][2] = {
        {0, 0},
        {0, 1},
        {1, 0},
        {1, 1}};

    double targets[4][1] = {
        {0},
        {1},
        {1},
        {0}};

    // Skapa nätverk
    NeuralNetwork *network = create_network(2, 4, 1);

    // Träna nätverket
    train(network, (double **)inputs, (double **)targets, 4, 10000, 0.1);

    // Testa nätverket
    printf("XOR Problem Test:\n");
    for (int i = 0; i < 4; i++)
    {
        feedforward(network, inputs[i]);
        printf("Input: [%f, %f] -> Output: %f (Expected: %f)\n",
               inputs[i][0], inputs[i][1],
               network->output_layer[0],
               targets[i][0]);
    }

    // Frigör minne
    free_network(network);

    return 0;
}