#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_VAR_NAME 17
#define MAX_VARS 1024
#define MAX_GATES 1024

typedef enum {
    AND,
    OR,
    NOT,
    NAND,
    NOR,
    XOR,
    DECODER,
    MULTIPLEXER,
    PASS
} GateType;

typedef struct {
    char name[MAX_VAR_NAME];
    int index;
} Variable;

typedef struct {
    int* edges;
    int count;
    int capacity;
} AdjList;

typedef struct {
    AdjList* lists;
    int num_vertices;
} Graph;

typedef struct {
    GateType type;
    int size;
    int* inputs;
    int* outputs;
    int num_inputs;
    int num_outputs;
} Gate;

typedef struct {
    Variable vars[MAX_VARS];
    int var_count;
    int num_inputs;
    int num_outputs;
    Gate gates[MAX_GATES];
    int gate_count;
    bool* values;
} Circuit;

// Function to find or add a variable
int find_or_add_var(Circuit* circuit, const char* name) {
    // Check for constants
    if (strcmp(name, "0") == 0) {
        return -1;  // Special index for constant 0
    }
    if (strcmp(name, "1") == 0) {
        return -2;  // Special index for constant 1
    }
    if (strcmp(name, "_") == 0) {
        return -3;  // Special index for discarded output
    }

    for (int i = 0; i < circuit->var_count; i++) {
        if (strcmp(circuit->vars[i].name, name) == 0) {
            return i;
        }
    }
    
    strcpy(circuit->vars[circuit->var_count].name, name);
    circuit->vars[circuit->var_count].index = circuit->var_count;
    return circuit->var_count++;
}

Graph* create_graph(int num_vertices) {
    Graph* graph = malloc(sizeof(Graph));
    graph->num_vertices = num_vertices;
    graph->lists = calloc(num_vertices, sizeof(AdjList));
    for (int i = 0; i < num_vertices; i++) {
        graph->lists[i].edges = malloc(sizeof(int) * num_vertices);
        graph->lists[i].capacity = num_vertices;
        graph->lists[i].count = 0;
    }
    return graph;
}

void add_edge(Graph* graph, int from, int to) {
    if (graph->lists[from].count < graph->lists[from].capacity) {
        graph->lists[from].edges[graph->lists[from].count++] = to;
    }
}

void topological_sort_util(Graph* graph, int vertex, bool* visited, bool* stack, int* order, int* order_index) {
    if (stack[vertex]) {
        // Cycle detected
        fprintf(stderr, "Circuit contains a cycle\n");
        exit(1);
    }
    
    if (visited[vertex]) return;
    
    visited[vertex] = true;
    stack[vertex] = true;
    
    for (int i = 0; i < graph->lists[vertex].count; i++) {
        int adjacent = graph->lists[vertex].edges[i];
        topological_sort_util(graph, adjacent, visited, stack, order, order_index);
    }
    
    stack[vertex] = false;
    order[(*order_index)++] = vertex;
}

int* topological_sort(Graph* graph) {
    bool* visited = calloc(graph->num_vertices, sizeof(bool));
    bool* stack = calloc(graph->num_vertices, sizeof(bool));
    int* order = malloc(sizeof(int) * graph->num_vertices);
    int order_index = 0;
    
    for (int i = 0; i < graph->num_vertices; i++) {
        if (!visited[i]) {
            topological_sort_util(graph, i, visited, stack, order, &order_index);
        }
    }
    
    free(visited);
    free(stack);
    return order;
}

void free_graph(Graph* graph) {
    for (int i = 0; i < graph->num_vertices; i++) {
        free(graph->lists[i].edges);
    }
    free(graph->lists);
    free(graph);
}

// Get value of a variable (including constants)
bool get_value(bool* values, int var_index) {
    if (var_index == -1) return false;  // Constant 0
    if (var_index == -2) return true;   // Constant 1
    return values[var_index];
}

