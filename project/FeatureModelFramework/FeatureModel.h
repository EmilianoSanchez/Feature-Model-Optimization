#ifndef FeatureModel_H
#define FeatureModel_H

/*! \file FeatureModel.h
 * \brief Definition of feature models coming from/ going to FAMILIAR, with a
 * name, a root feature and some utilities to perform searches, to compare FMs...
 */

#include <vector>
#include <boost/dynamic_bitset.hpp>

#include "Feature.h"
#include "Constraint.h"

class FeatureModel: public FMElement
{
public :

    class No_Index {}; // Exception

    // Accessors
    std::string name() {return _name;}

    Feature& root() {return _root;}

    int nb_features () {return _nb_features;}

    // Constructors
    FeatureModel (const std::string n, const Feature& r) : _name(n), _root(r)
    {
	//_root.set_model(*this);
    }

    FeatureModel (const std::string n) : _name(n)
    {}

    // To finish construction... set root and number of features in FM
    void fill(Feature& r, int nb)
    {
	_root=r;
	_nb_features = nb;
    }

    //! Translate into feature manager (FAMILIAR or other) format
    void accept (Visitor * v)
    {
	v->visit(this);
    }

    // Get the name of a feature from its index
    std::string get_feature_name(int i)
   {
       if (_features[i])
	   return _features[i]->name();
       else throw No_Index();
    }

    int get_feature_index(std::string name)
   {
       for(int index=0;index<_features.size();index++)
            if(_features[index]->name()==name)
                return index;
       return -1;
    }

    // Get the feature from its index
    Feature* get_feature(int i)
   {
        return _features[i];
    }


    // Initialise (empty) configuration, create new feature
    Feature* new_feature(const std::string&,const int);


    // Set new configuration-> include(exclude) features from current configuration
    void configure (const std::vector<std::string>& toadd,
		    const std::vector<std::string>& torem);
/*
    //Compare two configurations through their bitsets
    int compare(const boost::dynamic_bitset<>&,const boost::dynamic_bitset<>&);

    // Modify current configuration vs new one
    //void reconfigure (const boost::dynamic_bitset<>&);
    void reconfigure (const std::vector<std::string>&,
		      const std::vector<std::string>&);
    */

    std::map<std::string,std::pair<bool,int>>  &get_current_configuration(){return _configuration;};

    /** This method adds a new cross-tree constraint to the feature model as a set of clauses in Clausular Normal Form
     */
    void add_constraint(Constraint *c);

    /** This method returns a set of cross-tree constraint clauses that includes the given feature as variable
     */
    std::list<Clause> &get_constraints_CNF(unsigned int feature_index){return _constraints_CNF[feature_index];};
    /** This method returns all cross-tree constraint. It is used by the Visitor class
     */
    std::list<Constraint *> &get_constraints(){return _constraints;};

//for testing the representation of constraints in CNF
/*    void show_constraints(){
        for(int i=0;i<_nb_features;i++){
            std::cout<<"feature "<<i;

            std::list<Clause> clauses= _constraints_CNF[i];
            for(std::list<Clause>::iterator it=clauses.begin();it!=clauses.end();it++){
                std::cout<<'(';
                for(Clause::iterator it2=it->begin();it2!=it->end();it2++){
                    if(it2->second)
                        std::cout<<it2->first<<'|';
                    else
                        std::cout<<'~'<<it2->first<<'|';
                }
                std::cout<<") & ";
            }
            std::cout<<std::endl;
        }
    }*/

private :

    std::string _name; //!< name of the FM (== a name space for feature names)
    Feature _root; //!< root feature
    int _nb_features; //!< total number of features in the model
    std::vector<Feature*> _features; //!< vector of features(indexed)
    std::map<std::string,std::pair<bool,int>> _configuration; //!< current configuration: Map<FeatureName,pair<FeatureState,FeatureIndex>>
    //std::list<Constraints&> _contraints; //!<< Internal constraints

    std::vector<std::list<Clause> > _constraints_CNF; //!< vector of clauses(indexed by feature)
    std::list<Constraint *> _constraints;//!< list of constraints

};

#endif //FeatureModel_H
