#include "Experiments.h"

#include <iostream>
#include <list>
using namespace std;

#include "../FeatureModelFramework/Configuration.h"
#include "../ConfigurationSelectionAlgorithm/SearchAlgorithms.h"
#include "../ConfigurationSelectionAlgorithm/HeuristicsA.h"
#include "../ConfigurationSelectionAlgorithm/HeuristicsB.h"
#include "fm_generator.h"
#include <boost/timer.hpp>

#define NUMBER_OF_EXPERIMENTS  8
#define SAMPLES_PER_EXPERIMENT  30

/**Note: if conf is full configurations then:
 * M_lwf_1->calculate(conf)==HA_lwf_1->calculate(conf)==HB_lwf_1->calculate(conf)
 * M_lwf_4->calculate(conf)==HA_lwf_4->calculate(conf)==HB_lwf_4->calculate(conf)
 */
LinearWeightedFunction *M_lwf_1;
LinearWeightedFunction *M_lwf_4;

LinearWeightedFunction *HA_lwf_1;
LinearWeightedFunction *HA_lwf_4;

LinearWeightedFunction *HB_lwf_1;
LinearWeightedFunction *HB_lwf_4;

void initialize_linear_weighted_functions(){
    M_lwf_1=new LinearWeightedFunction();
    M_lwf_1->add_function(new AdditionMetric(0,1));

    HA_lwf_1=new LinearWeightedFunction();
    HA_lwf_1->add_function(new AdditionMinValueA(0,1));

    HB_lwf_1=new LinearWeightedFunction();
    HB_lwf_1->add_function(new AdditionMinValueB(0,1));

    M_lwf_4=new LinearWeightedFunction();
    M_lwf_4->add_function(new AdditionMetric(0,1));
    M_lwf_4->add_function(new ProductMetric(2,3));
    M_lwf_4->add_function(new MaximumMetric(4,5));
    M_lwf_4->add_function(new MinimumMetric(6,7));

    HA_lwf_4=new LinearWeightedFunction();
    HA_lwf_4->add_function(new AdditionMinValueA(0,1));
    HA_lwf_4->add_function(new ProductMinValueA(2,3));
    HA_lwf_4->add_function(new MaximumMinValueA(4,5));
    HA_lwf_4->add_function(new MinimumMinValueA(6,7));

    HB_lwf_4=new LinearWeightedFunction();
    HB_lwf_4->add_function(new AdditionMinValueB(0,1));
    HB_lwf_4->add_function(new ProductMinValueB(2,3));
    HB_lwf_4->add_function(new MaximumMinValueB(4,5));
    HB_lwf_4->add_function(new MinimumMinValueB(6,7));
}

/**Experiment 1.A: measuring execution time for GBFS and A* with HA and HB
    random feature model with 5 children per feature without cross-tree constraints.
    linear weighted function with four aggregation functions, one of each type.
    starting with 10 features until 10*NUMBER_OF_EXPERIMENTS (intervals of 10 features)
*/
void experiment_1_A(){

    //matrix used to store the results
    double results[5][NUMBER_OF_EXPERIMENTS];

    for(int nb_features=10;nb_features<=NUMBER_OF_EXPERIMENTS*10;nb_features+=10){

        cout<<nb_features<<endl;

        list<Configuration> initialConfigurations;
        double time;

        //generation of random feature models. the initial partial configuration is obteined selecting the root feature
        for(int nb_experiments=0;nb_experiments<SAMPLES_PER_EXPERIMENT;nb_experiments++){
            Configuration initial(generate_fm("A",nb_features,nb_features/5,HyperLink_Type::RANDOM,0,8));
            initial.apply_selection(0);
            initialConfigurations.push_back(initial);
        }

        //execution of the four variants of the algorithm
        results[0][nb_features/10-1]=nb_features;

        execute_and_return_time_efficiency(time,initialConfigurations,GreedyBFS,HA_lwf_4);
        results[1][nb_features/10-1]=time;

        execute_and_return_time_efficiency(time,initialConfigurations,GreedyBFS,HB_lwf_4);
        results[2][nb_features/10-1]=time;

        execute_and_return_time_efficiency(time,initialConfigurations,BFSStar,HA_lwf_4);
        results[3][nb_features/10-1]=time;

        execute_and_return_time_efficiency(time,initialConfigurations,BFSStar,HB_lwf_4);
        results[4][nb_features/10-1]=time;

    }

    //plot the results
    plot<5,NUMBER_OF_EXPERIMENTS>(results);
}

