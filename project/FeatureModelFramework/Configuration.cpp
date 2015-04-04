
#include "HyperLink.h"
#include "Constraint.h"
#include "Configuration.h"
using namespace std;

bool Configuration::apply_assignment(Assignment &assignment){

    /* This method apply each individual assignment in order.
     * If one of them fails, the complete process fails.
     */
    for(int i=0;i<assignment.size();i++){
        switch(assignment[i].second){
            case SELECTED:
                if(!this->apply_selection(assignment[i].first))
                    return false;
                break;
            case DESELECTED:
                if(!this->apply_deselection(assignment[i].first))
                    return false;
                break;
        }
    }
    return true;
}

bool Configuration::apply_selection(unsigned int feature_index){

    //assignment of the new state:
    switch(features[feature_index]){
        //If the feature state is SELECTED, it means that it was selected in the the original partial configuration (given by FAMILIAR) or in a triggered constraint before
        //The process can finish successfully
        case SELECTED:
            return true;

        //Is the state is UNSELECTED, the assignment is performed
        case UNSELECTED:
            features[feature_index]=SELECTED;
            break;

        //If the feature is DESELECTED, it means that the process reach an invalid configuration.
        //The process finishs with failure
        case DESELECTED:
            return false;
    }

    //tree constraint propagation rules:

    Feature *f= get_feature(feature_index);

    //If the feature is not the root ...
    if(f->get_link_type()!=Tag::root){
        //its parent feature must be selected
        if(!apply_selection(f->link()->back()->start()->get_index()))
            return false;

        //if it is part of a XOR group, all its brothers (features that belong to the same group) must be deselected
        //If the deselection of one of them fails, the complete process fails.
        if(f->link()->back()->start()->get_hyperlink_type()==HLTag::xor_group){
            list<Link *> children=f->link()->back()->start()->decomposition()->links();
            for(list<Link *>::iterator c= children.begin();c!=children.end();c++){
                if((*c)->end()->get_index()!=feature_index && !apply_deselection((*c)->end()->get_index()))
                    return false;
            }
        }
    }

    //If the feature has an AND_GROUP, all its mandatory children must be selected
    //If the selection of one of them fails, the complete process fails.
    if(f->get_hyperlink_type()==HLTag::and_group){
        list<Link *> children=f->decomposition()->links();
        for(list<Link *>::iterator c= children.begin();c!=children.end();c++){
            if((*c)->get_type()==Tag::mandatory)
                if(!apply_selection((*c)->end()->get_index()))
                    return false;
        }
    }

    //cross-tree constraint propagation rules:

    list<Clause> constraints = fm->get_constraints_CNF(feature_index);
    // In this section, the satisfiability of all clauses are verified one by one and new assignments can be propagated. This code supports N-SAT clauses
    for(list<Clause>::iterator it=constraints.begin();it!=constraints.end();it++){
        if((*it)[feature_index]==true){
            //Do nothing because as the feature is selected, the clause is satisfied
        }else{
            //At least one of the rest of the literals in the clause has to be TRUE.
            //That means that if no assigned literals(SELECTED or DESELECTED features) satisfy the clause, and only one feature remains unselected,
            //a concrete value is assigned to this last in order to satisfy the clause.
            //Otherwise (if no features remain unselected) the process fail because the clause cannot be satisfied.
            list<pair<unsigned int,bool> > unselected;
            bool at_least_one_true=false;

            for(Clause::iterator itc=it->begin();itc!=it->end();itc++){
                if(itc->first!=feature_index){
                    if(features[itc->first]==UNSELECTED)
                        unselected.push_back(*itc);
                    else
                        if(features[itc->first]==itc->second){
                            at_least_one_true=true;
                            break;
                        }
                }
            }

            if(at_least_one_true==false){
                switch(unselected.size()){
                    case 0:
                        return false;
                    case 1:
                        pair<unsigned int,bool> aux=unselected.front();
                        if( aux.second ){
                            if(!apply_selection(aux.first))
                                return false;
                        }else{
                            if(!apply_deselection(aux.first))
                                return false;
                        }
                }
            }
        }
    }

    return true;
};

