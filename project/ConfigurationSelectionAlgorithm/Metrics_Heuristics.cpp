#include "Metrics_Heuristics.h"
#include <algorithm>
using namespace std;


void LinearWeightedFunction::add_function(ConfigurationEvaluationFunction *h,double weight){
        hs.push_back(std::make_pair(h,weight));
}

double LinearWeightedFunction::calculate(Configuration &conf){
        double result=0.0;
        for(std::list<std::pair<ConfigurationEvaluationFunction *,double> >::iterator it=hs.begin();it!=hs.end();it++ )
            result+= it->second * it->first->calculate(conf);
        return result;
}



