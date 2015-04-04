#ifndef SoftElement_H
#define SoftElement_H

#include <iostream>
#include <string>

#include "../FeatureModelFramework/PropertyMap.h"

/*! \file SoftElement.h
 * \brief Software elements that are represented by features (i.e. full components
 *  or parameters/options of a component) in a Feature Model
 */

/*! \enum
 * \brief Enum for all things that can be associated with a feature
 */
enum SoftElementType{
    COMPONENT=1,
    PARAMETER=2
};

/*! \enum
 * \brief Enum for all runtime states that can be associated to software elements
 */
enum SoftElementState{
    STARTED,/**<Component state*/
    STOPPED,/**<Component state*/
    ACTIVATED,/**<Parameter state*/
    DESACTIVATED/**<Parameter state*/
};

/*! \class SoftElement
 * \brief Abstract class for all things that can be associated with a feature
 */
class SoftElement: public PropertyMap<int,double>{
protected :
    // (default) constructor
    SoftElement (std::string n,SoftElementType t,SoftElementState s)
	: name(n),type(t),state(s)
    {}

    std::string name ; // name of element (+-same as corresponding feature)

    SoftElementType type;
    SoftElementState state;

public:
    // Useful general methods

    std::string get_name() const {return name;}

    SoftElementType get_type(){return type;};
    SoftElementState get_state(){return state;};

    friend std::ostream& operator<<(std::ostream& os, const SoftElement& ce);


    // At runtime: this element can be added or removed (what does that means
    // depend on the sub-classes, Component or Parameter)
    virtual void add() =0;
    virtual void remove() =0;

};

/*! \class Component: abstract class
 * \brief A component representation linked to an executable (e.g. OSGi) one,
 * and (possibly) to a feature in a Feature Model. Its sub-classes (components
 * of an application) will have attributes/parameters (aka slots, with range and
 * default value)
 */

class Component : public SoftElement
{
public :

    Component (std::string n="")
	: SoftElement (n,COMPONENT,STOPPED) // weigth of component set to 5.... CHANGE
    {}

    // For components, add and remove correspond to start and stop methods,
    // that launch (resp. terminate) the corresponding (OSGi) component
    void add() {
        //std::cout << "   Start component " << name()<<std::endl;
        state=STARTED;
    }
    void remove() {
        //std::cout << "   Stop component " << name()<<std::endl;
        state=STOPPED;
    }

};


// Composites a voir

/*! \class Parameter
 * \brief parameter representation (can work for options too?)
 * It should usually have a numeric/... equivalent to be used in practice
 */

class Parameter : public SoftElement
{
public :

    Parameter (std::string n="", Component* co=0) //weight =1 for parameters
	: SoftElement (n,PARAMETER,DESACTIVATED), component(co)
    {}

    // For parameters/options, add and remove correspond to changing a value
    // in the calling syntax of the corresponding component
    // remove() does nothing, add() just replaces the previous value by the new one
    void add() {
        //std::cout << "    Parameter value= " << name()<<std::endl;
        state=ACTIVATED;
    }
    void remove() {
        //std::cout << "    Not selected parameter= "<< name()<<std::endl;
        state=DESACTIVATED;
    }

    Component *get_component(){return component;};

private :
    Component* component; // The component the parameter belongs to
};


#endif //SoftElement_H
