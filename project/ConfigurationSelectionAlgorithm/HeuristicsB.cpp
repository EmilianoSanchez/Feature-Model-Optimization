#include "HeuristicsB.h"

#include <algorithm>
#include <list>
using namespace std;


double AdditionMinValueB::calculate(Configuration &conf,Feature *f,FeatureState state){
    double sum=0.0;

    if(f->get_hyperlink_type()!=HLTag::leaf){
        list<Link *> children= (f->decomposition()->links());
        if(state==DESELECTED){
            for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                sum+=calculate(conf,(*iter)->end(),DESELECTED);

        }else{
            bool at_least_one_selected;
            double value_selected,value_deselected,min_difference;
            switch(f->get_hyperlink_type()){
                case HLTag::and_group:
                    for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                        switch(conf[(*iter)->end()->get_index()]){
                            case SELECTED:
                                sum+=calculate(conf,(*iter)->end(),SELECTED);
                                break;
                            case DESELECTED:
                                sum+=calculate(conf,(*iter)->end(),DESELECTED);
                                break;
                            case UNSELECTED:
                                if((*iter)->get_type()==Tag::mandatory)
                                    sum+=calculate(conf,(*iter)->end(),SELECTED);
                                else
                                    sum+=min(calculate(conf,(*iter)->end(),SELECTED),calculate(conf,(*iter)->end(),DESELECTED));
                        }
                    break;
                case HLTag::or_group:
                    at_least_one_selected=false;
                    min_difference=PLUS_INFINITE;
                    for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                        switch(conf[(*iter)->end()->get_index()]){
                            case SELECTED:
                                sum+=calculate(conf,(*iter)->end(),SELECTED);
                                at_least_one_selected=true;
                                break;
                            case DESELECTED:
                                sum+=calculate(conf,(*iter)->end(),DESELECTED);
                                break;
                            case UNSELECTED:
                                value_selected=calculate(conf,(*iter)->end(),SELECTED);
                                value_deselected=calculate(conf,(*iter)->end(),DESELECTED);
                                if(min_difference>value_selected-value_deselected)
                                    min_difference=value_selected-value_deselected;
                                if(value_selected<value_deselected)
                                    at_least_one_selected=true;
                                sum+=min(value_selected,value_deselected);
                        }
                    if(at_least_one_selected==false){
                        sum+=min_difference;
                    }
                    break;
                case HLTag::xor_group:
                    at_least_one_selected=false;
                    min_difference=PLUS_INFINITE;
                    for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                        switch(conf[(*iter)->end()->get_index()]){
                            case SELECTED:
                                sum+=calculate(conf,(*iter)->end(),SELECTED);
                                at_least_one_selected=true;
                                break;
                            case DESELECTED:
                                sum+=calculate(conf,(*iter)->end(),DESELECTED);
                                break;
                            case UNSELECTED:
                                value_selected=calculate(conf,(*iter)->end(),SELECTED);
                                value_deselected=calculate(conf,(*iter)->end(),DESELECTED);
                                if(min_difference>value_selected-value_deselected)
                                    min_difference=value_selected-value_deselected;
                                sum+=value_deselected;
                        }
                    if(at_least_one_selected==false){
                        sum+=min_difference;
                    }
            }
        }
    }

    if(state==DESELECTED)
        sum+=f->get_attribute(id_attribute_deselected);
    else
        sum+=f->get_attribute(id_attribute_selected);

    return sum;
}


