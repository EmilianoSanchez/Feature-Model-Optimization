#ifndef CONFIGURATION_H_INCLUDED
#define CONFIGURATION_H_INCLUDED

/*! \file Configuration.h
 * \brief Definition of Configuration class and FeatureState enum, to model partial and full configurations,
 */

#include "FeatureModel.h"
#include <vector>
#include <iostream>


//! State of a feature in a configuration.
typedef enum FeatureState{
    DESELECTED, /**<The feature is excluded from the configuration (DESELECTED)*/
    SELECTED, /**<The feature is part of the configuration (SELECTED)*/
    UNSELECTED /**<The feature is not fixed (UNSELECTED or UNASSIGNED)*/
};

/** Definition of an Assignment object
 *  It is represented with a vector of pairs, which identified index of features with a concrete FeatureState (SELECTED or DESELECTED)
 */
typedef std::vector<std::pair<unsigned int,FeatureState> > Assignment;

//! class that represents partial or full configurations of feature models
/*!
 * It is modeled as an array of FeatureState, that identified each feature by its index.
*/
class Configuration{
private:

    FeatureModel *fm;//!< reference to its feature model
    std::vector<FeatureState> features;//!<<vector of FeatureState

public:

    //! default contructor
    Configuration():features(0),fm(NULL){};
    //! constructor that initialize all features of "_fm" to UNSELECTED
    Configuration(FeatureModel *_fm,FeatureState feature_state=UNSELECTED):features(_fm->nb_features(),feature_state),fm(_fm){};

    //!The following two constructors were defined to ensure the compatibility with the FeatureModel class
    //! constructor that initialize all features of "_fm" according to the parameter "states".
    /*!
        \param states pair with the vectors of features to be SELECTED and DESELECTED respectively
    */
    Configuration(FeatureModel *_fm,std::pair<std::vector<std::string>, std::vector<std::string> > states):features(_fm->nb_features(),UNSELECTED),fm(_fm){
        for(std::vector<std::string>::iterator it=states.first.begin();it!=states.first.end();it++ )
            if(fm->get_feature_index(*it)<features.size())
                features[fm->get_feature_index(*it)]=SELECTED;
        for(std::vector<std::string>::iterator it=states.second.begin();it!=states.second.end();it++ )
            if(fm->get_feature_index(*it)<features.size())
                features[fm->get_feature_index(*it)]=DESELECTED;
    };
    //! constructor that initialize all features of "_fm" according to the parameter "states".
    /*!
        \param states map of features to be SELECTED and DESELECTED
    */
    Configuration(FeatureModel *_fm,std::map<std::string,std::pair<bool,int> > &states):features(_fm->nb_features(),UNSELECTED),fm(_fm){
        for(std::map<std::string,std::pair<bool,int> >::iterator it=states.begin();it!=states.end();it++ )
            if((it->second).second<features.size()){
                if((it->second).first)
                    features[(it->second).second]=SELECTED;
                else
                    features[(it->second).second]=DESELECTED;
            }
    };

    /** method that generate a pair of vector with SELECTED and DESELECTED features.
        It ensures compatibility with FeatureModel class
    */
    std::pair<std::vector<std::string>, std::vector<std::string> > get_add_remove_features(){
        std::pair<std::vector<std::string>, std::vector<std::string> > fs;
        for(int i=0;i<fm->nb_features();i++)
        switch(features[i]){
            case SELECTED:
                fs.first.push_back(fm->get_feature_name(i));
                break;
            case DESELECTED:
                fs.second.push_back(fm->get_feature_name(i));
        }
        return fs;
    }
    /*!
        \return true if the configuration is full, i.e without UNSELECTED features
    */
    bool is_full_configuration(){
        for(int i=0;i<nb_features();i++)
            if(features[i]==UNSELECTED)
                return false;
        return true;
    }

    int nb_features(){return fm->nb_features();};

    //! Method to edit feature states directly.
    /*! Use apply_assignment instaid of it, to ensure the validity of configurations
    */
    FeatureState &operator[](unsigned int feature_index){
        return features[feature_index];
    };

    Feature *get_feature(unsigned int feature_index){
        return fm->get_feature(feature_index);
    };

    FeatureModel *get_fm(){
        return fm;
    };

    //! equal operator
    bool operator==(Configuration &other){
        if(fm==other.fm){
            for(int i=0;i<nb_features();i++)
                if(features[i]!=other.features[i])
                    return false;
            return true;
        }else
            return false;
    };

    //! insertion operator to print the configuration
    friend std::ostream &operator<<(std::ostream &stream, Configuration &c);

    //! method to apply a vector of assignments
    /**
        \return true if it reachs a valid configuration, i.e no constraints are infringed
    */
    bool apply_assignment(Assignment &assignment);

    //! method to apply a selection assignments
    /**
        \param index of the feature to be assigned
        \return true if it reachs a valid configuration, i.e no constraints are infringed
    */
    bool apply_selection(unsigned int feature);
    //! method to apply a deselection assignments
    /**
        \param index of the feature to be assigned
        \return true if it reachs a valid configuration, i.e no constraints are infringed
    */
    bool apply_deselection(unsigned int feature);

};



#endif // CONFIGURATION_H_INCLUDED
