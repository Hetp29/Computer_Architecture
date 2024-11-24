#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VAR_NAME 17 //no keyword or variable name exceeds 16 characters
#define MAX_VARS 1024 //pre-allocate array 
#define MAX_GATES 1024 //pre-allocate array

typedef enum { FALSE = 0, TRUE = 1} Boolean; 

typedef enum { AND, OR, NOT, NAND, NOR, XOR, DECODER, MULTIPLEXER, PASS } king_t; 

typedef struct {
    king_t type; 
    int size; 
    int *inputs;
    int *outputs;
    int num_inputs; 
    int num_outputs;
} Gate; //data structure for logic gate

typedef struct {
    char name[MAX_VAR_NAME];
    int index;
} Variable; //stores variable names and index for inputs and outputs

typedef struct {
    Variable vars[MAX_VARS];
    int var_count;
    int num_inputs;
    int num_outputs;
    Gate gates[MAX_GATES];
    int gate_count;
    Boolean* values;
} Circuit; //stores variables, gates, and truth table data

typedef struct {
    int* edges;
    int count;
    int capacity;
} AdjList; //in case circuit descripitions not sorted

typedef struct {
    AdjList* lists; 
    int num_vertices;
} Graph; //directed graphs connection between gates 

//function to assign indices to new variables or get existing ones
int addVar(Circuit* circuit, const char* name) {
    if (strcmp(name, "0") == 0) {
        return -1; //constant 0 index
    }

    if (strcmp(name, "1") == 0) {
        return -2; //constant 1 index
    }

    if(strcmp(name, "_") == 0) {
        return -3;
    }

    for(int i = 0; i < circuit->var_count; i++) {
        if(strcmp(circuit->vars[i].name, name) == 0) {
            return i;
        }
    }

    strcpy(circuit->vars[circuit->var_count].name, name);
    circuit->vars[circuit->var_count].index = circuit->var_count;
    return circuit->var_count++;
}

//get variable's value
Boolean get_value(Boolean* values, int index) {
    if(index == -1) return FALSE; //0
    if(index == -2) return TRUE; //1
    return values[index];
}

//calculate output based on gate inputs
void calculate_output(Gate *gate, Boolean* values) {
    Boolean result;
    switch(gate->type) {
        case AND:
            result = get_value(values, gate->inputs[0]) && get_value(values, gate->inputs[1]);
            if (gate->outputs[0] >= 0) values[gate->outputs[0]] = result;
            break;

        case OR:
            result = get_value(values, gate->inputs[0]) || get_value(values, gate->inputs[1]);
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

        case NOT:
            result = !get_value(values, gate->inputs[0]);
            if (gate->outputs[0] >= 0) values[gate->outputs[0]] = result;
            break;

        case PASS:
            if (gate->outputs[0] >= 0) {
                values[gate->outputs[0]] = get_value(values, gate->inputs[0]);
            }
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
                    if(i == input_val) {
                        values[gate->outputs[i]] = TRUE;
                    }
                    else {
                        values[gate->outputs[i]] = FALSE;
                    }
                }
            }
            break;

        case MULTIPLEXER:
            int select_val = 0;
            int num_selectors = gate->size;
            int num_inputs = 1 << num_selectors;

            //selector value
            for(int i = 0; i < num_selectors; i++) {
                if(get_value(values, gate -> inputs[num_inputs + i])) {
                    select_val |= (1 << (num_selectors - 1 - i));
                }
            }

            //output set to selected input
            if(gate->outputs[0] >= 0) {
                values[gate->outputs[0]] = get_value(values, gate->inputs[select_val]);
            }
            break;
        }
    
    }
}

//read input variables and add them to circuit
void process_input(Circuit* circuit, FILE* file) {
    int num_inputs;
    fscanf(file, "%d", &num_inputs);
    circuit->num_inputs = num_inputs;

    char name[MAX_VAR_NAME];
    for(int i = 0; i < num_inputs; i++) {
        fscanf(file, "%16s", name);
        addVar(circuit, name);
    }
}

//read output variables and add them to circuit
void process_output(Circuit* circuit, FILE* file) {
    int num_outputs;
    fscanf(file, "%d", &num_outputs);
    circuit->num_outputs = num_outputs;

    char name[MAX_VAR_NAME];
    for(int i = 0; i < num_outputs; i++) {
        fscanf(file, "%16s", name);
        addVar(circuit, name);
    }
}

