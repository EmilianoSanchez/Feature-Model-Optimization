#include "ComponentManagerServiceInterface.h"
#include "SoftElement.h"

using namespace std;

#include <fstream>



ComponentManager::ComponentManager(){};

ComponentManager::~ComponentManager(){
    for(map<string,SoftElement *>::iterator elements=software_elements.begin();elements!=software_elements.end();elements++)
        if((*elements).second!=NULL)
            delete (*elements).second;
};

double ComponentManager::initialize(){
    event_iterator=event_list.begin();
    return 0.0;
};

double ComponentManager::reconfigure(list<CMAction> &actions){
    double time=0.0;
    cout<<" *** RECONFIGURATION OF COMPONENT MANAGER. "<<actions.size()<<" component actions."<<endl;
    for(list<CMAction>::iterator iter=actions.begin();iter!=actions.end();iter++){
        if(software_elements[iter->software_element_name]==NULL)
            cout<<iter->software_element_name<<" doesn't exist"<<endl;
        else{
            cout<<"Action:";
            switch(iter->type){
                case ADD:
                    software_elements[iter->software_element_name]->add();
                    cout<<" add software element";
                    break;
                case REMOVE:
                    software_elements[iter->software_element_name]->remove();
                    cout<<" remove software element";
                    break;
                case START_COMPONENT:
                    software_elements[iter->software_element_name]->add();
                    cout<<" start component";
                    break;
                case STOP_COMPONENT:
                    software_elements[iter->software_element_name]->remove();
                    cout<<" stop component";
                    break;
                case ACTIVATE_PARAMETER:
                    software_elements[iter->software_element_name]->add();
                    cout<<" activate parameter";
                    break;
                case DESACTIVATE_PARAMETER:
                    software_elements[iter->software_element_name]->remove();
                    cout<<" desactivate parameter";
                    break;
            }
            cout<<iter->software_element_name<<endl;
        }
    }
    return time;
};

double ComponentManager::finalize(){

};

SoftElement *ComponentManager::get_element(string name){
    return software_elements[name];
};

SoftElementType ComponentManager::get_type(string name){
    return get_element(name)->get_type();
};

SoftElementState ComponentManager::get_state(string name){
    return get_element(name)->get_state();
};

double ComponentManager::get_attribute(string name,int attribute){
    return get_element(name)->get_attribute(attribute);
};

Event ComponentManager::wait_for_event(){
    if(event_iterator==event_list.end())
        return Event();
    else{
        Event e = (*event_iterator);
        event_iterator++;
        return e;
    }
};

std::ostream& operator<<(std::ostream& os, ComponentManager& cm)
{
    os<<"Software elements: \n";
    for(map<string,SoftElement *>::iterator elements=cm.software_elements.begin();elements!=cm.software_elements.end();elements++)
        if((*elements).second!=NULL)
            os<<*(elements->second)<<"\n";
    os<<"List of events: \n";
    for(list<Event>::iterator events=cm.event_list.begin();events!=cm.event_list.end();events++)
        os<<*events<<"\n";
    return os;
}

void ComponentManager::add_software_element(SoftElement *soft_element){
    software_elements[soft_element->get_name()]=soft_element;
}

void ComponentManager::add_event(Event &event){
    event_list.push_back(event);
}