double ProductMinValueB::calculate(Configuration &conf,Feature *f,FeatureState state/*SELECTED or DESELECTED*/){
    double product=1.0;

    if(f->get_hyperlink_type()!=HLTag::leaf){
        list<Link *> children= (f->decomposition()->links());
        if(state==DESELECTED){
            for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                product*=calculate(conf,(*iter)->end(),DESELECTED);

        }else{
            bool at_least_one_selected;
            double value_selected,value_deselected,min_difference;
            switch(f->get_hyperlink_type()){
                case HLTag::and_group:
                    for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                        switch(conf[(*iter)->end()->get_index()]){
                            case SELECTED:
                                product*=calculate(conf,(*iter)->end(),SELECTED);
                                break;
                            case DESELECTED:
                                product*=calculate(conf,(*iter)->end(),DESELECTED);
                                break;
                            case UNSELECTED:
                                if((*iter)->get_type()==Tag::mandatory)
                                    product*=calculate(conf,(*iter)->end(),SELECTED);
                                else
                                    product*=min(calculate(conf,(*iter)->end(),SELECTED),calculate(conf,(*iter)->end(),DESELECTED));
                        }
                    break;
                case HLTag::or_group:
                    at_least_one_selected=false;
                    min_difference=PLUS_INFINITE;
                    for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                        switch(conf[(*iter)->end()->get_index()]){
                            case SELECTED:
                                product*=calculate(conf,(*iter)->end(),SELECTED);
                                at_least_one_selected=true;
                                break;
                            case DESELECTED:
                                product*=calculate(conf,(*iter)->end(),DESELECTED);
                                break;
                            case UNSELECTED:
                                value_selected=calculate(conf,(*iter)->end(),SELECTED);
                                value_deselected=calculate(conf,(*iter)->end(),DESELECTED);
                                if(min_difference>value_selected/value_deselected)
                                    min_difference=value_selected/value_deselected;
                                if(value_selected<value_deselected)
                                    at_least_one_selected=true;
                                product*=min(value_selected,value_deselected);
                        }
                    if(at_least_one_selected==false){
                        product*=min_difference;
                    }
                    break;
                case HLTag::xor_group:
                    at_least_one_selected=false;
                    min_difference=PLUS_INFINITE;
                    for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                        switch(conf[(*iter)->end()->get_index()]){
                            case SELECTED:
                                product*=calculate(conf,(*iter)->end(),SELECTED);
                                at_least_one_selected=true;
                                break;
                            case DESELECTED:
                                product*=calculate(conf,(*iter)->end(),DESELECTED);
                                break;
                            case UNSELECTED:
                                value_selected=calculate(conf,(*iter)->end(),SELECTED);
                                value_deselected=calculate(conf,(*iter)->end(),DESELECTED);
                                if(min_difference>value_selected/value_deselected)
                                    min_difference=value_selected/value_deselected;
                                product*=value_deselected;
                        }
                    if(at_least_one_selected==false){
                        product*=min_difference;
                    }
            }
        }
    }

    if(state==DESELECTED)
        product*=f->get_attribute(id_attribute_deselected);
    else
        product*=f->get_attribute(id_attribute_selected);

    return product;
}


