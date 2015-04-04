#include <map>
#include <iostream>

#include "Event.h"

using namespace std;

ostream& operator<<(ostream& os, const Event& e)
{
        os << " <EVENT> " << e.name<<". Afected features: ";
        for(list<pair<string,FMAction> >::const_iterator iter=e.fm_actions.begin();iter!=e.fm_actions.end();iter++){
            os<<iter->first<<"[";
            switch(iter->second){
                case FMAction::SELECT:
                    os<<"SELECT";break;
                case FMAction::DESELECT:
                    os<<"DESELECT";break;
                case FMAction::UNSELECT:
                    os<<"UNSELECT";
            }
            os<<"] ";
        }
        return os;
}


/*

//! \var Global map of event rules
map <string,reaction*> Event_rules;

// Scan the rules to find the one(s) triggered by an event
string react(const string& evt)
{
    map <string, reaction*>::iterator r = Event_rules.find(evt);
    if (r != Event_rules.end())
    {
	reaction *res = r->second ;

// No direct action, pass through FAMILIAR first
	switch (res->_action)
	{
	case SELECT :
	    //res->_feature->add();
	    break;
	case UNSELECT :
	    //res->_feature->?();
	    break;
	case DESELECT :
	    //res->_feature->remove();
	    break;
	default: cout << "UNKNOWN ACTION?!?"<<endl;
	    break;

	}
	return res->_familiar ;
    }
    else
	return "";
}

void to_Familiar(const string& evt)
{
    map <string, string>::iterator r = Event_rules.find(evt);
if (r != Event_rules.end()) // send command to Familiar
	  r->second ;
    else
	return;
}


//Fill the event rule map (done from external syntax by parser)
void add_rule(const string& evt, const string& fam,const action a, Feature *f)
{
    reaction *act = new reaction(fam,a,f);
    Event_rules[evt]= act;
}

*/