/**Experiment E.2.A: measuring optimality of heuristics A and B in GBFS,
    random feature model with no CTC and branching factor of 5
    LWF with only 1 aggregation function (addition)
    starting with 10 features until 10*NUMBER_OF_EXPERIMENTS (intervals of 10 features)
*/
void experiment_2_A(){

    //heuristic used to get the worst solution using best_first_search_star
    LinearWeightedFunction *worst_lwh_1=new LinearWeightedFunction();
    worst_lwh_1->add_function(new AdditiveMaxValueB(0,1),W_MAXIMIZE);

    double results[3][NUMBER_OF_EXPERIMENTS];
    Configuration goal;

    for(int nb_features=10;nb_features<=NUMBER_OF_EXPERIMENTS*10;nb_features+=10){
        cout<<nb_features<<endl;

        double value_GBFS_HA=0.0,value_GBFS_HB=0.0,best_value=0.0,worst_value=0.0;

        //generation of random feature models and execution of algorithms to get the best, worst and found solutions using HA and HB
        for(int i=0;i<SAMPLES_PER_EXPERIMENT;i++){
            Configuration initial(generate_fm("A",nb_features,nb_features/5,HyperLink_Type::RANDOM,0,8));
            initial.apply_selection(0);

            GreedyBFS(initial,goal,HA_lwf_1);
            value_GBFS_HA+=M_lwf_1->calculate(goal);

            GreedyBFS(initial,goal,HB_lwf_1);
            value_GBFS_HB+=M_lwf_1->calculate(goal);

            BFSStar(initial,goal,HB_lwf_1);
            best_value+=M_lwf_1->calculate(goal);

            BFSStar(initial,goal,worst_lwh_1);
            worst_value+=M_lwf_1->calculate(goal);

        }

        value_GBFS_HA/=SAMPLES_PER_EXPERIMENT;
        value_GBFS_HB/=SAMPLES_PER_EXPERIMENT;
        best_value/=SAMPLES_PER_EXPERIMENT;
        worst_value/=SAMPLES_PER_EXPERIMENT;

        results[0][nb_features/10-1]=nb_features;
        results[1][nb_features/10-1]=(worst_value-value_GBFS_HA)/(worst_value-best_value);
        results[2][nb_features/10-1]=(worst_value-value_GBFS_HB)/(worst_value-best_value);

    }

    //plot the results
    plot<3,NUMBER_OF_EXPERIMENTS>(results);
}

/**Experiment E.2.B: measuring optimality of heuristics A and B in GBFS,
    random feature model with 30 features and branching factor of 5
    LWF with only 1 aggregation function (addition)
    starting with 0 ctc until 4*(NUMBER_OF_EXPERIMENTS-1) (intervals of 4 ctc)
*/
void experiment_2_B(){

    //heuristic used to get the worst solution using BFS*
    LinearWeightedFunction *worst_lwh_1=new LinearWeightedFunction();
    worst_lwh_1->add_function(new AdditiveMaxValueB(0,1),W_MAXIMIZE);

    double results[3][NUMBER_OF_EXPERIMENTS];
    Configuration goal;

    for(int nb_ctc=0;nb_ctc<NUMBER_OF_EXPERIMENTS*4;nb_ctc+=4){
        cout<<nb_ctc<<endl;

        double value_GBFS_HA=0.0,value_GBFS_HB=0.0,best_value=0.0,worst_value=0.0;

        //generation of random feature models and execution of algorithms to get the best, worst and found solutions using HA and HB
        for(int i=0;i<SAMPLES_PER_EXPERIMENT;i++){
            Configuration initial(generate_fm("A",30,6,HyperLink_Type::RANDOM,nb_ctc,8));
            initial.apply_selection(0);

            GreedyBFS(initial,goal,HA_lwf_1);
            value_GBFS_HA+=M_lwf_1->calculate(goal);
            GreedyBFS(initial,goal,HB_lwf_1);
            value_GBFS_HB+=M_lwf_1->calculate(goal);
            BFSStar(initial,goal,HB_lwf_1);
            best_value+=M_lwf_1->calculate(goal);
            BFSStar(initial,goal,worst_lwh_1);
            worst_value+=M_lwf_1->calculate(goal);

        }

        value_GBFS_HA/=SAMPLES_PER_EXPERIMENT;
        value_GBFS_HB/=SAMPLES_PER_EXPERIMENT;
        best_value/=SAMPLES_PER_EXPERIMENT;
        worst_value/=SAMPLES_PER_EXPERIMENT;

        results[0][nb_ctc/4]=nb_ctc;
        results[1][nb_ctc/4]=(worst_value-value_GBFS_HA)/(worst_value-best_value);
        results[2][nb_ctc/4]=(worst_value-value_GBFS_HB)/(worst_value-best_value);

    }

    //plot the results
    plot<3,NUMBER_OF_EXPERIMENTS>(results);
}