double MaximumMinValueB::calculate(Configuration &conf,Feature *f,FeatureState state/*SELECTED or DESELECTED*/){
    double maximum=MINUS_INFINITE;

    if(f->get_hyperlink_type()!=HLTag::leaf){
        list<Link *> children= (f->decomposition()->links());
        if(state==DESELECTED){
            for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                maximum=max(maximum,calculate(conf,(*iter)->end(),DESELECTED));

        }else{
            bool at_least_one_selected;
            double value_selected,value_deselected,min_max_selected,max_max_deselected,maximum_unselected;
            int index_min_max_selected,index_max_max_deselected;
            switch(f->get_hyperlink_type()){
                case HLTag::and_group:
                    for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                        switch(conf[(*iter)->end()->get_index()]){
                            case SELECTED:
                                maximum=max(maximum,calculate(conf,(*iter)->end(),SELECTED));
                                break;
                            case DESELECTED:
                                maximum=max(maximum,calculate(conf,(*iter)->end(),DESELECTED));
                                break;
                            case UNSELECTED:
                                if((*iter)->get_type()==Tag::mandatory)
                                    maximum=max(maximum,calculate(conf,(*iter)->end(),SELECTED));
                                else
                                    maximum=max(maximum,min(calculate(conf,(*iter)->end(),SELECTED),calculate(conf,(*iter)->end(),DESELECTED)));
                        }
                    break;
                case HLTag::or_group:
                    at_least_one_selected=false;
                    maximum_unselected=MINUS_INFINITE;
                    index_min_max_selected = -1;
                    min_max_selected = PLUS_INFINITE;
                    for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                        switch(conf[(*iter)->end()->get_index()]){
                            case SELECTED:
                                maximum=max(maximum,calculate(conf,(*iter)->end(),SELECTED));
                                at_least_one_selected=true;
                                break;
                            case DESELECTED:
                                maximum=max(maximum,calculate(conf,(*iter)->end(),DESELECTED));
                                break;
                            case UNSELECTED:
                                value_selected=calculate(conf,(*iter)->end(),SELECTED);
                                value_deselected=calculate(conf,(*iter)->end(),DESELECTED);
                                if(value_selected<value_deselected){
                                    at_least_one_selected=true;
                                }else{
                                    if(min_max_selected>value_selected){
                                        min_max_selected=value_selected;
                                        index_min_max_selected=(*iter)->end()->get_index();
                                    }
                                }
                                maximum_unselected=max(maximum_unselected,min(value_selected,value_deselected));
                        }
                    if(at_least_one_selected==false){
                        for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                            if((conf[(*iter)->end()->get_index()])==UNSELECTED){
                                if((*iter)->end()->get_index()==index_min_max_selected)
                                    maximum=max(maximum,calculate(conf,(*iter)->end(),SELECTED));
                                else
                                    maximum=max(maximum,calculate(conf,(*iter)->end(),DESELECTED));
                            }
                    }else
                        maximum=max(maximum,maximum_unselected);
                    break;
                case HLTag::xor_group:
                    at_least_one_selected=false;
                    index_min_max_selected = -1;
                    min_max_selected = PLUS_INFINITE;
                    index_max_max_deselected = -1;
                    max_max_deselected = MINUS_INFINITE;
                    for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                        switch(conf[(*iter)->end()->get_index()]){
                            case SELECTED:
                                maximum=max(maximum,calculate(conf,(*iter)->end(),SELECTED));
                                at_least_one_selected=true;
                                break;
                            case DESELECTED:
                                maximum=max(maximum,calculate(conf,(*iter)->end(),DESELECTED));
                                break;
                            case UNSELECTED:
                                value_selected=calculate(conf,(*iter)->end(),SELECTED);
                                value_deselected=calculate(conf,(*iter)->end(),DESELECTED);
                                if(value_selected>value_deselected){
                                    if(min_max_selected>value_selected){
                                        min_max_selected=value_selected;
                                        index_min_max_selected=(*iter)->end()->get_index();
                                    }
                                }else{
                                    if(max_max_deselected<value_deselected){
                                        max_max_deselected=value_deselected;
                                        index_max_max_deselected=(*iter)->end()->get_index();
                                    }
                                }
                        }
                    if(at_least_one_selected==false){
                        int index_to_select;
                        if(index_max_max_deselected== -1)
                            index_to_select=index_min_max_selected;
                        else
                            index_to_select=index_max_max_deselected;
                        for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                            if((conf[(*iter)->end()->get_index()])==UNSELECTED){
                                if((*iter)->end()->get_index()==index_to_select)
                                    maximum=max(maximum,calculate(conf,(*iter)->end(),SELECTED));
                                else
                                    maximum=max(maximum,calculate(conf,(*iter)->end(),DESELECTED));
                            }

                    }
            }
        }
    }

    if(state==DESELECTED)
        maximum=max(maximum,f->get_attribute(id_attribute_deselected));
    else
        maximum=max(maximum,f->get_attribute(id_attribute_selected));

    return maximum;
}