bool Configuration::apply_deselection(unsigned int feature_index){

    //assignment of the new state:
    switch(features[feature_index]){
        //If the feature is SELECTED, it means that the process reach an invalid configuration.
        //The process finishs with failure
        case SELECTED:
            return false;

        //Is the state is UNSELECTED, the assignment is performed
        case UNSELECTED:
            features[feature_index]=DESELECTED;
            break;

        //If the feature state is DESELECTED, it means that it was deselected in the the original partial configuration (given by FAMILIAR) or in a triggered constraint before
        //The process can finish successfully
        case DESELECTED:
            return true;
    }

    //tree constraint propagation rules:

    Feature *f= get_feature(feature_index);

    //If the feature is not a leaf, all its children must be deselected
    //If the deselection of one of them fails, the complete process fails.
    if(f->get_hyperlink_type()!=HLTag::leaf){
        list<Link *> children=f->decomposition()->links();
        for(list<Link *>::iterator c= children.begin();c!=children.end();c++)
            if(!apply_deselection((*c)->end()->get_index()))
                return false;
    }


    switch(f->get_link_type()){
        case Tag::root:
            //do nothing
            break;

        case Tag::optional:
            //do nothing
            break;

        //If the feature is mandatory, its parent feature must be deselected too
        case Tag::mandatory:
            if(!apply_deselection(f->link()->back()->start()->get_index()))
                return false;
            break;

        case Tag::none://none means that the feature belongs to a XOR or OR group
            /*In this section is performed the XOR/OR inference following two rules:
             * -if the feature to be deselected is part of a XOR/OR group and all its brothers are deselected, its parent must be deselected.
             * -if the feature to be deselected is part of a XOR/OR group, its parent is selected, there aren't selected features and only one feature remains unselected among its brothers,
             * this last one must be selected to satisfied the XOR/OR condition.
             */

            FeatureState parentState=features[f->link()->back()->start()->get_index()];
            if(parentState!=DESELECTED){
                int amount_selected=0;
                int amount_unselected=0;
                unsigned int unselected_feature;
                list<Link *> children=f->link()->back()->start()->decomposition()->links();
                for(list<Link *>::iterator c= children.begin();c!=children.end();c++){
                    switch(features[(*c)->end()->get_index()]){
                        case SELECTED:
                            amount_selected++;
                            break;
                        case UNSELECTED:
                            amount_unselected++;
                            unselected_feature=(*c)->end()->get_index();
                    }
                }
                if(parentState==SELECTED){
                    if(amount_selected==0){
                        switch(amount_unselected){
                            case 0:
                                return false;
                            case 1:
                                if(!apply_selection(unselected_feature))
                                    return false;
                        }
                    }
                }else{//parentState==UNSELECTED
                    if(amount_unselected==0 && !apply_deselection(f->link()->back()->start()->get_index()))
                        return false;
                }
            }
    }



    //cross-tree constraint propagation rules:
    list<Clause> constraints = fm->get_constraints_CNF(feature_index);

    // In this section, the satisfiability of all clauses are verified one by one and new assignments can be propagated. This code supports N-SAT clauses
    for(list<Clause>::iterator it=constraints.begin();it!=constraints.end();it++){
        if((*it)[feature_index]==false){
            //Do nothing because as the feature is deselected, the clause is satisfied
        }else{
            //At least one of the rest of the literals in the clause has to be TRUE.
            //That means that if no assigned literals(SELECTED or DESELECTED features) satisfy the clause, and only one feature remains unselected,
            //a concrete value is assigned to this last in order to satisfy the clause.
            //Otherwise (if no features remain unselected) the process fail because the clause cannot be satisfied.
            list<pair<unsigned int,bool> > unselected;
            bool at_least_one_true=false;

            for(Clause::iterator itc=it->begin();itc!=it->end();itc++){
                if(itc->first!=feature_index){
                    if(features[itc->first]==UNSELECTED)
                        unselected.push_back(*itc);
                    else
                        if(features[itc->first]==itc->second){
                            at_least_one_true=true;
                            break;
                        }
                }
            }

            if(at_least_one_true==false){
                switch(unselected.size()){
                    case 0:
                        return false;
                    case 1:
                        pair<unsigned int,bool> aux=unselected.front();
                        if( aux.second ){
                            if(!apply_selection(aux.first))
                                return false;
                        }else{
                            if(!apply_deselection(aux.first))
                                return false;
                        }
                }
            }
        }
    }

    return true;

};

ostream & operator<<(ostream &stream, Configuration &c){

  for(int i=0;i<c.nb_features();i++){
    stream<<c.get_feature(i)->name()<<" ";
    switch(c[i]){
        case SELECTED:
            stream<<"SELECTED\n";
            break;
        case DESELECTED:
            stream<<"DESELECTED\n";
            break;
        case UNSELECTED:
            stream<<"UNSELECTED\n";
            break;
    }
  }
  return stream;
};


