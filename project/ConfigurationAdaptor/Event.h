#ifndef Event_H
#define Event_H

#include <list>
#include <iostream>

//Feature model actions:
typedef enum {
    DESELECT=0,
    SELECT=1,
    UNSELECT=2 } FMAction;

/** An event is the object triggered by the component manager and represents context changes.
 *  It has a name and a list of feature model actions (selections, deselections and unselections of features)
 *  that are performed by FAMILIAR in order to update the partial configuration that represents the current variability of the system
 */
class Event{
public:
    std::string name;
    std::list<std::pair<std::string,FMAction> > fm_actions;//Pair<feature_name,feature_action>

    Event(){};

    Event(std::string _name,std::list<std::pair<std::string,FMAction> > _fm_actions):name(_name),fm_actions(_fm_actions){}

    friend std::ostream& operator<<(std::ostream& os, const Event& e);
};

enum CMActionType{
    ADD,
    REMOVE,
    START_COMPONENT,
    STOP_COMPONENT,
    ACTIVATE_PARAMETER,
    DESACTIVATE_PARAMETER
};

/** A CMAction is a primitive instruction for the component manager to reconfigure the processing chain or any other component-based system.
 *  It consists on the name of the software element to modify and a type that indicates what to do with that element.
 *  To perform more advanced reconfigurations of the system, a list of actions is generated and given to the component manager by the mediator object.
 */
class CMAction{
public:
    std::string software_element_name;
    CMActionType type;

    CMAction(std::string _software_element_name,CMActionType _type):software_element_name(_software_element_name),type(_type){};
};




/*! \file Event.h
 * \brief Definition of rules relating external events (coming from low level to
 * FM actions (selection/deselection of some features)
 * These rules are defined by experts
 */
/*
#include "Feature.h"

typedef enum {UNSELECT,SELECT, DESELECT} action;

struct reaction
{
    std::string  _familiar; //FAMILIAR code to react to event (feature (de)selection)
    action _action;
    Feature *_feature ; //the affected feature

    reaction(const std::string fam, const action ac, Feature *f)
	: _familiar(fam), _action(ac), _feature(f)
    {}
};

// External functions, used during parsing for instance
void add_rule(const std::string&,const std::string&,const action, Feature*);
std::string react(const std::string&);
*/


#endif