double MinimumMinValueB::calculate(Configuration &conf,Feature *f,FeatureState state/*SELECTED or DESELECTED*/){
    double minimum=PLUS_INFINITE;

    if(f->get_hyperlink_type()!=HLTag::leaf){
        list<Link *> children= (f->decomposition()->links());
        if(state==DESELECTED){
            for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                minimum=min(minimum,calculate(conf,(*iter)->end(),DESELECTED));

        }else{
            switch(f->get_hyperlink_type()){
                case HLTag::and_group:
                    for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                        switch(conf[(*iter)->end()->get_index()]){
                            case SELECTED:
                                minimum=min(minimum,calculate(conf,(*iter)->end(),SELECTED));
                                break;
                            case DESELECTED:
                                minimum=min(minimum,calculate(conf,(*iter)->end(),DESELECTED));
                                break;
                            case UNSELECTED:
                                if((*iter)->get_type()==Tag::mandatory)
                                    minimum=min(minimum,calculate(conf,(*iter)->end(),SELECTED));
                                else
                                    minimum=min(minimum,min(calculate(conf,(*iter)->end(),SELECTED),calculate(conf,(*iter)->end(),DESELECTED)));
                        }
                    break;
                default: //or_group or xor_group:
                    for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                        switch(conf[(*iter)->end()->get_index()]){
                            case SELECTED:
                                minimum=min(minimum,calculate(conf,(*iter)->end(),SELECTED));
                                break;
                            case DESELECTED:
                                minimum=min(minimum,calculate(conf,(*iter)->end(),DESELECTED));
                                break;
                            case UNSELECTED:
                                minimum=min(minimum,calculate(conf,(*iter)->end(),SELECTED));
                                minimum=min(minimum,calculate(conf,(*iter)->end(),DESELECTED));
                        }
            }
        }
    }

    if(state==DESELECTED)
        minimum=min(minimum,f->get_attribute(id_attribute_deselected));
    else
        minimum=min(minimum,f->get_attribute(id_attribute_selected));

    return minimum;
}



double AdditiveMaxValueB::calculate(Configuration &conf,Feature *f,FeatureState state/*SELECTED or DESELECTED*/){
    double sum=0.0;

    if(f->get_hyperlink_type()!=HLTag::leaf){
        list<Link *> children= (f->decomposition()->links());
        if(state==DESELECTED){
            for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                sum+=calculate(conf,(*iter)->end(),DESELECTED);

        }else{
            bool at_least_one_selected;
            double value_selected,value_deselected,max_difference;
            switch(f->get_hyperlink_type()){
                case HLTag::and_group:
                    for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                        switch(conf[(*iter)->end()->get_index()]){
                            case SELECTED:
                                sum+=calculate(conf,(*iter)->end(),SELECTED);
                                break;
                            case DESELECTED:
                                sum+=calculate(conf,(*iter)->end(),DESELECTED);
                                break;
                            case UNSELECTED:
                                if((*iter)->get_type()==Tag::mandatory)
                                    sum+=calculate(conf,(*iter)->end(),SELECTED);
                                else
                                    sum+=max(calculate(conf,(*iter)->end(),SELECTED),calculate(conf,(*iter)->end(),DESELECTED));
                        }
                    break;
                case HLTag::or_group:
                    at_least_one_selected=false;
                    max_difference=MINUS_INFINITE;
                    for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                        switch(conf[(*iter)->end()->get_index()]){
                            case SELECTED:
                                sum+=calculate(conf,(*iter)->end(),SELECTED);
                                at_least_one_selected=true;
                                break;
                            case DESELECTED:
                                sum+=calculate(conf,(*iter)->end(),DESELECTED);
                                break;
                            case UNSELECTED:
                                value_selected=calculate(conf,(*iter)->end(),SELECTED);
                                value_deselected=calculate(conf,(*iter)->end(),DESELECTED);
                                if(max_difference<value_selected-value_deselected)
                                    max_difference=value_selected-value_deselected;
                                if(value_selected>value_deselected)
                                    at_least_one_selected=true;
                                sum+=max(value_selected,value_deselected);
                        }
                    if(at_least_one_selected==false){
                        sum+=max_difference;
                    }
                    break;
                case HLTag::xor_group:
                    at_least_one_selected=false;
                    max_difference=MINUS_INFINITE;
                    for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                        switch(conf[(*iter)->end()->get_index()]){
                            case SELECTED:
                                sum+=calculate(conf,(*iter)->end(),SELECTED);
                                at_least_one_selected=true;
                                break;
                            case DESELECTED:
                                sum+=calculate(conf,(*iter)->end(),DESELECTED);
                                break;
                            case UNSELECTED:
                                value_selected=calculate(conf,(*iter)->end(),SELECTED);
                                value_deselected=calculate(conf,(*iter)->end(),DESELECTED);
                                if(max_difference<value_selected-value_deselected)
                                    max_difference=value_selected-value_deselected;
                                sum+=value_deselected;
                        }
                    if(at_least_one_selected==false){
                        sum+=max_difference;
                    }
            }
        }
    }

    if(state==DESELECTED)
        sum+=f->get_attribute(id_attribute_deselected);
    else
        sum+=f->get_attribute(id_attribute_selected);

    return sum;
}