// Evaluate a single gate
void evaluate_gate(Gate* gate, bool* values) {
    bool result;
    switch (gate->type) {
        case AND:
            result = get_value(values, gate->inputs[0]) && get_value(values, gate->inputs[1]);
            if (gate->outputs[0] >= 0) values[gate->outputs[0]] = result;
            break;

        case OR:
            result = get_value(values, gate->inputs[0]) || get_value(values, gate->inputs[1]);
            if (gate->outputs[0] >= 0) values[gate->outputs[0]] = result;
            break;

        case NOT:
            result = !get_value(values, gate->inputs[0]);
            if (gate->outputs[0] >= 0) values[gate->outputs[0]] = result;
            break;

        case NAND:
            result = !(get_value(values, gate->inputs[0]) && get_value(values, gate->inputs[1]));
            if (gate->outputs[0] >= 0) values[gate->outputs[0]] = result;
            break;

        case NOR:
            result = !(get_value(values, gate->inputs[0]) || get_value(values, gate->inputs[1]));
            if (gate->outputs[0] >= 0) values[gate->outputs[0]] = result;
            break;

        case XOR:
            result = get_value(values, gate->inputs[0]) ^ get_value(values, gate->inputs[1]);
            if (gate->outputs[0] >= 0) values[gate->outputs[0]] = result;
            break;

        case DECODER: {
            int input_val = 0;
            for (int i = 0; i < gate->size; i++) {
                if (get_value(values, gate->inputs[i])) {
                    input_val |= (1 << (gate->size - 1 - i));
                }
            }
            for (int i = 0; i < (1 << gate->size); i++) {
                if (gate->outputs[i] >= 0) {
                    values[gate->outputs[i]] = (i == input_val);
                }
            }
            break;
        }

        case MULTIPLEXER: {
            int select_val = 0;
            int num_selectors = gate->size;
            int num_inputs = 1 << num_selectors;
            
            // Calculate selector value
            for (int i = 0; i < num_selectors; i++) {
                if (get_value(values, gate->inputs[num_inputs + i])) {
                    select_val |= (1 << (num_selectors - 1 - i));
                }
            }
            
            // Set output to selected input
            if (gate->outputs[0] >= 0) {
                values[gate->outputs[0]] = get_value(values, gate->inputs[select_val]);
            }
            break;
        }

        case PASS:
            if (gate->outputs[0] >= 0) {
                values[gate->outputs[0]] = get_value(values, gate->inputs[0]);
            }
            break;
    }
}

// Process INPUT directive
void process_input(Circuit* circuit, FILE* file) {
    int num_inputs;
    fscanf(file, "%d", &num_inputs);
    circuit->num_inputs = num_inputs;
    
    char var_name[MAX_VAR_NAME];
    for (int i = 0; i < num_inputs; i++) {
        fscanf(file, "%16s", var_name);
        find_or_add_var(circuit, var_name);
    }
}

// Process OUTPUT directive
void process_output(Circuit* circuit, FILE* file) {
    int num_outputs;
    fscanf(file, "%d", &num_outputs);
    circuit->num_outputs = num_outputs;
    
    char var_name[MAX_VAR_NAME];
    for (int i = 0; i < num_outputs; i++) {
        fscanf(file, "%16s", var_name);
        find_or_add_var(circuit, var_name);
    }
}

// Process gate directive
void process_gate(Circuit* circuit, FILE* file, const char* gate_name) {
    Gate* gate = &circuit->gates[circuit->gate_count];
    char var_name[MAX_VAR_NAME];
    
    if (strcmp(gate_name, "AND") == 0 || strcmp(gate_name, "OR") == 0 ||
        strcmp(gate_name, "NAND") == 0 || strcmp(gate_name, "NOR") == 0 ||
        strcmp(gate_name, "XOR") == 0) {
        
        if (strcmp(gate_name, "AND") == 0) gate->type = AND;
        else if (strcmp(gate_name, "OR") == 0) gate->type = OR;
        else if (strcmp(gate_name, "NAND") == 0) gate->type = NAND;
        else if (strcmp(gate_name, "NOR") == 0) gate->type = NOR;
        else gate->type = XOR;
        
        gate->num_inputs = 2;
        gate->num_outputs = 1;
        gate->inputs = malloc(sizeof(int) * 2);
        gate->outputs = malloc(sizeof(int));
        
        // Read two inputs and one output
        fscanf(file, "%16s", var_name);
        gate->inputs[0] = find_or_add_var(circuit, var_name);
        fscanf(file, "%16s", var_name);
        gate->inputs[1] = find_or_add_var(circuit, var_name);
        fscanf(file, "%16s", var_name);
        gate->outputs[0] = find_or_add_var(circuit, var_name);
    }
    else if (strcmp(gate_name, "NOT") == 0) {
        gate->type = NOT;
        gate->num_inputs = 1;
        gate->num_outputs = 1;
        gate->inputs = malloc(sizeof(int));
        gate->outputs = malloc(sizeof(int));
        
        fscanf(file, "%16s", var_name);
        gate->inputs[0] = find_or_add_var(circuit, var_name);
        fscanf(file, "%16s", var_name);
        gate->outputs[0] = find_or_add_var(circuit, var_name);
    }
    else if (strcmp(gate_name, "DECODER") == 0) {
        gate->type = DECODER;
        int n;
        fscanf(file, "%d", &n);
        gate->size = n;
        gate->num_inputs = n;
        gate->num_outputs = 1 << n;
        
        gate->inputs = malloc(sizeof(int) * n);
        gate->outputs = malloc(sizeof(int) * (1 << n));
        
        // Read inputs
        for (int i = 0; i < n; i++) {
            fscanf(file, "%16s", var_name);
            gate->inputs[i] = find_or_add_var(circuit, var_name);
        }
        
        // Read outputs
        for (int i = 0; i < (1 << n); i++) {
            fscanf(file, "%16s", var_name);
            gate->outputs[i] = find_or_add_var(circuit, var_name);
        }
    }
    else if (strcmp(gate_name, "MULTIPLEXER") == 0) {
        gate->type = MULTIPLEXER;
        int n;
        fscanf(file, "%d", &n);
        gate->size = n;
        gate->num_inputs = (1 << n) + n;  // Regular inputs + selector inputs
        gate->num_outputs = 1;
        
        gate->inputs = malloc(sizeof(int) * ((1 << n) + n));
        gate->outputs = malloc(sizeof(int));
        
        // Read regular inputs
        for (int i = 0; i < (1 << n); i++) {
            fscanf(file, "%16s", var_name);
            gate->inputs[i] = find_or_add_var(circuit, var_name);
        }
        
        // Read selector inputs
        for (int i = 0; i < n; i++) {
            fscanf(file, "%16s", var_name);
            gate->inputs[(1 << n) + i] = find_or_add_var(circuit, var_name);
        }
        
        // Read output
        fscanf(file, "%16s", var_name);
        gate->outputs[0] = find_or_add_var(circuit, var_name);
    }
    else if (strcmp(gate_name, "PASS") == 0) {
        gate->type = PASS;
        gate->num_inputs = 1;
        gate->num_outputs = 1;
        gate->inputs = malloc(sizeof(int));
        gate->outputs = malloc(sizeof(int));
        
        fscanf(file, "%16s", var_name);
        gate->inputs[0] = find_or_add_var(circuit, var_name);
        fscanf(file, "%16s", var_name);
        gate->outputs[0] = find_or_add_var(circuit, var_name);
    }
    
    circuit->gate_count++;
}

