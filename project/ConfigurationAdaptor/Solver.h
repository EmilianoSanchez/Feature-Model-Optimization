#ifndef SOLVER_H_INCLUDED
#define SOLVER_H_INCLUDED

#include "../ConfigurationSelectionAlgorithm/SearchAlgorithms.h"
#include "../ConfigurationSelectionAlgorithm/HeuristicsA.h"
#include <list>

/*! \file  Solver.h
 * \brief Represents the configuration selection algorithms
 */

//!Key values of software element and feature attributes, used to access them in their property maps
#define EXECUTION_TIME      1// rates the time needed for a component to perform its task.
#define ACCURACY            2
#define MEMORY_CONSUMPTION  3

#define START_DELAY         4// rates the time cost to start a component or activate a parameter at runtime
#define STOP_DELAY          5// rates the time cost to stop a component or desactivate a parameter at runtime
#define OPERATIONAL_DELAY   6// rates the time needed for a software element to be operational from its start up
#define SWITCH_TIME         7

/*! \var Enumeration to indicate the type of attribute
 * \brief The mapping from software element attributes to feature attributes depends on the type and the aggregation function of the attribute.
 */
enum AttributeType{SWITCH_TIME_ATTRIBUTE,QUALITY_ATTRIBUTE};

/*! \var Enumeration to indicate the type of aggregation function used to evaluete the attribute for the whole configuration.
 * \brief The mapping from software element attributes to feature attributes depends on the type and the aggregation function of the attribute.
 */
enum AggregationFunction{ADDITION,PRODUCT,MAXIMUM,MINIMUM};

/*! \var Enumeration to indicate our objective respect to an attribute: minimize or maximize.
 */
enum Objective{MAXIMIZE,MINIMIZE};

/*! \class Attribute
 * \brief This class contains all the necessary information to manage an attribute:
 *  'id' is the key value used to access the attribute in the property map. For features, id represents the key value if f is selected (A_s(f)), and -id if deselected (A_d(f))
 *  'type' indicates the kind of attribute.
 *  'afunction' represents with which aggregation function is evaluated the attribute
 *  'objective' indicates if the attribute should be maximized or minimized
 *  'weight' represents the importance of the attribute. It is used by the linear weighted function to evaluate several attributes together.
 */

class Attribute{
public:
    int id;
    AttributeType type;
    AggregationFunction afunction;
    Objective objective;
    double weight;

    Attribute(){};
    Attribute(int _id,AttributeType _type,AggregationFunction _afunction,Objective _objective,double _weight)
        :id(_id),type(_type),afunction(_afunction),objective(_objective),weight(_weight){};

    /**The following methods are used to assign attributes from software elements to features of the given feature model.
     * When the Mediator initializes the ComponentManager, only the software elements have attributes.
     * Therefore, this method must be executed before the Solver object searchs for a configuration, so that the features have their associated values.
     * This function takes into account the value of 'type' and 'afunction' to assign A_s(f) and A_d(f).
     */
    void assign_quality_attribute(FeatureModel *fmodel);
    void assign_switch_time_attribute(FeatureModel *fmodel,Configuration &current);

    /**This method adds the correspondent heuristic to the given linear weighted function
     * considering the weight, objective and aggregation function type of the attribute.
     */
    void add_evaluation_function(LinearWeightedFunction *lwf);
};

/*! \class Solver
 * \brief This class performs the selection of full configurations from partial configurations, in two different steps of the system life:
 *  -Load time: when the system initialize an initial full configuration must be selected (method 'select_initial_configuration')
 *  -Run time: each event may involve the change of the partial configuration that represents the current variability of the system.
    In that case, the method 'select_new_configuration' is invoked to choose the new full configuration to adapt the system.
 */
class Solver{
private:

    std::list<Attribute> quality_attributes;
    Attribute switch_cost;

    LinearWeightedFunction *load_time_cef;
    LinearWeightedFunction *run_time_cef;
public:
    Solver(FeatureModel *fmodel):
        load_time_cef(new LinearWeightedFunction()),
        run_time_cef(new LinearWeightedFunction()),
        quality_attributes({
            Attribute(EXECUTION_TIME,AttributeType::QUALITY_ATTRIBUTE,AggregationFunction::MAXIMUM,Objective::MINIMIZE,0.1),
            Attribute(ACCURACY,AttributeType::QUALITY_ATTRIBUTE,AggregationFunction::PRODUCT,Objective::MAXIMIZE,0.5),
            Attribute(MEMORY_CONSUMPTION,AttributeType::QUALITY_ATTRIBUTE,AggregationFunction::ADDITION,Objective::MINIMIZE,0.1)
            }),
        switch_cost(SWITCH_TIME,AttributeType::SWITCH_TIME_ATTRIBUTE,AggregationFunction::ADDITION,Objective::MINIMIZE,1.0){

        for(std::list<Attribute>::iterator iter=quality_attributes.begin();iter!=quality_attributes.end();iter++){
            iter->assign_quality_attribute(fmodel);
            iter->add_evaluation_function(load_time_cef);
            iter->add_evaluation_function(run_time_cef);
        }
        switch_cost.add_evaluation_function(run_time_cef);

    };

    bool select_initial_configuration(Configuration &conf){
        return GreedyBFS(conf,conf,load_time_cef);
    };
    bool select_new_configuration(Configuration &conf,Configuration &current){
        /**before calling the search algorithm, the 'assign_switch_time_attribute' method must be invoked
         * to update the value of this attribute in the features taking into account the current configuration
         */
        switch_cost.assign_switch_time_attribute(current.get_fm(),current);
        return GreedyBFS(conf,conf,run_time_cef);
    };
};


#endif