double ProductMaxValueB::calculate(Configuration &conf,Feature *f,FeatureState state/*SELECTED or DESELECTED*/){
    double product=1.0;

    if(f->get_hyperlink_type()!=HLTag::leaf){
        list<Link *> children= (f->decomposition()->links());
        if(state==DESELECTED){
            for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                product*=calculate(conf,(*iter)->end(),DESELECTED);

        }else{
            bool at_least_one_selected;
            double value_selected,value_deselected,max_difference;
            switch(f->get_hyperlink_type()){
                case HLTag::and_group:
                    for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                        switch(conf[(*iter)->end()->get_index()]){
                            case SELECTED:
                                product*=calculate(conf,(*iter)->end(),SELECTED);
                                break;
                            case DESELECTED:
                                product*=calculate(conf,(*iter)->end(),DESELECTED);
                                break;
                            case UNSELECTED:
                                if((*iter)->get_type()==Tag::mandatory)
                                    product*=calculate(conf,(*iter)->end(),SELECTED);
                                else
                                    product*=max(calculate(conf,(*iter)->end(),SELECTED),calculate(conf,(*iter)->end(),DESELECTED));
                        }
                    break;
                case HLTag::or_group:
                    at_least_one_selected=false;
                    max_difference=MINUS_INFINITE;
                    for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                        switch(conf[(*iter)->end()->get_index()]){
                            case SELECTED:
                                product*=calculate(conf,(*iter)->end(),SELECTED);
                                at_least_one_selected=true;
                                break;
                            case DESELECTED:
                                product*=calculate(conf,(*iter)->end(),DESELECTED);
                                break;
                            case UNSELECTED:
                                value_selected=calculate(conf,(*iter)->end(),SELECTED);
                                value_deselected=calculate(conf,(*iter)->end(),DESELECTED);
                                if(max_difference<value_selected/value_deselected)
                                    max_difference=value_selected/value_deselected;
                                if(value_selected>value_deselected)
                                    at_least_one_selected=true;
                                product*=max(value_selected,value_deselected);
                        }
                    if(at_least_one_selected==false){
                        product*=max_difference;
                    }
                    break;
                case HLTag::xor_group:
                    at_least_one_selected=false;
                    max_difference=MINUS_INFINITE;
                    for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                        switch(conf[(*iter)->end()->get_index()]){
                            case SELECTED:
                                product*=calculate(conf,(*iter)->end(),SELECTED);
                                at_least_one_selected=true;
                                break;
                            case DESELECTED:
                                product*=calculate(conf,(*iter)->end(),DESELECTED);
                                break;
                            case UNSELECTED:
                                value_selected=calculate(conf,(*iter)->end(),SELECTED);
                                value_deselected=calculate(conf,(*iter)->end(),DESELECTED);
                                if(max_difference<value_selected/value_deselected)
                                    max_difference=value_selected/value_deselected;
                                product*=value_deselected;
                        }
                    if(at_least_one_selected==false){
                        product*=max_difference;
                    }
            }
        }
    }

    if(state==DESELECTED)
        product*=f->get_attribute(id_attribute_deselected);
    else
        product*=f->get_attribute(id_attribute_selected);

    return product;
}


