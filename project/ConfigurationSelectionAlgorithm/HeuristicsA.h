#ifndef HEURISTICSA_H_INCLUDED
#define HEURISTICSA_H_INCLUDED

#include "Metrics_Heuristics.h"

/*! \file  HeuristicsA.h
 * \brief The heuristics of type A are exact for the relaxed model A (without cross-tree constraints and considering XOR and OR groups as AND groups with optional children).
 *        As each attribute could be maximized or minimized with four different aggregation functions, we need to define two heuristics for each aggregation function.
          For minimizing, the heuristic must follow the 'lowest' or 'min' criterion, i.e. estimate the minimum value of the set of full configurations of the given partial configuration.
          For maximizing, the heuristic must follow the 'highest' or 'max' criterion, i.e. estimate the maximum value of the set of full configurations of the given partial configuration.
 */

template<typename Criteria,typename Function>
class HeuristicA: public RecursiveEvaluationFunction{
protected:
    double calculate(Configuration &conf,Feature *f,FeatureState state);
public:
    HeuristicA(int id_s,int id_d):RecursiveEvaluationFunction(id_s,id_d){};
};

#include <algorithm>
#include <list>

template<typename Criteria,typename Function>
double HeuristicA<Criteria,Function>::calculate(Configuration &conf,Feature *f,FeatureState state){

    Criteria crit;
    Function func;
    double result=Function::Neutral_Element;

    if(f->get_hyperlink_type()!=HLTag::leaf){
        std::list<Link *> children= (f->decomposition()->links());
        if(state==DESELECTED){
            for(std::list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                result=func(result,calculate(conf,(*iter)->end(),DESELECTED));

        }else{
            for(std::list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                switch(conf[(*iter)->end()->get_index()]){
                    case SELECTED:
                        result=func(result,calculate(conf,(*iter)->end(),SELECTED));
                        break;
                    case DESELECTED:
                        result=func(result,calculate(conf,(*iter)->end(),DESELECTED));
                        break;
                    case UNSELECTED:
                        if((*iter)->get_type()==Tag::mandatory)
                            result=func(result,calculate(conf,(*iter)->end(),SELECTED));
                        else
                            result=func(result,crit(calculate(conf,(*iter)->end(),SELECTED),calculate(conf,(*iter)->end(),DESELECTED)));
                }
        }
    }

    if(state==DESELECTED)
        result=func(result,f->get_attribute(id_attribute_deselected));
    else
        result=func(result,f->get_attribute(id_attribute_selected));

    return result;
}

typedef HeuristicA<Minimum,Addition> AdditionMinValueA;
typedef HeuristicA<Minimum,Product> ProductMinValueA;
typedef HeuristicA<Minimum,Minimum> MinimumMinValueA;
typedef HeuristicA<Minimum,Maximum> MaximumMinValueA;
typedef HeuristicA<Maximum,Addition> AdditionMaxValueA;
typedef HeuristicA<Maximum,Product> ProductMaxValueA;
typedef HeuristicA<Maximum,Minimum> MinimumMaxValueA;
typedef HeuristicA<Maximum,Maximum> MaximumMaxValueA;

#endif // HEURISTICSA_H_INCLUDED
