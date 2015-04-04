#include <algorithm>
#include <cassert>

#include "FeatureModel.h"

using namespace std;

// Set initial (empty) configuration: create a new Feature, insert it in maps
Feature* FeatureModel::new_feature(const string& name,const int index)
{
    Feature* f=  new Feature(name,index);
    _features.push_back(f);
    _configuration[name] = make_pair(false,index);
//.first= false;  _configuration[n].second= i;

    _constraints_CNF.push_back(list<Clause>());
    return f;
}

void FeatureModel::add_constraint(Constraint *c){
    list<Clause> clauses=c->getCNF();

    for(list<Clause>::iterator it=clauses.begin();it!=clauses.end();it++){
        for(Clause::iterator it2=it->begin();it2!=it->end();it2++){
            _constraints_CNF[it2->first].push_back(*it);
        }
    }

    _constraints.push_back(c);
}

// Set new configuration at run time: a set of features to be included/excluded
// are given by FAMILIAR (read by Spirit internal parser)
// BUT they are given on the aggregate FM (both Specif and Implementation)
// so we need to sort out the features belonging to THIS FM
void FeatureModel::configure (const vector<string>& toadd,
			      const vector<string>& torem)
{
    map<string,pair<bool,int>>::const_iterator out = _configuration.end();

    //cerr << "Selected: " <<endl;
    for (auto it = toadd.begin();it!=toadd.end();++it)
    { //cerr << *it << ' ';
        if (_configuration.find(*it) != out) // feature found in this FM
        {
            _configuration[*it].first=true;// feature marked as selected
            //_features[(_configuration[*it].second)]->add();
        }
    }
    //cerr << endl;
    //cerr << "Deselected: " <<endl;
    for (auto it = torem.begin();it!=torem.end();++it)
    { //cerr << *it << ' ';
        if (_configuration.find(*it) != out)
        {
            _configuration[*it].first=false;// element marked as deselected
            //_features[(_configuration[*it].second)]->remove();
        }
    }
    //cerr << endl;
}



/*
// Modify current configuration: apply changes from current config to new one
void FeatureModel::reconfigure (const vector<string>& toadd,
			      const vector<string>& torem)
{
    map<string,pair<bool,int>>::const_iterator out = _configuration.end();

    for (auto it = toadd.begin();it!=toadd.end();++it)
        if (_configuration.find(*it) != out) // feature found in this FM
            if (_configuration[*it].first!=true){ // feature not already selected
                _configuration[*it].first=true;
                //_features[(_configuration[*it].second)]->add();
            }

    for (auto it = torem.begin();it!=torem.end();++it)
        if (_configuration.find(*it) != out)
            if (_configuration[*it].first!=false){// feature not already deselected
                _configuration[*it].first=false;
                //_features[(_configuration[*it].second)]->remove();
            }
}
*/

/*
// Compare two bitsets/strings to detect changes (+ or -)
int FeatureModel::compare (const boost::dynamic_bitset<>& current,
			   const boost::dynamic_bitset<>& newc)
//boost::dynamic_bitset<>& newc)
{
    pair<list<int>,list<int>> modifs = modifications(current,newc);
    list<int> add = modifs.first;
    list<int> rem = modifs.second;

    int sum=0;

    cerr << "To add " ;
    for (auto it = add.begin();it!=add.end();++it)
    {
	Feature *f = _features[*it];
	cerr << f->name() << " ";
	sum += f->weight();
    }
    cerr << endl;
    cerr << "To remove " ;
    for (auto it = rem.begin();it!=rem.end();++it)
    {
	Feature *f = _features[*it];
	cerr << f->name() << " ";
	sum += f->weight();
    }
    cerr << endl;
    return sum;
}*/



// Same using bitsets
/*
void FeatureModel::reconfigure (const boost::dynamic_bitset<>& new_config)
{
    const boost::dynamic_bitset<>& conf = to_bitset(_configuration,_nb_features);
    //cerr << "|" << conf<< "|"<< endl;
    //cerr << "|" <<  new_config<< "|"<< endl;
    assert (conf.size() == new_config.size());

    // Compute differences between current and new config
    pair<list<int>,list<int>> modifs = modifications(conf, new_config);
    list<int> add = modifs.first;
    list<int> rem = modifs.second;

    list<string> toadd (add.size());
    transform(add.begin(), add.end(), toadd.begin(),
	      bind1st(mem_fun(&FeatureModel::get_feature_name), this));

    // ... and apply changes (newly included/excluded features)
    cerr << "Adding " <<endl;
    for (auto it = toadd.begin();it!=toadd.end();++it)
    { cerr << *it << endl;
	_configuration[*it].first=true;
    }

    list<string> torem (rem.size());
    transform(rem.begin(), rem.end(), torem.begin(),
	      bind1st(mem_fun(&FeatureModel::get_feature_name), this));

    cerr << "Removing " ;
    for (auto it = torem.begin();it!=torem.end();++it)
    {
	cerr << *it << endl;
	_configuration[*it].first=false;
    }

    //cerr << to_bitset(_configuration,_nb_features)<<endl;
}
*/
