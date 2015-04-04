#ifndef COMPONENTMANAGERSERVICEINTERFACE_H_INCLUDED
#define COMPONENTMANAGERSERVICEINTERFACE_H_INCLUDED

#include <string>
#include <list>
#include <map>

#include "Event.h"
#include "SoftElement.h"



class ComponentManager{

public:

    ComponentManager();
    ~ComponentManager();

    //Component manager operations:
    double initialize();
    double reconfigure(std::list<CMAction> &actions);
    double finalize();
    Event wait_for_event();

    //Observers:
    SoftElement *get_element(std::string name);
    SoftElementType get_type(std::string name);
    SoftElementState get_state(std::string name);
    double get_attribute(std::string name,int attribute);
    friend std::ostream& operator<<(std::ostream& os, ComponentManager& cm);

    //Modifiers used by the method 'load' to incorporate software elements and events to the component Manager used for testing purposes
    void add_software_element(SoftElement *soft_element);
    void add_event(Event &event);
    std::map<std::string,SoftElement *> &get_software_elements(){return software_elements;};
    std::list<Event> &get_events(){return event_list;};

private:

    std::map<std::string,SoftElement *> software_elements;
    std::list<Event> event_list;
    std::list<Event>::iterator event_iterator;

};



#endif // COMPONENTMANAGERSERVICEINTERFACE_H_INCLUDED
