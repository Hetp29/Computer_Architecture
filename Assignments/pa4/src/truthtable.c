#include <stdio.h>
#include <stdlib.h>

typedef enum {
    AND, OR, NAND, NOR, XOR, NOT, PASS, DECODER, MULTIPLEXER
} GateType;

typedef struct {
    GateType type;
    int size;
    int *params;
} Gate;

typedef struct {
    int num_inputs;
    int num_objects;
    int num_vars;
    int *input_vars;
    int *output_vars;
    Gate *gates;
    int num_gates;
} Circuit;