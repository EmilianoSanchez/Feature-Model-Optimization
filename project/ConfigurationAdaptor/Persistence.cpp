#include "Persistence.h"
#include <string>
#include <map>

using namespace std;

//tokens:
#define COMPONENT   1
#define PARAMETER   2
#define EVENT       3
#define FEATURE     4
#define END         5

#define DESELECTED  0
#define SELECTED    1
#define UNSELECTED  2

#include "SoftElement.h"

map<string,int> tokens;
map<string,int> attributes;
map<string,FMAction> feature_actions;

map<int,string> iattributes;
map<FMAction,string> ifeature_actions;

void initialize_dictionaries(){
    tokens["<COMPONENT>"]=COMPONENT;
    tokens["<PARAMETER>"]=PARAMETER;
    tokens["<EVENT>"]=EVENT;
    tokens["<FEATURE>"]=FEATURE;
    tokens["<END>"]=END;

    //Software element attributes
    attributes["START_TIME"]=START_DELAY;
    attributes["STOP_TIME"]=STOP_DELAY;
    attributes["EXECUTION_TIME"]=EXECUTION_TIME;
    attributes["ACCURACY"]=ACCURACY;
    attributes["MEMORY_CONSUMPTION"]=MEMORY_CONSUMPTION;

    iattributes[START_DELAY]="START_TIME";
    iattributes[STOP_DELAY]="STOP_TIME";
    iattributes[EXECUTION_TIME]="EXECUTION_TIME";
    iattributes[ACCURACY]="ACCURACY";
    iattributes[MEMORY_CONSUMPTION]="MEMORY_CONSUMPTION";

    feature_actions["SELECT"]=FMAction::SELECT;
    feature_actions["DESELECT"]=FMAction::DESELECT;
    feature_actions["UNSELECT"]=FMAction::UNSELECT;

    ifeature_actions[FMAction::SELECT]="SELECT";
    ifeature_actions[FMAction::DESELECT]="DESELECT";
    ifeature_actions[FMAction::UNSELECT]="UNSELECT";
}

bool load(ComponentManager &cm,FeatureModel &fm,string file_name){

    //!The dictionaries are needed to map strings in the files to integers used by the application
    initialize_dictionaries();

    ifstream fichero(file_name);

    string token;
    fichero >>token;
    while(!fichero.eof()){
        SoftElement *new_elem;
        Feature *f;
        Event event;
        string name,component,feature,attribute,feature_action;
        double value;

        switch(tokens[token]){
            case COMPONENT:
                fichero>>name;
                new_elem=new Component(name);
                fichero>>attribute;
                while(tokens[attribute]==0){
                    fichero>>value;
                    new_elem->set_attribute(attributes[attribute],value);
                    fichero>>attribute;
                }
                cm.add_software_element(new_elem);
                token=attribute;
                break;
            case PARAMETER:
                fichero>>name>>component;
                new_elem=new Parameter(name,dynamic_cast<Component*>(cm.get_element(component)));
                fichero>>attribute;
                while(tokens[attribute]==0){
                    fichero>>value;
                    new_elem->set_attribute(attributes[attribute],value);
                    fichero>>attribute;
                }
                cm.add_software_element(new_elem);
                token=attribute;
                break;
            case EVENT:
                fichero>>name>>feature;
                event.name=name;
                event.fm_actions.clear();
                while(tokens[feature]==0){
                    fichero>>feature_action;
                    event.fm_actions.push_back(make_pair(feature,feature_actions[feature_action]));
                    fichero>>feature;
                }
                cm.add_event(event);
                token=feature;
                break;
            case FEATURE:
                fichero>>feature>>component;
                f=fm.get_feature(fm.get_feature_index(feature));
                if(component!="NULL")
                    f->add_soft(cm.get_element((component)));
                fichero>>attribute;
                while(tokens[attribute]==0){
                    fichero>>value;
                    f->set_attribute(attributes[attribute],value);
                    fichero>>attribute;
                }
                token=attribute;
                break;

            case END:
                fichero>>token;

        }
    }
    fichero.close();

}

bool save(ComponentManager &cm,FeatureModel &fm,string file_name){

    ofstream fichero(file_name);

    /**to generate a test_case file, first all components are added, then parameters, events and finally features,
     * because the load method must read them in that order since parameters reference components and features reference parameters and components
     */

    //add Components
    for(map<string,SoftElement *>::iterator iter=cm.get_software_elements().begin();iter!=cm.get_software_elements().end();iter++)
        if(iter->second->get_type()==COMPONENT){
            fichero<<"<COMPONENT>\n";
            fichero<<iter->first<<"\n";
            for(map<int,double>::const_iterator attributes=(iter->second->get_attributes()).begin();attributes!=(iter->second->get_attributes()).end();attributes++)
                fichero<<iattributes[attributes->first]<<" "<<attributes->second<<"\n";
            fichero<<"\n";
        }
    //add Parameters
    for(map<string,SoftElement *>::iterator iter=cm.get_software_elements().begin();iter!=cm.get_software_elements().end();iter++)
        if(iter->second->get_type()==PARAMETER){
            fichero<<"<PARAMETER>\n";
            Parameter *p=dynamic_cast<Parameter *>(iter->second);
            fichero<<iter->first<<" "<<p->get_component()->get_name() <<"\n";
            for(map<int,double>::const_iterator attributes=(p->get_attributes()).begin();attributes!=(p->get_attributes()).end();attributes++)
                fichero<<iattributes[attributes->first]<<" "<<attributes->second<<"\n";
            fichero<<"\n";
        }

    //add events
    for(list<Event>::iterator iter=cm.get_events().begin();iter!=cm.get_events().end();iter++){
        fichero<<"<EVENT>\n"<<iter->name<<"\n";
        for(list<pair<string,FMAction> >::iterator actions=(iter->fm_actions).begin();actions!=(iter->fm_actions).end();actions++)
            fichero<<actions->first<<" "<<ifeature_actions[actions->second]<<"\n";
        fichero<<"\n";
    }

    //add features
    for(int i=0;i<fm.nb_features();i++){
        Feature *f=fm.get_feature(i);
        fichero<<"<FEATURE>\n";
        fichero<<f->name()<<" ";
        if(f->get_soft()==NULL)
            fichero<<"NULL \n";
        else
            fichero<<f->get_soft()->get_name()<<"\n";

        for(map<int,double>::const_iterator attributes=(f->get_attributes()).begin();attributes!=(f->get_attributes()).end();attributes++){
            if(iattributes.find(attributes->first)==iattributes.end()){
                if(iattributes.find(-(attributes->first))==iattributes.end())
                    fichero<<"Attribute_"<<attributes->first;
                else
                    fichero<<iattributes[-(attributes->first)]<<"_IF_DESELECTED";
            }else
                fichero<<iattributes[attributes->first];
            fichero<<" "<<attributes->second<<"\n";
        }
        fichero<<"\n";

    }

    fichero.close();

}
