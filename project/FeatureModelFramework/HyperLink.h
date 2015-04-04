#ifndef HyperLink_H
#define HyperLink_H

/*! \file  HyperLink.h
 * This file contains the definition of class HyperLink (top) and its sub classes
 *    Disjunction, Conjunction, etc. for graphs & models
 */

// DO NOT MODIFY


#include <utility>

#include <string>
#include <list>

#include <iostream>

#include "FMElement.h"
#include "Feature.h"


/*! \class  HyperLink
 * \brief Represent the connections from one node (AND, OR, XOR..) to its children
 */

class HyperLink : public FMElement
{
public :
    // Accessors
    Feature* start() {return _start;}
    std::list<Link *>& links() {return _links ;}

    // Constructors
    HyperLink(Feature* fe) : _start(fe)
    { }

    HyperLink(Feature* fe, std::list<std::pair<Feature*,Tag>>& lf) : _start(fe)
    {
	for (std::list<std::pair<Feature*,Tag>>::iterator it = lf.begin();
         it!=lf.end(); it++)
	    add_feature(it->first, it->second);
    }


    //! Add a link (corresponding to the Tag) to a new child in the hyperlink
    void add_feature(Feature* f, Tag t = none);

    virtual bool bitvalue() {return true;}

    //virtual void accept (Visitor *v){}

    virtual HLTag get_type() = 0;

protected :
    Feature* _start; //!< Origin feature node
    std::list<Link *> _links; //!< Links connecting the node to its children
};

// Sub classes of HyperLink

/*! \class OR
 * \brief Represent (non exclusive) alternatives in a model
 *
 * One or several sub-features can be selected
 */

class OR_HyperLink : public HyperLink
{
public :
    //  Constructors
    OR_HyperLink(Feature* fe, std::list<std::pair<Feature*,Tag>>& lf)
	: HyperLink(fe,lf)//, _choice_criterium(0)
    {}
    OR_HyperLink(Feature* fe)
	: HyperLink(fe)
    {}

    //Executable* choice_criterium() {return _choice_criterium;}

    friend std::ostream& operator<< (std::ostream& os, const OR_HyperLink& hl)
    {
        os << "OR " ;
        return os;
    }

    void accept (Visitor *v) {v->visit (this);}
    bool bitvalue() {return false;}

    HLTag get_type(){
        return or_group;
    }

//private:
    //Executable* _choice_criterium ;//!< Indicate how to choose among alternatives
};

class XOR_HyperLink : public HyperLink
{
public :
    //  Constructors
    XOR_HyperLink(Feature* fe, std::list<std::pair<Feature*,Tag>>& lf)
	: HyperLink(fe,lf)//, _choice_criterium(0)
    {}
    XOR_HyperLink(Feature* fe)
	: HyperLink(fe)
    {}
    friend std::ostream& operator<< (std::ostream& os, const XOR_HyperLink& hl)
    {
        os << "XOR " ;
        return os;
    }

    void accept (Visitor *v) {v->visit (this);}

    bool bitvalue() {return false;}

    HLTag get_type(){
        return xor_group;
    }
//private:
    //Executable* _choice_criterium ;//!< Indicate how to choose among alternatives
};

/*! \class AND
 * \brief The order of children is not relevant, all non optional
 *  children must be selected
 */

class AND_HyperLink : public HyperLink
{
  public :
    //  Constructors
    AND_HyperLink(Feature* fe, std::list<std::pair<Feature*,Tag>>& lf)
	:  HyperLink(fe,lf)   {}

    AND_HyperLink(Feature* fe)
	:  HyperLink(fe)   {}

    friend std::ostream& operator<< (std::ostream& os, const AND_HyperLink& hl)
    {
        os << "AND " ;
        return os;
    }

    void accept (Visitor *v){v->visit (this);}

    HLTag get_type(){
        return and_group;
    }
};



/*! \class  Link
 * \brief An individual link, connects a node and a feature
 */

class Link : public FMElement
{
  public :
    //   Accessors
   Feature* end() const {return _end;}
   HyperLink* back () const {return _back;}

    virtual ~Link() {}

    //  Constructors
    Link(Feature* end,HyperLink* hl=0) :_end(end),_back(hl)  {}

    // What to do when you traverse a link
    //virtual void traverse()//<! Methods: traverse
    //{ // Default: selection of target feature
    //	_end->select();
    //};

    // Visitor pattern, (e.g. to translate into FAMILIAR)
    void accept(Visitor * v)
    {
	v->visit (this);
    }

    //Connect to a parent trough an hyperlink (OR,XOR,AND)
    void connect (HyperLink *hp) {_back=hp;}

    /*friend std::ostream& operator<< (std::ostream& os, const Link& li)
    {
        os << "Link " << *(li.end());// << " and " << li.back()
        return os;
    }*/

    virtual Tag get_type(){
        return none;
    }

  protected :
    Feature* _end; //!< feature at end of link
    HyperLink* _back; //!< Back reference to parent (via hyperlink)
};

/*! \class  OptLink
 * \brief Optional link, the end features may not be selected
 */
class OptLink : public Link
{
  public :

    //  Constructors
    OptLink(Feature* end,HyperLink* hl=0) : Link(end,hl)  {}

    // Visitor pattern, (e.g. to translate into FAMILIAR)
    void accept(Visitor * v)
    { //std::cout << "accept Opt "<< std::endl;
	v->visit (this);
    }

    /*friend std::ostream& operator<< (std::ostream& os, const OptLink& li)
    {
        os << "Optional Link " << *(li.end());// << " and " << li.back()
        return os;
    }*/

    Tag get_type(){
        return optional;
    }
};

class MandLink : public Link
{
  public :

    //  Constructors
    MandLink(Feature* end,HyperLink* hl=0) : Link(end,hl)  {}

    /*friend std::ostream& operator<< (std::ostream& os, const MandLink& li)
    {
        os << "Mandatory Link " << *(li.end());// << " and " << li.back()
        return os;
    }*/

    Tag get_type(){
        return mandatory;
    }
};

#endif //HyperLink_H