/**Experiment E.2.C: measuring optimality of heuristics A and B in GBFS,
    random feature model with 30 features, branching factor of 5 and no CTC
    starting with 1 aggregation function until NUMBER_OF_EXPERIMENTS aggregation functions in the linear objective function
*/
void experiment_2_C(){

    double results[3][NUMBER_OF_EXPERIMENTS];
    Configuration goal;

    //generation of random feature models and their initial partial configurations
    list<Configuration> initialConfigurations;
    for(int nb_samples=0;nb_samples<SAMPLES_PER_EXPERIMENT;nb_samples++){
            Configuration initial(generate_fm("A",30,6,HyperLink_Type::RANDOM,0,16));
            initial.apply_selection(0);
            initialConfigurations.push_back(initial);
    }

    //linear weighted functions
    LinearWeightedFunction *M_lwh_alpha=new LinearWeightedFunction();
    LinearWeightedFunction *HA_lwh_alpha=new LinearWeightedFunction();
    LinearWeightedFunction *HB_lwh_alpha=new LinearWeightedFunction();
    LinearWeightedFunction *worst_lwh_alpha=new LinearWeightedFunction();

    for(int nb_experiment=0;nb_experiment<NUMBER_OF_EXPERIMENTS;nb_experiment++){

        cout<<nb_experiment<<endl;

        //for each iteration, a new aggregation function is added to the linear weighted functions
        M_lwh_alpha->add_function(new AdditionMetric(nb_experiment*2,nb_experiment*2+1),W_MINIMIZE);
        HA_lwh_alpha->add_function(new AdditionMinValueA(nb_experiment*2,nb_experiment*2+1),W_MINIMIZE);
        HB_lwh_alpha->add_function(new AdditionMinValueB(nb_experiment*2,nb_experiment*2+1),W_MINIMIZE);
        worst_lwh_alpha->add_function(new AdditiveMaxValueB(nb_experiment*2,nb_experiment*2+1),W_MAXIMIZE);

        //execution of algorithms to get the best, worst and found solutions using HA and HB
        double value_GBFS_HA=0.0,value_GBFS_HB=0.0,best_value=0.0,worst_value=0.0;

        for(list<Configuration>::iterator iter=initialConfigurations.begin();iter!=initialConfigurations.end();iter++){

            GreedyBFS(*iter,goal,HA_lwh_alpha);
            value_GBFS_HA+=M_lwh_alpha->calculate(goal);
            GreedyBFS(*iter,goal,HB_lwh_alpha);
            value_GBFS_HB+=M_lwh_alpha->calculate(goal);
            BFSStar(*iter,goal,HB_lwh_alpha);
            best_value+=M_lwh_alpha->calculate(goal);
            BFSStar(*iter,goal,worst_lwh_alpha);
            worst_value+=M_lwh_alpha->calculate(goal);
        }

        value_GBFS_HA/=SAMPLES_PER_EXPERIMENT;
        value_GBFS_HB/=SAMPLES_PER_EXPERIMENT;
        best_value/=SAMPLES_PER_EXPERIMENT;
        worst_value/=SAMPLES_PER_EXPERIMENT;

        results[0][nb_experiment]=nb_experiment+1;
        results[1][nb_experiment]=(worst_value-value_GBFS_HA)/(worst_value-best_value);
        results[2][nb_experiment]=(worst_value-value_GBFS_HB)/(worst_value-best_value);

    }

    //plot the results
    plot<3,NUMBER_OF_EXPERIMENTS>(results);
}

