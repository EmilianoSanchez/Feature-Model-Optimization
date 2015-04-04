#ifndef Visitor_H
#define Visitor_H

/*! \file  Visitor.h
 * \brief Different sorts of visitors  for feature models to display, translate,
 * compute information...
 */

#include <fstream>
#include <iostream>
#include <map>
#include <boost/dynamic_bitset.hpp>

class Feature;
class Link;
class OptLink;
//class HyperLink;
class AND_HyperLink;
class OR_HyperLink;
class XOR_HyperLink;
class Imply;
class Exclude;
class MutualImplication;
class MutualExclusion;
class Constraint;
class FeatureModel;

/*! \class Visitor
 * \brief Abstract class for all visitors
 */
class Visitor
{
  public :
    virtual void visit (Feature*)= 0;
    virtual void visit (Link*)= 0;
    virtual void visit (OptLink*)= 0;
    //virtual void visit(HyperLink*)= 0;
    virtual void visit(OR_HyperLink*)= 0;
    virtual void visit(XOR_HyperLink*)= 0;
    virtual void visit(AND_HyperLink*)= 0;
    virtual void visit(Imply*)= 0;
    virtual void visit(Exclude*)= 0;
    virtual void visit(MutualImplication*)= 0;
    virtual void visit(MutualExclusion*)= 0;
    virtual void visit(FeatureModel*)=0;

};

/*! \class VisitorFamiliar
 * \brief (backward) Translation into FAMILIAR
 */
class VisitorFamiliar : public Visitor
{
    void visit (Feature* fe);
    void visit (Link* l);
    void visit (OptLink* ol);
    //void visit (HyperLink* hl) {}
    void visit(AND_HyperLink* hl);
    void visit(OR_HyperLink* hl);
    void visit(XOR_HyperLink* hl);
    void visit(Imply*);
    void visit(Exclude*);
    void visit(MutualImplication*);
    void visit(MutualExclusion*);
    void visit(FeatureModel*);
};

/*! \class VisitorFamiliar
 * \brief Translation into an .fml file
 */
class VisitorFMLFile : public Visitor
{
private:
    std::fstream fichero;

public:

    VisitorFMLFile(std::string file);
    ~VisitorFMLFile();
    void visit (Feature* fe);
    void visit (Link* l);
    void visit (OptLink* ol);
    //void visit (HyperLink* hl) {}
    void visit(AND_HyperLink* hl);
    void visit(OR_HyperLink* hl);
    void visit(XOR_HyperLink* hl);
    void visit(Imply*);
    void visit(Exclude*);
    void visit(MutualImplication*);
    void visit(MutualExclusion*);
    void visit(FeatureModel*);
};

/*
 //!\class VisitorBitset
 //! \brief Translation into a bitset for some heuristics computations

class VisitorBitset : public Visitor
{
public:
// Constructor
    VisitorBitset(const int n) : bbyte (boost::dynamic_bitset<>(n))
    {}

    boost::dynamic_bitset<> bbyte;

private:

    void visit (Feature* fe);
    void visit (Link* l);
    void visit (OptLink* ol);

    void visit(AND_HyperLink* hl) ;
    void visit(OR_HyperLink* hl);
    void visit(XOR_HyperLink* hl);
    void visit(Imply*);
    void visit(Exclude*);
    void visit(MutualImplication*);
    void visit(MutualExclusion*);
    void visit(FeatureModel*);
};


// temporary functions for test
boost::dynamic_bitset<> test_Visitorbitset(FeatureModel&);
void test_VisitorFamiliar(FeatureModel&);
*/

#endif //Visitor_H
