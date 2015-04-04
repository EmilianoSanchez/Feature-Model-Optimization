#ifndef EXPERIMENTS_H_INCLUDED
#define EXPERIMENTS_H_INCLUDED

#include <list>
#include "../FeatureModelFramework/Configuration.h"
#include "../ConfigurationSelectionAlgorithm/SearchAlgorithms.h"

void initialize_linear_weighted_functions();

/**Experiment 1.A: measuring execution time for GBFS and A* with HA and HB
    random feature model with 5 children per feature without cross-tree constraints.
    linear weighted function with four aggregation functions, one of each type.
    starting with 10 features until 10*NUMBER_OF_EXPERIMENTS (intervals of 10 features)
*/
void experiment_1_A();

/**Experiment E.2.A: measuring optimality of heuristics A and B in GBFS,
    random feature model with no CTC and branching factor of 5
    LWF with only 1 aggregation function (addition)
    starting with 10 features until 10*NUMBER_OF_EXPERIMENTS (intervals of 10 features)
*/
void experiment_2_A();

/**Experiment E.2.B: measuring optimality of heuristics A and B in GBFS,
    random feature model with 30 features and branching factor of 5
    LWF with only 1 aggregation function (addition)
    starting with 0 ctc until 4*(NUMBER_OF_EXPERIMENTS-1) (intervals of 4 ctc)
*/
void experiment_2_B();

/**Experiment E.2.C: measuring optimality of heuristics A and B in GBFS,
    random feature model with 30 features, branching factor of 5 and no CTC
    starting with 1 aggregation function until NUMBER_OF_EXPERIMENTS aggregation functions in the linear objective function
*/
void experiment_2_C();

/**Experiment E.3.B: measuring efficiency of heuristics A and B in A*,
    random feature model with 30 features and branching factor of 5
    LWF with only 1 aggregation function (addition)
    starting with 0 ctc until 4*(NUMBER_OF_EXPERIMENTS-1) (intervals of 4 ctc)
*/
void experiment_3_B();

/**Experiment E.3.C: measuring execution time of heuristics A and B in BFS*,
    random feature model with 30 features, branching factor of 5 and no CTC
    starting with 1 aggregation function until NUMBER_OF_EXPERIMENTS aggregation functions in the linear objective function
*/
void experiment_3_C();

/**Function that execute several times "search_algorithm" with "heuristic" using the list of partial configuration as input.
 * The execution time (in seconds) is assigned to the variable "average_time"
 */
void execute_and_return_time_efficiency(
    double &average_time,
    std::list<Configuration> &initial_configurations,
    bool (*search_algorithm)(Configuration &initial,Configuration &goal,ConfigurationEvaluationFunction *heuristic),
    ConfigurationEvaluationFunction *heuristic);

/**Function used to load the content of a matrix in OpenOffice (or any other plotting tool)
 */
template <int SIZE_X,int SIZE_Y>
void plot(double data[SIZE_X][SIZE_Y]);

//Implementation of template function:

#include <fstream>

template <int SIZE_X,int SIZE_Y>
void plot(double data[SIZE_X][SIZE_Y]){
    std::fstream fichero;
    fichero.open("results.csv", std::ios::out);
    for(int j=0;j<SIZE_Y;j++){
        for(int i=0;i<SIZE_X;i++)
            fichero<<data[i][j]<<";";
        fichero<<std::endl;
    }
    fichero.close();

    //change this command to open the file with another plotting tool
    system("openoffice.org -r results.csv");
}


#endif // EXPERIMENTS_H_INCLUDED
