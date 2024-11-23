#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_VAR_NAME 17
#define MAX_VARS 1024
#define MAX_GATES 1024

// Gate types
typedef enum {
    AND, OR, NAND, NOR, XOR, NOT, PASS, DECODER, MULTIPLEXER
} GateType;

// Variable structure
typedef struct {
    char name[MAX_VAR_NAME];
    int index;
    bool is_input;
    bool is_output;
} Variable;

// Gate structure
typedef struct {
    GateType type;
    int size;           // For DECODER and MULTIPLEXER
    int* inputs;        // Array of input variable indices
    int* outputs;       // Array of output variable indices
    int num_inputs;
    int num_outputs;
} Gate;

// Circuit structure
typedef struct {
    Variable vars[MAX_VARS];
    int num_vars;
    int num_inputs;
    int num_outputs;
    Gate gates[MAX_GATES];
    int num_gates;
    bool* values;       // Current values of all variables
} Circuit;

// Function prototypes
void init_circuit(Circuit* c);
int find_or_add_var(Circuit* c, const char* name);
void parse_input(Circuit* c, FILE* fp);
void parse_output(Circuit* c, FILE* fp);
void parse_gate(Circuit* c, FILE* fp, GateType type);
void evaluate_gate(Circuit* c, Gate* g);
void generate_truth_table(Circuit* c);

// Initialize circuit
void init_circuit(Circuit* c) {
    c->num_vars = 0;
    c->num_inputs = 0;
    c->num_outputs = 0;
    c->num_gates = 0;
    c->values = (bool*)calloc(MAX_VARS, sizeof(bool));
}

// Find or add variable to circuit
int find_or_add_var(Circuit* c, const char* name) {
    // Check if variable exists
    for (int i = 0; i < c->num_vars; i++) {
        if (strcmp(c->vars[i].name, name) == 0) {
            return i;
        }
    }
    
    // Add new variable
    strcpy(c->vars[c->num_vars].name, name);
    c->vars[c->num_vars].index = c->num_vars;
    c->vars[c->num_vars].is_input = false;
    c->vars[c->num_vars].is_output = false;
    return c->num_vars++;
}

// Parse INPUT directive
void parse_input(Circuit* c, FILE* fp) {
    int n;
    fscanf(fp, " %d", &n);
    c->num_inputs = n;
    
    char var_name[MAX_VAR_NAME];
    for (int i = 0; i < n; i++) {
        fscanf(fp, " %16s", var_name);
        int idx = find_or_add_var(c, var_name);
        c->vars[idx].is_input = true;
    }
}

// Parse OUTPUT directive
void parse_output(Circuit* c, FILE* fp) {
    int n;
    fscanf(fp, " %d", &n);
    c->num_outputs = n;
    
    char var_name[MAX_VAR_NAME];
    for (int i = 0; i < n; i++) {
        fscanf(fp, " %16s", var_name);
        int idx = find_or_add_var(c, var_name);
        c->vars[idx].is_output = true;
    }
}

// Evaluate a single gate
void evaluate_gate(Circuit* c, Gate* g) {
    switch (g->type) {
        case AND:
            c->values[g->outputs[0]] = c->values[g->inputs[0]] && c->values[g->inputs[1]];
            break;
        case OR:
            c->values[g->outputs[0]] = c->values[g->inputs[0]] || c->values[g->inputs[1]];
            break;
        case NAND:
            c->values[g->outputs[0]] = !(c->values[g->inputs[0]] && c->values[g->inputs[1]]);
            break;
        case NOR:
            c->values[g->outputs[0]] = !(c->values[g->inputs[0]] || c->values[g->inputs[1]]);
            break;
        case XOR:
            c->values[g->outputs[0]] = c->values[g->inputs[0]] ^ c->values[g->inputs[1]];
            break;
        case NOT:
            c->values[g->outputs[0]] = !c->values[g->inputs[0]];
            break;
        case PASS:
            c->values[g->outputs[0]] = c->values[g->inputs[0]];
            break;
        case DECODER: {
            int select = 0;
            for (int i = 0; i < g->size; i++) {
                if (c->values[g->inputs[i]]) {
                    select |= (1 << (g->size - i - 1));
                }
            }
            for (int i = 0; i < (1 << g->size); i++) {
                c->values[g->outputs[i]] = (i == select);
            }
            break;
        }
        case MULTIPLEXER: {
            int select = 0;
            for (int i = 0; i < g->size; i++) {
                if (c->values[g->inputs[(1 << g->size) + i]]) {
                    select |= (1 << (g->size - i - 1));
                }
            }
            c->values[g->outputs[0]] = c->values[g->inputs[select]];
            break;
        }
    }
}

