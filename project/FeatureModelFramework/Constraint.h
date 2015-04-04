#ifndef Constraint_H
#define Constraint_H

/*! \file Constraint.h
 *  \brief Simple constraints e.g., A => B (and C) ; B excludes D, as found
 * in FAMILIAR, but with only ONE left element for the moment (A|B ->c has to
 * be decomposed into two constraints: A->c and B->C)
 * We just memorize that there is a relation between left and right parts.
 * The real operations on constraints are done in FAMILIAR
 */

#include <list>
#include <map>
#include <set>
#include <iostream>

#include "FMElement.h"
#include "Feature.h"

/**A clause in CNF can be modeled by a map where each literal/variable, identified by the index of the feature,
 * has associated a boolean value that depends if the literal is negated or not
 */
typedef std::map<unsigned int,bool> Clause;

/*! \class Constraint (abstract class)
 * \brief A constraint is composed of a left part (trigger) = name of feature(s)
 * and a right part (names of features influenced)
 * The type of constraint (implies, excludes,...) is determined by the sub-classes
 * The sub-classes must implement the method getCNF to transform the constraint into a list of clauses in conjunctive normal form.
 * This method is used by Configuration class to verify the satisfiability of cross-tree constraints.
 */

class Constraint : public FMElement
{
public :
    //Constraint(std::string n, Feature* l, std::list<Feature*> r) :
    Constraint(Feature* l, std::list<Feature*> r) : _left(l), _right(r)
    {}

    Feature* left() {return _left;}
    std::list<Feature*> &right() {return _right;}

    virtual std::list<Clause> getCNF()=0;

protected :
    //std::string _name; //!< unique name ???
    Feature* _left; //left part (e.g. of the ->)
    std::list<Feature*> _right; //right part (impacted features)
};

/*! \class Imply
 * \brief Sub-class for implies constraints
 */
class Imply : public Constraint
{
public :

    Imply (Feature* l, std::list<Feature*> r) : Constraint (l, r)
    {}

    // Visitor pattern, (e.g. to translate into FAMILIAR)
    void accept(Visitor * v)
    {
	v->visit (this);
    }

    std::list<Clause> getCNF();

};

/*! \class Exclude
 * \brief Sub-class for exclude constraints
 */
class Exclude : public Constraint
{
public :

    Exclude(Feature* l, std::list<Feature*> r) : Constraint (l, r)
    {}

    // Visitor pattern, (e.g. to translate into FAMILIAR)
    void accept(Visitor * v)
    {
	v->visit (this);
    }

    std::list<Clause> getCNF();

};

/*! \class MutualExclusion
 * \brief Sub-class for "Mutual-exclusion" constraints, A<->!B = A xor B
 */
class MutualExclusion : public Constraint
{
public :

    MutualExclusion(Feature* l, Feature* r) : Constraint (l, {r})
    {}

    // Visitor pattern, (e.g. to translate into FAMILIAR)
    void accept(Visitor * v)
    {
	v->visit (this);
    }

    std::list<Clause> getCNF();

};

/*! \class MutualExclusion
 * \brief Sub-class for "Mutual-implication" constraints, A <-> B
 */
class MutualImplication : public Constraint
{
public :

    MutualImplication(Feature* l, Feature* r) : Constraint (l, {r})
    {}

    // Visitor pattern, (e.g. to translate into FAMILIAR)
    void accept(Visitor * v)
    {
	v->visit (this);
    }

    std::list<Clause> getCNF();

};

#endif //Constraint_H
