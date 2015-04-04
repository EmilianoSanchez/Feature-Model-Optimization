#include "Constraint.h"

using namespace std;

/** A Imply constraint has the form A-> (B and C and ...)
 *  So, in CNF it has the form  (not A or B) and (not A or C) and (not A or ...) and ...
 *  All clauses have negated A, the left part of the constraint, and all right features are affirmative.
 */
list<Clause> Imply::getCNF(){
        list<Clause> clauses;
        Clause aux;
        aux[_left->get_index()]=false;
        for(list<Feature*>::iterator it=_right.begin();it!=_right.end();it++){
            aux[(*it)->get_index()]=true;
            clauses.push_back(aux);
            aux.erase(aux.find((*it)->get_index()));
        }
        return clauses;
};

/** A Exclude constraint has the form A-> (not B and not C and ...)
 *  So, in CNF it has the form  (not A or not B) and (not A or not C) and (not A or not ...) and ...
 *  All clauses have negated both: A and the right feature
 */
list<Clause> Exclude::getCNF(){
        list<Clause> clauses;
        Clause aux;
        aux[_left->get_index()]=false;
        for(list<Feature*>::iterator it=_right.begin();it!=_right.end();it++){
            aux[(*it)->get_index()]=false;
            clauses.push_back(aux);
            aux.erase(aux.find((*it)->get_index()));
        }
        return clauses;
};

/** A MutualExclusion constraint has the form A <-> !B, equivalent to A xor B
 *  So, in CNF it has the form  (not A or not B) and (A or B)
 */
list<Clause> MutualExclusion::getCNF(){
        Feature *r=_right.front();
        list<Clause> clauses;
        Clause aux;
        aux[_left->get_index()]=false;
        aux[r->get_index()]=false;
        clauses.push_back(aux);
        aux[_left->get_index()]=true;
        aux[r->get_index()]=true;
        clauses.push_back(aux);
        return clauses;
};

/** A MutualImplication constraint has the form A <-> B
 *  So, in CNF it has the form  (not A or B) and (A or not B)
 */
list<Clause> MutualImplication::getCNF(){
        Feature *r=_right.front();
        list<Clause> clauses;
        Clause aux;
        aux[_left->get_index()]=true;
        aux[r->get_index()]=false;
        clauses.push_back(aux);
        aux[_left->get_index()]=false;
        aux[r->get_index()]=true;
        clauses.push_back(aux);
        return clauses;
};