// Parse a gate directive
void parse_gate(Circuit* c, FILE* fp, GateType type) {
    Gate* g = &c->gates[c->num_gates++];
    g->type = type;
    
    char var_name[MAX_VAR_NAME];
    
    switch (type) {
        case NOT:
        case PASS:
            g->num_inputs = 1;
            g->num_outputs = 1;
            break;
        case AND:
        case OR:
        case NAND:
        case NOR:
        case XOR:
            g->num_inputs = 2;
            g->num_outputs = 1;
            break;
        case DECODER:
            fscanf(fp, " %d", &g->size);
            g->num_inputs = g->size;
            g->num_outputs = 1 << g->size;
            break;
        case MULTIPLEXER:
            fscanf(fp, " %d", &g->size);
            g->num_inputs = (1 << g->size) + g->size;
            g->num_outputs = 1;
            break;
    }
    
    g->inputs = (int*)malloc(g->num_inputs * sizeof(int));
    g->outputs = (int*)malloc(g->num_outputs * sizeof(int));
    
    // Read inputs
    for (int i = 0; i < g->num_inputs; i++) {
        fscanf(fp, " %16s", var_name);
        if (strcmp(var_name, "0") == 0) {
            g->inputs[i] = -1;  // Represent constant 0
        } else if (strcmp(var_name, "1") == 0) {
            g->inputs[i] = -2;  // Represent constant 1
        } else {
            g->inputs[i] = find_or_add_var(c, var_name);
        }
    }
    
    // Read outputs
    for (int i = 0; i < g->num_outputs; i++) {
        fscanf(fp, " %16s", var_name);
        if (strcmp(var_name, "_") == 0) {
            g->outputs[i] = -1;  // Represent discarded output
        } else {
            g->outputs[i] = find_or_add_var(c, var_name);
        }
    }
}

void generate_truth_table(Circuit* c) {
    int num_rows = 1 << c->num_inputs;

    for (int row = 0; row < num_rows; row++) {
        // Set input values
        for (int i = 0; i < c->num_inputs; i++) {
            c->values[c->vars[i].index] = (row & (1 << (c->num_inputs - i - 1))) != 0;
        }

        // Evaluate all gates
        for (int i = 0; i < c->num_gates; i++) {
            evaluate_gate(c, &c->gates[i]);
        }

        // Print input values
        for (int i = 0; i < c->num_inputs; i++) {
            printf("%d", c->values[c->vars[i].index]);
            if (i < c->num_inputs - 1) printf(" ");
        }

        printf(" |");

        // Print output values
        for (int i = 0; i < c->num_outputs; i++) {
            int output_index = c->vars[c->num_vars - c->num_outputs + i].index;
            printf(" %d", c->values[output_index]);
        }
        printf("\n");
    }
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <circuit_file>\n", argv[0]);
        return 1;
    }
    
    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        fprintf(stderr, "Error: Could not open file %s\n", argv[1]);
        return 1;
    }
    
    Circuit circuit;
    init_circuit(&circuit);
    
    char directive[MAX_VAR_NAME];
    while (fscanf(fp, " %16s", directive) == 1) {
        if (strcmp(directive, "INPUT") == 0) {
            parse_input(&circuit, fp);
        } else if (strcmp(directive, "OUTPUT") == 0) {
            parse_output(&circuit, fp);
        } else if (strcmp(directive, "AND") == 0) {
            parse_gate(&circuit, fp, AND);
        } else if (strcmp(directive, "OR") == 0) {
            parse_gate(&circuit, fp, OR);
        } else if (strcmp(directive, "NOT") == 0) {
            parse_gate(&circuit, fp, NOT);
        } else if (strcmp(directive, "NAND") == 0) {
            parse_gate(&circuit, fp, NAND);
        } else if (strcmp(directive, "NOR") == 0) {
            parse_gate(&circuit, fp, NOR);
        } else if (strcmp(directive, "XOR") == 0) {
            parse_gate(&circuit, fp, XOR);
        } else if (strcmp(directive, "DECODER") == 0) {
            parse_gate(&circuit, fp, DECODER);
        } else if (strcmp(directive, "MULTIPLEXER") == 0) {
            parse_gate(&circuit, fp, MULTIPLEXER);
        } else if (strcmp(directive, "PASS") == 0) {
            parse_gate(&circuit, fp, PASS);
        }
    }
    
    generate_truth_table(&circuit);
    
    // Cleanup
    free(circuit.values);
    for (int i = 0; i < circuit.num_gates; i++) {
        free(circuit.gates[i].inputs);
        free(circuit.gates[i].outputs);
    }
    
    fclose(fp);
    return 0;
}