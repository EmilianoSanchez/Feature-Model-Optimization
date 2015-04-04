#ifndef HEURISTICSB_H_INCLUDED
#define HEURISTICSB_H_INCLUDED

#include "Metrics_Heuristics.h"

/*! \file  HeuristicsB.h
 * \brief The heuristics of type B are exact for the relaxed model B (without cross-tree constraints).
 *        As each attribute could be maximized or minimized with four different aggregation functions, we need to define two heuristics for each aggregation function.
          For minimizing, the heuristic must follow the 'lowest' or 'min' criterion, i.e. estimate the minimum value of the set of full configurations of the given partial configuration.
          For maximizing, the heuristic must follow the 'highest' or 'max' criterion, i.e. estimate the maximum value of the set of full configurations of the given partial configuration.
 */


class AdditionMinValueB: public RecursiveEvaluationFunction{
protected:
    double calculate(Configuration &conf,Feature *f,FeatureState state);
public:
    AdditionMinValueB(int id_s,int id_d):RecursiveEvaluationFunction(id_s,id_d){};
};

class ProductMinValueB: public RecursiveEvaluationFunction{
protected:
    double calculate(Configuration &conf,Feature *f,FeatureState state);
public:
    ProductMinValueB(int id_s,int id_d):RecursiveEvaluationFunction(id_s,id_d){};
};

class MaximumMinValueB: public RecursiveEvaluationFunction{
protected:
    double calculate(Configuration &conf,Feature *f,FeatureState state);
public:
    MaximumMinValueB(int id_s,int id_d):RecursiveEvaluationFunction(id_s,id_d){};
};

class MinimumMinValueB: public RecursiveEvaluationFunction{
protected:
    double calculate(Configuration &conf,Feature *f,FeatureState state);
public:
    MinimumMinValueB(int id_s,int id_d):RecursiveEvaluationFunction(id_s,id_d){};
};

class AdditiveMaxValueB: public RecursiveEvaluationFunction{
protected:
    double calculate(Configuration &conf,Feature *f,FeatureState state);
public:
    AdditiveMaxValueB(int id_s,int id_d):RecursiveEvaluationFunction(id_s,id_d){};
};

class ProductMaxValueB: public RecursiveEvaluationFunction{
protected:
    double calculate(Configuration &conf,Feature *f,FeatureState state);
public:
    ProductMaxValueB(int id_s,int id_d):RecursiveEvaluationFunction(id_s,id_d){};
};

class MaximumMaxValueB: public RecursiveEvaluationFunction{
protected:
    double calculate(Configuration &conf,Feature *f,FeatureState state);
public:
    MaximumMaxValueB(int id_s,int id_d):RecursiveEvaluationFunction(id_s,id_d){};
};

class MinimumMaxValueB: public RecursiveEvaluationFunction{
protected:
    double calculate(Configuration &conf,Feature *f,FeatureState state);
public:
    MinimumMaxValueB(int id_s,int id_d):RecursiveEvaluationFunction(id_s,id_d){};
};

#endif // HEURISTICSB_H_INCLUDED
