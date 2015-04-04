#ifndef METRICS_HEURISTICS_H_INCLUDED
#define METRICS_HEURISTICS_H_INCLUDED

#include "../FeatureModelFramework/Configuration.h"
#include <functional>

//!Aggregation functions defined as C++ bynary functors: Addition, Product, Maximum, Minimum

#define MINUS_INFINITE      -999999.0
#define PLUS_INFINITE       999999.0
#define ZERO                0.0
#define ONE                 1.0

struct Addition: std::binary_function <double,double,double> {
  double operator() (const double& x, const double& y) const
    {return x+y;}
  static const double Neutral_Element = ZERO;
};

struct Product: std::binary_function <double,double,double> {
  double operator() (const double& x, const double& y) const
    {return x*y;}
  static const double Neutral_Element = ONE;
};

struct Maximum: std::binary_function <double,double,double> {
  double operator() (const double& x, const double& y) const
    {return std::max(x,y);}
  static const double Neutral_Element = MINUS_INFINITE;
};

struct Minimum: std::binary_function <double,double,double> {
  double operator() (const double& x, const double& y) const
    {return std::min(x,y);}
  static const double Neutral_Element = PLUS_INFINITE;
};


//!Base class for evaluating configurations:
class ConfigurationEvaluationFunction;

//!Base class for heuristic functions
class RecursiveEvaluationFunction;

//!Function that allows the aggregation of other configuration functions (Composite design pattern).
class LinearWeightedFunction;

//!Function that evaluates full configurations, represents the aggregation functions used as quality metrics, and is equivalent to the heuristic functions for evaluating full configurations.
template<typename Function>
class Metric;

typedef Metric<Addition> AdditionMetric;
typedef Metric<Product> ProductMetric;
typedef Metric<Minimum> MinimumMetric;
typedef Metric<Maximum> MaximumMetric;


//!Definitions of classes

class ConfigurationEvaluationFunction{
public:
    virtual double calculate(Configuration &conf)=0;
};

class RecursiveEvaluationFunction:public ConfigurationEvaluationFunction{
protected:
    int id_attribute_selected;
    int id_attribute_deselected;
    virtual double calculate(Configuration &conf,Feature *f,FeatureState state/*SELECTED or DESELECTED*/)=0;
    RecursiveEvaluationFunction(int id_s,int id_d):id_attribute_selected(id_s),id_attribute_deselected(id_d){};
public:
    double calculate(Configuration &conf){
        return calculate(conf,conf.get_feature(0),SELECTED);
    }
};

#define W_MINIMIZE 1.0 //!If a positive value is used as weight (w), the function is minimized
#define W_MAXIMIZE -1.0//!If a negative value is used as weight (w), the function is maximized

class LinearWeightedFunction: public ConfigurationEvaluationFunction{
private:
    std::list<std::pair<ConfigurationEvaluationFunction *,double> > hs;
public:
    void add_function(ConfigurationEvaluationFunction *h,double weight=W_MINIMIZE);
    double calculate(Configuration &conf);
};

template<typename Function>
class Metric: public ConfigurationEvaluationFunction{
protected:
    int id_attribute_selected;
    int id_attribute_deselected;
public:
    Metric(int id_s,int id_d):id_attribute_selected(id_s),id_attribute_deselected(id_d){};
    double calculate(Configuration &conf){
        Function func;
        double result=Function::Neutral_Element;
        for(int i=0;i<conf.nb_features();i++){
            SoftElement *soft=conf.get_feature(i)->get_soft();
            if(soft!=NULL)
                switch(conf[i]){
                    case SELECTED:
                        result=func(result,soft->get_attribute(id_attribute_selected));
                        break;
                    case DESELECTED:
                        result=func(result,soft->get_attribute(id_attribute_deselected));
                }
        }
        return result;
    };
};

#endif // METRICS_HEURISTICS_H_INCLUDED
