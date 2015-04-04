#include "Solver.h"

using namespace std;

void Attribute::assign_quality_attribute(FeatureModel *fmodel){
    switch(afunction){
        case AggregationFunction::ADDITION:
            for(int i=0;i<fmodel->nb_features();i++){
                Feature *f=fmodel->get_feature(i);
                if(f->is_concrete())
                    f->set_attribute(id,f->get_soft()->get_attribute(id));
                else
                    f->set_attribute(id,Addition::Neutral_Element);
                f->set_attribute(-id,Addition::Neutral_Element);
            }
            break;
        case AggregationFunction::PRODUCT:
            for(int i=0;i<fmodel->nb_features();i++){
                Feature *f=fmodel->get_feature(i);
                if(f->is_concrete())
                    f->set_attribute(id,f->get_soft()->get_attribute(id));
                else
                    f->set_attribute(id,Product::Neutral_Element);
                f->set_attribute(-id,Product::Neutral_Element);
            }
            break;
        case AggregationFunction::MAXIMUM:
            for(int i=0;i<fmodel->nb_features();i++){
                Feature *f=fmodel->get_feature(i);
                if(f->is_concrete())
                    f->set_attribute(id,f->get_soft()->get_attribute(id));
                else
                    f->set_attribute(id,Maximum::Neutral_Element);
                f->set_attribute(-id,Maximum::Neutral_Element);
            }
            break;
        case AggregationFunction::MINIMUM:
            for(int i=0;i<fmodel->nb_features();i++){
                Feature *f=fmodel->get_feature(i);
                if(f->is_concrete())
                    f->set_attribute(id,f->get_soft()->get_attribute(id));
                else
                    f->set_attribute(id,Minimum::Neutral_Element);
                f->set_attribute(-id,Minimum::Neutral_Element);
            }
    }
};

void Attribute::assign_switch_time_attribute(FeatureModel *fmodel,Configuration &current){
    for(int i=0;i<fmodel->nb_features();i++){
        Feature *f=fmodel->get_feature(i);
        if(f->is_concrete()){
            if(current[i]==FeatureState::SELECTED){
                f->set_attribute(id,0.0);
                f->set_attribute(-id,f->get_soft()->get_attribute(STOP_DELAY));
            }else{
                f->set_attribute(id,f->get_soft()->get_attribute(START_DELAY));
                f->set_attribute(-id,0.0);
            }
        }else{
            switch(afunction){
                case AggregationFunction::ADDITION:
                    f->set_attribute(id,Addition::Neutral_Element);
                    f->set_attribute(-id,Addition::Neutral_Element);
                    break;
                case AggregationFunction::PRODUCT:
                    f->set_attribute(id,Product::Neutral_Element);
                    f->set_attribute(-id,Product::Neutral_Element);
                    break;
                case AggregationFunction::MAXIMUM:
                    f->set_attribute(id,Maximum::Neutral_Element);
                    f->set_attribute(-id,Maximum::Neutral_Element);
                    break;
                case AggregationFunction::MINIMUM:
                    f->set_attribute(id,Minimum::Neutral_Element);
                    f->set_attribute(-id,Minimum::Neutral_Element);
                    break;
            }
        }
    }
};

void Attribute::add_evaluation_function(LinearWeightedFunction *lwf){
    ConfigurationEvaluationFunction *cef;

    switch(afunction){
        case AggregationFunction::ADDITION:
            if(objective==Objective::MINIMIZE){
                cef=new AdditionMinValueA(id,-id);
            }else{
                cef=new AdditionMaxValueA(id,-id);
            }
            break;
        case AggregationFunction::PRODUCT:
            if(objective==Objective::MINIMIZE){
                cef=new ProductMinValueA(id,-id);
            }else{
                cef=new ProductMaxValueA(id,-id);
            }
            break;
        case AggregationFunction::MAXIMUM:
            if(objective==Objective::MINIMIZE){
                cef=new MaximumMinValueA(id,-id);
            }else{
                cef=new MaximumMaxValueA(id,-id);
            }
            break;
        case AggregationFunction::MINIMUM:
            if(objective==Objective::MINIMIZE){
                cef=new MinimumMinValueA(id,-id);
            }else{
                cef=new MinimumMaxValueA(id,-id);
            }
    }

    if(objective==Objective::MINIMIZE)
        lwf->add_function(cef,weight);
    else
        lwf->add_function(cef,-weight);
};