//read and match gates with their inputs and outputs 
void match_gate(Circuit* circuit, FILE* file, const char* gateName) {
    Gate* gate = &circuit->gates[circuit->gate_count];
    char name[MAX_VAR_NAME];

    if (strcmp(gateName, "AND") == 0 || strcmp(gateName, "OR") == 0 ||
        strcmp(gateName, "NAND") == 0 || strcmp(gateName, "NOR") == 0 ||
        strcmp(gateName, "XOR") == 0) {

        if (strcmp(gateName, "AND") == 0) gate->type = AND;
        else if (strcmp(gateName, "OR") == 0) gate->type = OR;
        else if (strcmp(gateName, "NAND") == 0) gate->type = NAND;
        else if (strcmp(gateName, "NOR") == 0) gate->type = NOR;
        else gate->type = XOR;

        gate->num_inputs = 2;
        gate->num_outputs = 1;
        gate->inputs = malloc(sizeof(int) * 2);
        gate->outputs = malloc(sizeof(int));

        fscanf(file, "%16s", name);
        gate->inputs[0] = addVar(circuit, name);
        fscanf(file, "%16s", name);
        gate->inputs[1] = addVar(circuit, name);
        fscanf(file, "%16s", name);
        gate->outputs[0] = addVar(circuit, name);
        }
    else if (strcmp(gateName, "NOT") == 0) {
        gate->type = NOT;
        gate->num_inputs = 1;
        gate->num_outputs = 1;
        gate->inputs = malloc(sizeof(int));
        gate->outputs = malloc(sizeof(int));
        
        fscanf(file, "%16s", name);
        gate->inputs[0] = addVar(circuit, name);
        fscanf(file, "%16s", name);
        gate->outputs[0] = addVar(circuit, name);
    }
    else if (strcmp(gateName, "DECODER") == 0) {
        gate->type = DECODER;
        int n;
        fscanf(file, "%d", &n);
        gate->size = n;
        gate->num_inputs = n;
        gate->num_outputs = 1 << n;
        
        gate->inputs = malloc(sizeof(int) * n);
        gate->outputs = malloc(sizeof(int) * (1 << n));
        
        for (int i = 0; i < n; i++) {
            fscanf(file, "%16s", name);
            gate->inputs[i] = addVar(circuit, name);
        }
        
        for (int i = 0; i < (1 << n); i++) {
            fscanf(file, "%16s", name);
            gate->outputs[i] = addVar(circuit, name);
        }
    }
    else if (strcmp(gateName, "MULTIPLEXER") == 0) {
        gate->type = MULTIPLEXER;
        int n;
        fscanf(file, "%d", &n);
        gate->size = n;
        gate->num_inputs = (1 << n) + n;  
        gate->num_outputs = 1;
        
        gate->inputs = malloc(sizeof(int) * ((1 << n) + n));
        gate->outputs = malloc(sizeof(int));
        
        for (int i = 0; i < (1 << n); i++) {
            fscanf(file, "%16s", name);
            gate->inputs[i] = addVar(circuit, name);
        }
        
        for (int i = 0; i < n; i++) {
            fscanf(file, "%16s", name);
            gate->inputs[(1 << n) + i] = addVar(circuit, name);
        }
        
        fscanf(file, "%16s", name);
        gate->outputs[0] = addVar(circuit, name);
    }
    else if (strcmp(gateName, "PASS") == 0) {
        gate->type = PASS;
        gate->num_inputs = 1;
        gate->num_outputs = 1;
        gate->inputs = malloc(sizeof(int));
        gate->outputs = malloc(sizeof(int));
        
        fscanf(file, "%16s", name);
        gate->inputs[0] = addVar(circuit, name);
        fscanf(file, "%16s", name);
        gate->outputs[0] = addVar(circuit, name);
    }
    
    circuit->gate_count++;
}

//initalize graph with vertices and adjacency list
Graph* create_graph(int num_vertices) {
    Graph* graph = malloc(sizeof(Graph));
    graph->num_vertices = num_vertices;
    graph->lists = malloc(num_vertices * sizeof(AdjList));
    memset(graph->lists, 0, num_vertices * sizeof(AdjList));
    for (int i = 0; i < num_vertices; i++) {
        graph->lists[i].edges = malloc(sizeof(int) * num_vertices);
        graph->lists[i].capacity = num_vertices;
        graph->lists[i].count = 0;
    }
    return graph;
}

//add directed edge from one grate to another
void add_edge(Graph* graph, int from, int to) {
    if (graph->lists[from].count < graph->lists[from].capacity) {
        graph->lists[from].edges[graph->lists[from].count++] = to;
    }
}

