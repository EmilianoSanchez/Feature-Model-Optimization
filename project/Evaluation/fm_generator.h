#ifndef FM_GENERATOR_H_INCLUDED
#define FM_GENERATOR_H_INCLUDED

#include "../FeatureModelFramework/FeatureModel.h"


enum HyperLink_Type {
    RANDOM,//for generating any kind of feature models
    AND,//for generating feature models with only AND groups
    OR,//for generating feature models with only OR groups
    XOR,//for generating feature models with only XOR groups
    AND_OPT//for generating feature models with only optional features
};

/** Function that generates a random feature model.
 * preconditions: nb_features>=2; nb_hyperlinks>=1; nb_hyperlinks<nb_features; nb_constraints>=0; nb_attributes>=0
 * the average branching factor is equal to (nb_features-1) / nb_hyperlinks
 */
FeatureModel *generate_fm(std::string name,int nb_features=2,int nb_hyperlinks=1,HyperLink_Type hlt=RANDOM,int nb_constraints=0,int nb_attributes=0);

/** Function that generates a vector with three feature models: a random generated feature model, its relaxed model B and its relaxed model A
 */
std::vector<FeatureModel *> generate_fms(std::string name,int nb_features=2,int nb_hyperlinks=1,HyperLink_Type hlt=RANDOM,int nb_constraints=0,int nb_attributes=0);


//!Auxiliar functions invoked by the previous ones

void generete_hyperlink(FeatureModel *fm,Feature **fs,unsigned int index_parent,unsigned int index_children,int cant_children,HyperLink_Type hlt);
void generete_hyperlink(std::vector<FeatureModel *> &fms,std::vector<Feature **> &fss,unsigned int index_parent,unsigned int index_children,int cant_children,HyperLink_Type hlt);

void generate_attributes(FeatureModel *fm,int nb_attributes);
void generate_attributes(std::vector<FeatureModel *> &fms,int nb_attributes);

bool generate_ctc(FeatureModel *fm,int nb_constraints);

void export_to_familiar(std::string file,FeatureModel *fm);



#endif // FM_GENERATOR_H_INCLUDED
