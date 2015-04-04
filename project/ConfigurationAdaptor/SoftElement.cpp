#include "SoftElement.h"

using namespace std;

ostream& operator<<(ostream& os, const SoftElement& ce){
        switch(ce.type){
            case COMPONENT:
                os << " <COMPONENT> ";
                break;
            case PARAMETER:
                os << " <PARAMETER> ";
                break;
            default:
                os << " <SOFT_ELEMENT> ";
        }
        os<< ce.get_name() ;
        for(map<int,double>::const_iterator it=ce.get_attributes().begin();it!=ce.get_attributes().end();it++)
            os<<". Attribute "<<it->first<<" Value "<<it->second;
        return os;
}


// The global component repository (declare static member)
//map<string, Component> Component::repository;

// Create a new component (by parsing declaration file), insert it in repository
// and connect it with its feature
/*Component* create_component(string n, set<string> evts,Feature* f)
{
    Component *comp =new Component (n,evts,f);
    Component::repository [n]= *comp;
    if (f) f->add_soft(comp);
    return comp;
}*/

// Just a test function, to be removed
/*void test_Component()
{
    cout << "Stored components: " ;
    map<string, Component>::iterator it;
    for (it = Component::repository.begin();
	 it != Component::repository.end();++it)
	cout << (*it).second.name() << " " ;
    cout <<endl;
}*/

// Create a new parameter (by parsing declaration file)
// and connect it with its feature and its component
/*
void create_parameter(string n, Feature* f, Component* co)
{    Parameter *par =new Parameter (n,f,co);
    //Parameter::repository [n]= *par;
    if (f) f->add_soft(par);
}*/
