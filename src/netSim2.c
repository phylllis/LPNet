
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(int, char**);
void usage();

void netSim(double alpha,
              double beta,
              double gamma,
              double xi,
              double delta_in,
              double delta_out,
              double target_n_edges_double,
              char *edge_file_name,
              char *degree_file_name,
              char *evolution_file_name
              )
{
    int i;
    FILE *edge_file;
    FILE *degree_file;
    FILE *evolution_file;
    
    int target_n_edges = (int) target_n_edges_double;
    int current_n_edges;
    int current_n_nodes;
    int current_scenario;
    int* in_deg_counts = (int*) malloc(sizeof(int) *target_n_edges*2);
    int* out_deg_counts = (int*) malloc(sizeof(int) *target_n_edges*2);
    int* edge_start = (int*) malloc(sizeof(int) *target_n_edges);
    int* edge_end = (int*) malloc(sizeof(int) *target_n_edges);
    int* evolution = (int*) malloc(sizeof(int) *target_n_edges);
    int this_edge_start;
    int this_edge_end;
    
    edge_start[0] = 0;
    edge_end[0] = 1;
    evolution[0] = 1;
    current_n_edges = 1;
    current_n_nodes = 2;
    current_scenario = 1;
    in_deg_counts[0] = 0;
    out_deg_counts[0] = 1;
    in_deg_counts[1] = 1;
    out_deg_counts[1] = 0;
    
    double u,v,w;
    
    srand((time(NULL) & 0xFFFF) | (getpid() << 16));
    
    printf("Simulating linear preferential attachment network with %d edges\n", target_n_edges);
    
    while (current_n_edges < target_n_edges)
    {
        // u = (double)rand() / (double)RAND_MAX;
        u = (double) ( rand() % 100000 / (double) 100000 );
        
        if (u <= alpha)
        {
            // Scenario alpha
            v = (double) ( rand() % 100000 / (double) 100000 ) * (current_n_edges + current_n_nodes*delta_in);
            if (v < current_n_edges)
            {
                this_edge_end = edge_end[(int) ceil(v)-1];
            }
            else
            {
                this_edge_end = (int) ceil((v-current_n_edges)/delta_in) -1;
            }
            this_edge_start = current_n_nodes;
            in_deg_counts[this_edge_end] ++;
            out_deg_counts[this_edge_start] = 1;
            in_deg_counts[this_edge_start] = 0;
            current_n_nodes++;
            current_scenario = 1;
        }
        
        else if (u > alpha & u <= alpha+beta)
        {
            // Scenario beta
            v = (double) ( rand() % 100000 / (double) 100000 )* (current_n_edges + current_n_nodes*delta_in);
            if (v < current_n_edges)
            {
                this_edge_end = edge_end[(int) ceil(v)-1];
            }
            else
            {
                this_edge_end = (int) ceil((v-current_n_edges)/delta_in) -1;
            }
            
            w = (double) ( rand() % 100000 / (double) 100000 )* (current_n_edges + current_n_nodes*delta_out);
            if (w < current_n_edges)
            {
                this_edge_start = edge_start[(int) ceil(w)-1];
            }
            else
            {
                this_edge_start = (int) ceil((w-current_n_edges)/delta_out) -1;
            }
            in_deg_counts[this_edge_end] ++;
            out_deg_counts[this_edge_start] ++;
            current_scenario = 2;
        }
        
        else if (u > alpha+beta & u <= alpha+beta+gamma)
        {
            // Scenario gamma
            w = (double) ( rand() % 100000 / (double) 100000 )* (current_n_edges + current_n_nodes*delta_out);
            if (w < current_n_edges)
            {
                this_edge_start = edge_start[(int) ceil(w)-1];
            }
            else
            {
                this_edge_start = (int) ceil((w-current_n_edges)/delta_out) -1;
            }
            this_edge_end = current_n_nodes;
            in_deg_counts[this_edge_end] = 1;
            out_deg_counts[this_edge_end] = 0;
            out_deg_counts[this_edge_start] ++;
            current_n_nodes++;
            current_scenario = 3;
        }
        else if (u > alpha+beta+gamma & u <= alpha+beta+gamma+xi)
        {
            // Scenario xi
            this_edge_start = (int) current_n_nodes;
            in_deg_counts[this_edge_start] = 0;
            out_deg_counts[this_edge_start] = 1;
            current_n_nodes++;
            this_edge_end = (int) current_n_nodes;
            in_deg_counts[this_edge_end] = 1;
            out_deg_counts[this_edge_end] = 0;
            current_n_nodes++;
            current_scenario = 4;
        }
        
        else // if (u > alpha+beta+gamma+xi & u <= 1)
        {
            // Scenario loop
            this_edge_start = (int) current_n_nodes;
            this_edge_end = (int) current_n_nodes;
            in_deg_counts[this_edge_end] = 1;
            out_deg_counts[this_edge_end] = 1;
            current_n_nodes++;
            current_scenario = 5;
        }
        
        evolution[current_n_edges] = current_scenario;
        edge_end[current_n_edges] = this_edge_end;
        edge_start[current_n_edges] = this_edge_start;
        current_n_edges ++;
    }
    printf("Simulation complete.\n");
    
    printf("Writing network edges to %s\n", edge_file_name);
    edge_file = fopen(edge_file_name, "w");
    for (i=0; i<target_n_edges; i++)
    {
        fprintf(edge_file,"%d,%d\n", edge_start[i],edge_end[i]);
    }
    
    printf("Writing network degree counts to %s\n", degree_file_name);
    degree_file = fopen(degree_file_name, "w");
    for (i=0; i<current_n_nodes; i++)
    {
        fprintf(degree_file,"%d,%d\n", in_deg_counts[i],out_deg_counts[i]);
    }
    
    printf("Writing evolution into %s\n", evolution_file_name);
    evolution_file = fopen(evolution_file_name, "w");
    for(i=0; i<target_n_edges; i++) {
        fprintf(evolution_file, "%d\n", evolution[i]);
    }
    free (evolution);
    free (in_deg_counts);
    free (out_deg_counts);
    free (edge_start);
    free (edge_end);
}

void usage() {
    printf("Usage:\nnetSim alpha beta gamma xi delta_in delta_out target_n_edges edge_output_filename.csv degree_output_filename.csv evolution_output_filename.csv\n");
}


int main(int argc, char** argv) {
    if (argc != 11) {
        usage();
        return -1;
    }
    double alpha,beta,gamma,xi,delta_in,delta_out,target_n_edges_double;
    
    //srand ( (unsigned)time ( NULL ) );
    int seed = time(NULL);
    srand((time(NULL) & 0xFFFF) | (getpid() << 16));
    
    sscanf(argv[1], "%lf", &alpha);
    sscanf(argv[2], "%lf", &beta);
    sscanf(argv[3], "%lf", &gamma);
    sscanf(argv[4], "%lf", &xi);
    sscanf(argv[5], "%lf", &delta_in);
    sscanf(argv[6], "%lf", &delta_out);
    sscanf(argv[7], "%lf", &target_n_edges_double);
    
    netSim(alpha,beta,gamma,xi,delta_in,delta_out,target_n_edges_double,
             argv[8],
             argv[9],
             argv[10]);
    return 0;
}
