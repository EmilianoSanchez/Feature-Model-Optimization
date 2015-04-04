#ifndef SEARCHALGORITHMS_H_INCLUDED
#define SEARCHALGORITHMS_H_INCLUDED

#include "../FeatureModelFramework/Configuration.h"
#include "Metrics_Heuristics.h"
#include "Containers.h"

/*! \file  SearchAlgorithms.h
 * \brief This file includes the definition and implementation of the best first search algorithm, used by the Solver class to select a full configuration from a partial one.
 */

template< typename Container >
bool best_first_search(Configuration &initial,Configuration &goal,
                                         ConfigurationEvaluationFunction *heuristic);

#define GreedyBFS best_first_search<CPriorityStack>
#define BFSStar best_first_search<CPriorityQueue>

//-----------------Implementation------------------

template< typename Container >
bool best_first_search(Configuration &initial,Configuration &goal,ConfigurationEvaluationFunction *heuristic){
    /**put the start state 'initial' on a set called OPEN of unexpanded nodes*/
    Container OPEN;
    OPEN.push(Node(initial));

    /**while OPEN is not empty, remove one by one all its elements to expand and travel the solution space*/
    while(!OPEN.empty()){
        Configuration n=OPEN.top().conf;
        OPEN.pop();

        /**put in the variable 'unselected_feature' the index of the following unselected feature to specialize.
        The strategy used is to choose the next unassigned feature in order, but heuristics could be used*/
        unsigned int unselected_feature=-1;
        for(unsigned int index=0;index<n.nb_features();index++){
            if(n[index]==UNSELECTED){
                unselected_feature=index;
                break;
            }
        }

        /**if no unselected features were found in the previous 'for', the state n is a full configuration. exit successfully with n as goal state*/
        if(unselected_feature==-1){//goal reached
            goal=n;
            return true;
        }else{
        /**otherwise expand n. generate two successors of n selecting and deselecting the chosen unselected feature.
           if the new states are valid (the assignment application succeeds), compute and record its heuristic value, and put it on the OPEN set.*/
            Node new_state_S(n);
            if(new_state_S.conf.apply_selection(unselected_feature)){
                new_state_S.value=heuristic->calculate(new_state_S.conf);
                OPEN.push(new_state_S);
            }
            Node new_state_D(n);
            if(new_state_D.conf.apply_deselection(unselected_feature)){
                new_state_D.value=heuristic->calculate(new_state_D.conf);
                OPEN.push(new_state_D);
            }
        }
    }

    /**if this statement is reached, it means that not valid full configurations were found. Therefore the initial configuration is invalid*/
    return false;
}


#endif // SEARCHALGORITHMS_H_INCLUDED