//topological sort: orders the gates based on dependencies using depth-first approach 
void topological_sort_two(Graph* graph, int vertex, Boolean* visited, Boolean* stack, int* order, int* order_index) {
    if (stack[vertex]) {
        exit(1);
    }

    if (visited[vertex]) return;

    visited[vertex] = TRUE;
    stack[vertex] = TRUE;

    for (int i = 0; i < graph->lists[vertex].count; i++) {
        int adjacent = graph->lists[vertex].edges[i];
        topological_sort_two(graph, adjacent, visited, stack, order, order_index);
    }

    stack[vertex] = FALSE;
    order[(*order_index)++] = vertex;
}

int* topological_sort(Graph* graph) {
    Boolean* visited = malloc(graph->num_vertices * sizeof(Boolean));
    Boolean* stack = malloc(graph->num_vertices * sizeof(Boolean));
    memset(visited, 0, graph->num_vertices * sizeof(Boolean));
    memset(stack, 0, graph->num_vertices * sizeof(Boolean));
    int* order = malloc(sizeof(int) * graph->num_vertices);
    int order_index = 0;
    
    for (int i = 0; i < graph->num_vertices; i++) {
        if (!visited[i]) {
            topological_sort_two(graph, i, visited, stack, order, &order_index);
        }
    }
    
    free(visited);
    free(stack);
    return order;
}

//free memory allocated for graph
void free_graph(Graph* graph) {
    for(int i = 0; i < graph->num_vertices; i++) {
        free(graph->lists[i].edges);
    }
    free(graph->lists);
    free(graph);
}

//create truth table by simulating the circuit
void create_truth_table(Circuit* circuit) {
    Graph* graph = create_graph(circuit->gate_count);

    int* var_to_gate = malloc(sizeof(int) * circuit->var_count);
    memset(var_to_gate, -1, sizeof(int) * circuit->var_count);

    //map output to gates
    for (int i = 0; i < circuit->gate_count; i++) {
        Gate* gate = &circuit->gates[i];
        for (int j = 0; j < gate->num_outputs; j++) {
            if (gate->outputs[j] >= 0) {
                var_to_gate[gate->outputs[j]] = i;
            }
        }
    }

    for (int i = 0; i < circuit->gate_count; i++) {
        Gate* gate = &circuit->gates[i];
        for (int j = 0; j < gate->num_inputs; j++) {
            int input_var = gate->inputs[j];
            if (input_var >= 0) {  
                int producer_gate = var_to_gate[input_var];
                if (producer_gate != -1) {
                    add_edge(graph, producer_gate, i);
                }
            }
        }
    }

    int* gate_order = topological_sort(graph);

    int num_rows = 1 << circuit->num_inputs;
    circuit->values = malloc(circuit->var_count * sizeof(Boolean));
    memset(circuit->values, 0, circuit->var_count * sizeof(Boolean));

    for (int row = 0; row < num_rows; row++) {
        for (int i = 0; i < circuit->num_inputs; i++) {
            circuit->values[i] = (row & (1 << (circuit->num_inputs - 1 - i))) ? TRUE : FALSE;
        }
        
        for (int i = circuit->gate_count - 1; i >= 0; i--) {
            calculate_output(&circuit->gates[gate_order[i]], circuit->values);
        }
        
        for (int i = 0; i < circuit->num_inputs; i++) {
            printf("%d", circuit->values[i]);
            if (i < circuit->num_inputs - 1) printf(" ");
        }
        
        printf(" | ");
        
        for (int i = 0; i < circuit->num_outputs; i++) {
            printf("%d", circuit->values[circuit->num_inputs + i]);
            if (i < circuit->num_outputs - 1) printf(" ");
        }
        printf("\n");
    }
    
    free(gate_order);
    free(var_to_gate);
    free_graph(graph);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if(!file) {
        return 1;
    }

    Circuit circuit = {0};
    char directive[MAX_VAR_NAME];

    while(fscanf(file, "%16s", directive) == 1) {
        if(strcmp(directive, "INPUT") == 0) {
            process_input(&circuit, file);
        }
        else if(strcmp(directive, "OUTPUT") == 0) {
            process_output(&circuit, file);
        }
        else {
            match_gate(&circuit, file, directive);
        }
    }

    create_truth_table(&circuit);

    for (int i = 0; i < circuit.gate_count; i++) {
        free(circuit.gates[i].inputs);
        free(circuit.gates[i].outputs);
    }
    free(circuit.values);
    fclose(file);
    
    return 0;
}