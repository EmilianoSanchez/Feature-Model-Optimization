#include <iostream>

#include "Feature.h"


using namespace std;

// Build the tree: add a sub-feature (at construction time)
void Feature::add_sub_feature(Feature* f, Tag t)
{
    // cout << "add_sub_feature " << t << endl;
    if (_decomposition)
	_decomposition->add_feature(f,t);
    else
	cout << "ERREUR, NULL " << endl; // throw
}

HLTag Feature::get_hyperlink_type(){
        if(_decomposition==NULL)
            return leaf;
        else
            return _decomposition->get_type();
}

Tag Feature::get_link_type(){
        if(_link==NULL)
            return root;
        else
            return _link->get_type();
}
