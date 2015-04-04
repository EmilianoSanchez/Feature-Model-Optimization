#ifndef Feature_H
#define Feature_H

#include <iostream>
#include <list>
#include <map>

/*! \file Feature.h
 * \brief Definition of features of a Feature Model, they have the same attributes
 * as FAMILIAR ones, plus other ones for visiting them, computing metrics, etc.
 */

#include "FMElement.h"
#include "../ConfigurationAdaptor/SoftElement.h"
#include "HyperLink.h"
#include "PropertyMap.h"

// Forward declarations for the model(tree) organisation
class Link;
class HyperLink;
class SoftElement ;

/*! \class   Feature
 * \brief Represent one feature in a model, linked with features of FAMILIAR
 * They are implemented as a tree with links (optional/mandatory) and
 * hyperlinks (nodes OR,XOR, AND...)
 */

class Feature : public FMElement, public PropertyMap<int,double>
{
public :

    // Constructor
    Feature(const std::string& n="", unsigned int i=0, unsigned int weight=100)
	: _name(n),_decomposition(0), _link(0), _weight(weight),
	  _soft_element(0), _index(i)
    { }

    // Accessors/Observers
    HyperLink* decomposition() {return _decomposition;}
    std::string name() const {return _name;}
    Link* link()     {return _link; }

    unsigned int get_index() {return _index; }
    unsigned int get_weight() {return _weight; };

    HLTag get_hyperlink_type();
    Tag get_link_type();

    // Modifiers
    void add_link(Link *l)  {_link=l;  }
    void add_hyper_link(HyperLink* hl) {_decomposition = hl;}
    //! Add a sub feature
    void add_sub_feature(Feature* f, Tag t=none);
    //! Get sub-features
    //std::list<Feature *>& const get_sub_features();

    // Get/set the corresponding component/parameter if any
    SoftElement* get_soft() const { return _soft_element;   }
    void add_soft(SoftElement *se) {_soft_element=se;   }
    bool is_concrete(){return _soft_element!=NULL;  };

    // Add/remove the feature to/from the current configuration.
    //In fact, these methods are not used because the selection and deselection of features is performed by the methods "configure" and "reconfigure" of FeatureModel class,
    //and the start and stop of software components and parameters is performed by the ComponentManager.
    //void add(){};
    //void remove(){};

    friend std::ostream& operator<<(std::ostream& os, const Feature& fe)
    {
        os << "Feature " << fe.name() ;
        return os;
    }

    // Visitor pattern, (e.g. to translate into FAMILIAR, to compute weights ...)
    void accept(Visitor * v)
    {
	v->visit (this);
    }


private :
    std::string _name; //!< Unique name in the FM, = the feature in FAMILIAR
    HyperLink* _decomposition; //!< Connection to children sub features, if any
    Link* _link ;  //!< The link of which the feature is the end (null for root)
    unsigned int _weight; //! Weigth (0-100) of the feature, default 100
    unsigned int _index ; //! Index of feature to facilitate retrieval
    SoftElement *_soft_element; //!< Connected component or parameter, if any
};

// Lien avec FAMILIAR via une map ??

// Attributs simples: string, bool, numerique...range DISCRET pour le moment

#endif //Feature_H