double MaximumMaxValueB::calculate(Configuration &conf,Feature *f,FeatureState state/*SELECTED or DESELECTED*/){
    double maximum=MINUS_INFINITE;

    if(f->get_hyperlink_type()!=HLTag::leaf){
        list<Link *> children= (f->decomposition()->links());
        if(state==DESELECTED){
            for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                maximum=max(maximum,calculate(conf,(*iter)->end(),DESELECTED));

        }else{
            switch(f->get_hyperlink_type()){
                case HLTag::and_group:
                    for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                        switch(conf[(*iter)->end()->get_index()]){
                            case SELECTED:
                                maximum=max(maximum,calculate(conf,(*iter)->end(),SELECTED));
                                break;
                            case DESELECTED:
                                maximum=max(maximum,calculate(conf,(*iter)->end(),DESELECTED));
                                break;
                            case UNSELECTED:
                                if((*iter)->get_type()==Tag::mandatory)
                                    maximum=max(maximum,calculate(conf,(*iter)->end(),SELECTED));
                                else
                                    maximum=max(maximum,max(calculate(conf,(*iter)->end(),SELECTED),calculate(conf,(*iter)->end(),DESELECTED)));
                        }
                    break;
                default: //or_group or xor_group:
                    for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                        switch(conf[(*iter)->end()->get_index()]){
                            case SELECTED:
                                maximum=max(maximum,calculate(conf,(*iter)->end(),SELECTED));
                                break;
                            case DESELECTED:
                                maximum=max(maximum,calculate(conf,(*iter)->end(),DESELECTED));
                                break;
                            case UNSELECTED:
                                maximum=max(maximum,calculate(conf,(*iter)->end(),SELECTED));
                                maximum=max(maximum,calculate(conf,(*iter)->end(),DESELECTED));
                        }
            }
        }
    }

    if(state==DESELECTED)
        maximum=max(maximum,f->get_attribute(id_attribute_deselected));
    else
        maximum=max(maximum,f->get_attribute(id_attribute_selected));

    return maximum;
}