// Generate and print truth table
void generate_truth_table(Circuit* circuit) {
    // Build dependency graph
    Graph* graph = create_graph(circuit->gate_count);
    
    // Create mapping from variable to gate that produces it
    int* var_to_gate = malloc(sizeof(int) * circuit->var_count);
    memset(var_to_gate, -1, sizeof(int) * circuit->var_count);
    
    // Map output variables to their producing gates
    for (int i = 0; i < circuit->gate_count; i++) {
        Gate* gate = &circuit->gates[i];
        for (int j = 0; j < gate->num_outputs; j++) {
            if (gate->outputs[j] >= 0) {
                var_to_gate[gate->outputs[j]] = i;
            }
        }
    }
    
    // Add edges for dependencies
    for (int i = 0; i < circuit->gate_count; i++) {
        Gate* gate = &circuit->gates[i];
        for (int j = 0; j < gate->num_inputs; j++) {
            int input_var = gate->inputs[j];
            if (input_var >= 0) {  // Skip constants and discarded outputs
                int producer_gate = var_to_gate[input_var];
                if (producer_gate != -1) {
                    add_edge(graph, producer_gate, i);
                }
            }
        }
    }
    
    // Get evaluation order
    int* gate_order = topological_sort(graph);
    
    // Generate truth table
    int num_rows = 1 << circuit->num_inputs;
    circuit->values = calloc(circuit->var_count, sizeof(bool));
    
    for (int row = 0; row < num_rows; row++) {
        // Set input values
        for (int i = 0; i < circuit->num_inputs; i++) {
            circuit->values[i] = (row & (1 << (circuit->num_inputs - 1 - i))) != 0;
        }
        
        // Evaluate gates in topological order
        for (int i = circuit->gate_count - 1; i >= 0; i--) {
            evaluate_gate(&circuit->gates[gate_order[i]], circuit->values);
        }
        
        // Print input values
        for (int i = 0; i < circuit->num_inputs; i++) {
            printf("%d", circuit->values[i]);
            if (i < circuit->num_inputs - 1) printf(" ");
        }
        
        printf(" | ");
        
        // Print output values
        for (int i = 0; i < circuit->num_outputs; i++) {
            printf("%d", circuit->values[circuit->num_inputs + i]);
            if (i < circuit->num_outputs - 1) printf(" ");
        }
        printf("\n");
    }
    
    // Cleanup
    free(gate_order);
    free(var_to_gate);
    free_graph(graph);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <circuit_file>\n", argv[0]);
        return 1;
    }
    
    FILE* file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", argv[1]);
        return 1;
    }
    
    Circuit circuit = {0};
    char directive[MAX_VAR_NAME];
    
    while (fscanf(file, "%16s", directive) == 1) {
        if (strcmp(directive, "INPUT") == 0) {
            process_input(&circuit, file);
        } else if (strcmp(directive, "OUTPUT") == 0) {
            process_output(&circuit, file);
        } else {
            process_gate(&circuit, file, directive);
        }
    }
    
    generate_truth_table(&circuit);
    
    // Cleanup
    for (int i = 0; i < circuit.gate_count; i++) {
        free(circuit.gates[i].inputs);
        free(circuit.gates[i].outputs);
    }
    free(circuit.values);
    fclose(file);
    
    return 0;
}