/**Experiment E.3.B: measuring efficiency of heuristics A and B in A*,
    random feature model with 30 features and branching factor of 5
    LWF with only 1 aggregation function (addition)
    starting with 0 ctc until 4*(NUMBER_OF_EXPERIMENTS-1) (intervals of 4 ctc)
*/
void experiment_3_B(){

    double results[3][NUMBER_OF_EXPERIMENTS];

    for(int nb_ctc=0;nb_ctc<NUMBER_OF_EXPERIMENTS*4;nb_ctc+=4){

        cout<<nb_ctc<<endl;

        //generation of random feature models and their initial partial configurations
        list<Configuration> initialConfigurations;
        for(int nb_experiments=0;nb_experiments<SAMPLES_PER_EXPERIMENT;nb_experiments++){
            Configuration initial(generate_fm("A",30,6,HyperLink_Type::RANDOM,nb_ctc,8));
            initial.apply_selection(0);
            initialConfigurations.push_back(initial);
        }

        //execution of algorithms
        double time;
        results[0][nb_ctc/4]=nb_ctc;
        execute_and_return_time_efficiency(time,initialConfigurations,BFSStar,HA_lwf_1);
        results[1][nb_ctc/4]=time;
        execute_and_return_time_efficiency(time,initialConfigurations,BFSStar,HB_lwf_1);
        results[2][nb_ctc/4]=time;

    }

    //plot the results
    plot<3,NUMBER_OF_EXPERIMENTS>(results);
}

/**Experiment E.3.C: measuring execution time of heuristics A and B in BFS*,
    random feature model with 30 features, branching factor of 5 and no CTC
    starting with 1 aggregation function until NUMBER_OF_EXPERIMENTS aggregation functions in the linear objective function
*/
void experiment_3_C(){

    double results[3][NUMBER_OF_EXPERIMENTS];
    Configuration goal;

    //generation of random feature models and their initial partial configurations
    list<Configuration> initialConfigurations;
    for(int nb_samples=0;nb_samples<SAMPLES_PER_EXPERIMENT;nb_samples++){
            Configuration initial(generate_fm("A",30,6,HyperLink_Type::RANDOM,0,8));
            initial.apply_selection(0);
            initialConfigurations.push_back(initial);
    }

    LinearWeightedFunction *HA_lwf_alpha=new LinearWeightedFunction();
    LinearWeightedFunction *HB_lwf_alpha=new LinearWeightedFunction();

    for(int nb_experiment=0;nb_experiment<NUMBER_OF_EXPERIMENTS;nb_experiment++){
        cout<<nb_experiment<<endl;

        //for each iteration, a new aggregation function is added to the linear weighted functions
        int id_attribute=rand()%4;
        switch(nb_experiment%4){
            case 0:
                HA_lwf_alpha->add_function(new AdditionMinValueA(id_attribute*2,id_attribute*2+1));
                HB_lwf_alpha->add_function(new AdditionMinValueB(id_attribute*2,id_attribute*2+1));
                break;
            case 1:
                HA_lwf_alpha->add_function(new ProductMinValueA(id_attribute*2,id_attribute*2+1));
                HB_lwf_alpha->add_function(new ProductMinValueB(id_attribute*2,id_attribute*2+1));
                break;
            case 2:
                HA_lwf_alpha->add_function(new MaximumMinValueA(id_attribute*2,id_attribute*2+1));
                HB_lwf_alpha->add_function(new MaximumMinValueB(id_attribute*2,id_attribute*2+1));
                break;
            case 3:
                HA_lwf_alpha->add_function(new MinimumMinValueA(id_attribute*2,id_attribute*2+1));
                HB_lwf_alpha->add_function(new MinimumMinValueB(id_attribute*2,id_attribute*2+1));
        }

        //execution of algorithms
        double time;
        results[0][nb_experiment]=nb_experiment+1;
        execute_and_return_time_efficiency(time,initialConfigurations,BFSStar,HA_lwf_alpha);
        results[1][nb_experiment]=time;
        execute_and_return_time_efficiency(time,initialConfigurations,BFSStar,HB_lwf_alpha);
        results[2][nb_experiment]=time;

    }

    //plot the results
    plot<3,NUMBER_OF_EXPERIMENTS>(results);
}


void execute_and_return_time_efficiency(
    double &average_time,
    list<Configuration> &initial_configurations,
    bool (*search_algorithm)(Configuration &initial,Configuration &goal,ConfigurationEvaluationFunction *heuristic),
    ConfigurationEvaluationFunction *heuristic){

    Configuration goal;
    boost::timer t;

    for(list<Configuration>::iterator fiter=initial_configurations.begin();fiter!=initial_configurations.end();fiter++){
        search_algorithm(*fiter,goal,heuristic);
    }

    average_time= t.elapsed()/(double)initial_configurations.size();
}