double MinimumMaxValueB::calculate(Configuration &conf,Feature *f,FeatureState state/*SELECTED or DESELECTED*/){
    double minimum=PLUS_INFINITE;

    if(f->get_hyperlink_type()!=HLTag::leaf){
        list<Link *> children= (f->decomposition()->links());
        if(state==DESELECTED){
            for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                minimum=min(minimum,calculate(conf,(*iter)->end(),DESELECTED));

        }else{
            bool at_least_one_selected;
            double value_selected,value_deselected,max_min_selected,min_min_deselected,minimum_unselected;
            int index_max_min_selected,index_min_min_deselected;
            switch(f->get_hyperlink_type()){
                case HLTag::and_group:
                    for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                        switch(conf[(*iter)->end()->get_index()]){
                            case SELECTED:
                                minimum=min(minimum,calculate(conf,(*iter)->end(),SELECTED));
                                break;
                            case DESELECTED:
                                minimum=min(minimum,calculate(conf,(*iter)->end(),DESELECTED));
                                break;
                            case UNSELECTED:
                                if((*iter)->get_type()==Tag::mandatory)
                                    minimum=min(minimum,calculate(conf,(*iter)->end(),SELECTED));
                                else
                                    minimum=min(minimum,max(calculate(conf,(*iter)->end(),SELECTED),calculate(conf,(*iter)->end(),DESELECTED)));
                        }
                    break;
                case HLTag::or_group:
                    at_least_one_selected=false;
                    minimum_unselected=PLUS_INFINITE;
                    index_max_min_selected = -1;
                    max_min_selected = MINUS_INFINITE;
                    for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                        switch(conf[(*iter)->end()->get_index()]){
                            case SELECTED:
                                minimum=min(minimum,calculate(conf,(*iter)->end(),SELECTED));
                                at_least_one_selected=true;
                                break;
                            case DESELECTED:
                                minimum=min(minimum,calculate(conf,(*iter)->end(),DESELECTED));
                                break;
                            case UNSELECTED:
                                value_selected=calculate(conf,(*iter)->end(),SELECTED);
                                value_deselected=calculate(conf,(*iter)->end(),DESELECTED);
                                if(value_selected>value_deselected){
                                    at_least_one_selected=true;
                                }else{
                                    if(max_min_selected<value_selected){
                                        max_min_selected=value_selected;
                                        index_max_min_selected=(*iter)->end()->get_index();
                                    }
                                }
                                minimum_unselected=min(minimum_unselected,max(value_selected,value_deselected));
                        }
                    if(at_least_one_selected==false){
                        for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                            if((conf[(*iter)->end()->get_index()])==UNSELECTED){
                                if((*iter)->end()->get_index()==index_max_min_selected)
                                    minimum=min(minimum,calculate(conf,(*iter)->end(),SELECTED));
                                else
                                    minimum=min(minimum,calculate(conf,(*iter)->end(),DESELECTED));
                            }
                    }else
                        minimum=min(minimum,minimum_unselected);
                    break;
                case HLTag::xor_group:
                    at_least_one_selected=false;
                    index_max_min_selected = -1;
                    max_min_selected = MINUS_INFINITE;
                    index_min_min_deselected = -1;
                    min_min_deselected = PLUS_INFINITE;
                    for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                        switch(conf[(*iter)->end()->get_index()]){
                            case SELECTED:
                                minimum=min(minimum,calculate(conf,(*iter)->end(),SELECTED));
                                at_least_one_selected=true;
                                break;
                            case DESELECTED:
                                minimum=min(minimum,calculate(conf,(*iter)->end(),DESELECTED));
                                break;
                            case UNSELECTED:
                                value_selected=calculate(conf,(*iter)->end(),SELECTED);
                                value_deselected=calculate(conf,(*iter)->end(),DESELECTED);
                                if(value_selected<value_deselected){
                                    if(max_min_selected<value_selected){
                                        max_min_selected=value_selected;
                                        index_max_min_selected=(*iter)->end()->get_index();
                                    }
                                }else{
                                    if(min_min_deselected>value_deselected){
                                        min_min_deselected=value_deselected;
                                        index_min_min_deselected=(*iter)->end()->get_index();
                                    }
                                }
                        }
                    if(at_least_one_selected==false){
                        int index_to_select;
                        if(index_min_min_deselected== -1)
                            index_to_select=index_max_min_selected;
                        else
                            index_to_select=index_min_min_deselected;
                        for(list<Link *>::iterator iter=children.begin();iter!=children.end();iter++)
                            if((conf[(*iter)->end()->get_index()])==UNSELECTED){
                                if((*iter)->end()->get_index()==index_to_select)
                                    minimum=min(minimum,calculate(conf,(*iter)->end(),SELECTED));
                                else
                                    minimum=min(minimum,calculate(conf,(*iter)->end(),DESELECTED));
                            }

                    }
            }
        }
    }

    if(state==DESELECTED)
        minimum=min(minimum,f->get_attribute(id_attribute_deselected));
    else
        minimum=min(minimum,f->get_attribute(id_attribute_selected));

    return minimum;
}
