#include "fm_generator.h"
#include "../FeatureModelFramework/HyperLink.h"
#include "../FeatureModelFramework/Visitor.h"

#include <boost/lexical_cast.hpp>
using namespace std;

FeatureModel *generate_fm(string name,int nb_features,int nb_hyperlinks,HyperLink_Type hlt,int nb_constraints,int nb_attributes){

    //generate feature model as a balanced tree with random links and hyperlinks:
    FeatureModel *fm=new FeatureModel(name);

    Feature * features[nb_features];


    int children=(nb_features-1)/nb_hyperlinks;//children means branching factor
    int children_root=children+(nb_features-1)%nb_hyperlinks;
    int index_children=1;

    features[0]=fm->new_feature("A_"+boost::lexical_cast<string>(0),0);
    generete_hyperlink(fm,features,0,index_children,children_root,hlt);
    index_children+=children_root;

    for(int h=1;h<nb_hyperlinks;h++){
        generete_hyperlink(fm,features,h,index_children,children,hlt);
        index_children+=children;
    }

    fm->fill(*(features[0]),nb_features);

    //generate random values for feature attributes
    if(nb_attributes>0)
        generate_attributes(fm,nb_attributes);

    //generate random cross-tree constraints
    if(nb_constraints>0)
        if(!generate_ctc(fm,nb_constraints))
            return NULL;


    return fm;
}

void generete_hyperlink(FeatureModel *fm,Feature **fs,unsigned int index_parent,unsigned int index_children,int cant_children,HyperLink_Type hlt){

    if(hlt==RANDOM){
        hlt= (HyperLink_Type)(rand()%3+1);
    }

    list<pair<Feature*,Tag>> lf;
    if(hlt==AND || hlt==AND_OPT){
        for(int i=0;i<cant_children;i++){
            fs[i+index_children]=fm->new_feature("A_"+boost::lexical_cast<string>(i+index_children),i+index_children);
            if(hlt==AND_OPT || rand()%2==1)
                lf.push_back(make_pair(fs[i+index_children],optional));
            else
                lf.push_back(make_pair(fs[i+index_children],mandatory));
        }
    }else{
        for(int i=0;i<cant_children;i++){
            fs[i+index_children]=fm->new_feature("A_"+boost::lexical_cast<string>(i+index_children),i+index_children);
            lf.push_back(make_pair(fs[i+index_children],none));
        }
    }

    HyperLink *rh;
    switch(hlt){
        case AND :
            rh = new AND_HyperLink(fs[index_parent], lf);
            break;
        case AND_OPT:
            rh = new AND_HyperLink(fs[index_parent], lf);
            break;
        case OR:
            rh = new OR_HyperLink(fs[index_parent], lf);
            break;
        case XOR:
            rh = new XOR_HyperLink(fs[index_parent], lf);
            break;
    }

    fs[index_parent]->add_hyper_link(rh);
}


void generate_attributes(FeatureModel *fm,int nb_attributes){
    list<Feature *> leafs;
    for(int i=0;i<fm->nb_features();i++)
        if(fm->get_feature(i)->get_hyperlink_type()==HLTag::leaf)
            leafs.push_back(fm->get_feature(i));

    for(list<Feature *>::iterator iter=leafs.begin();iter!=leafs.end();iter++){
        string soft_name("C_");
        soft_name+=(*iter)->name();
        SoftElement *soft=new Component(soft_name);
        (*iter)->add_soft(soft);

        for(int a=0;a<nb_attributes;a++){
            double rvalue=((double)(rand()%100) +1)/100.0;
            soft->set_attribute(a,rvalue);
            (*iter)->set_attribute(a,rvalue);
        }
    }
}

vector<FeatureModel *> generate_fms(std::string name,int nb_features,int nb_hyperlinks,HyperLink_Type hlt,int nb_constraints/*,double CTCR=-1.0*/,int nb_attributes){

    //generate feature models:

    vector<FeatureModel *> fms(3);
    fms[0]=new FeatureModel(name);
    fms[1]=new FeatureModel(name+"_B");
    fms[2]=new FeatureModel(name+"_A");

    vector<Feature **> featuress(3);
    featuress[0]=new Feature*[nb_features];
    featuress[1]=new Feature*[nb_features];
    featuress[2]=new Feature*[nb_features];

    int children=(nb_features-1)/nb_hyperlinks;
    int children_root=children+(nb_features-1)%nb_hyperlinks;
    int index_children=1;

    featuress[0][0]=fms[0]->new_feature("A_"+boost::lexical_cast<string>(0),0);
    featuress[1][0]=fms[1]->new_feature("A_"+boost::lexical_cast<string>(0),0);
    featuress[2][0]=fms[2]->new_feature("A_"+boost::lexical_cast<string>(0),0);

    generete_hyperlink(fms,featuress,0,index_children,children_root,hlt);
    index_children+=children_root;

    for(int h=1;h<nb_hyperlinks;h++){
        generete_hyperlink(fms,featuress,h,index_children,children,hlt);
        index_children+=children;
    }

    fms[0]->fill(*(featuress[0][0]),nb_features);
    fms[1]->fill(*(featuress[1][0]),nb_features);
    fms[2]->fill(*(featuress[2][0]),nb_features);

    if(nb_attributes>0)
        generate_attributes(fms,nb_attributes);

    if(nb_constraints>0)
        generate_ctc(fms[0],nb_constraints);

    return fms;
}

void generete_hyperlink(vector<FeatureModel *> &fms,vector<Feature **> &fss,unsigned int index_parent,unsigned int index_children,int cant_children,HyperLink_Type hlt){

    if(hlt==RANDOM)
        hlt= (HyperLink_Type)(rand()%3+1);

    list<pair<Feature*,Tag>> lf[3];
    if(hlt==AND || hlt==AND_OPT){
        for(int i=0;i<cant_children;i++){
            fss[0][i+index_children]=fms[0]->new_feature("A_"+boost::lexical_cast<string>(i+index_children),i+index_children);
            fss[1][i+index_children]=fms[1]->new_feature("A_"+boost::lexical_cast<string>(i+index_children),i+index_children);
            fss[2][i+index_children]=fms[2]->new_feature("A_"+boost::lexical_cast<string>(i+index_children),i+index_children);
            if(hlt==AND_OPT || rand()%2==1){
                lf[0].push_back(make_pair(fss[0][i+index_children],optional));
                lf[1].push_back(make_pair(fss[1][i+index_children],optional));
                lf[2].push_back(make_pair(fss[2][i+index_children],optional));
            }else{
                lf[0].push_back(make_pair(fss[0][i+index_children],mandatory));
                lf[1].push_back(make_pair(fss[1][i+index_children],mandatory));
                lf[2].push_back(make_pair(fss[2][i+index_children],optional));
            }
        }
    }else{
        for(int i=0;i<cant_children;i++){
            fss[0][i+index_children]=fms[0]->new_feature("A_"+boost::lexical_cast<string>(i+index_children),i+index_children);
            fss[1][i+index_children]=fms[1]->new_feature("A_"+boost::lexical_cast<string>(i+index_children),i+index_children);
            fss[2][i+index_children]=fms[2]->new_feature("A_"+boost::lexical_cast<string>(i+index_children),i+index_children);
            lf[0].push_back(make_pair(fss[0][i+index_children],none));
            lf[1].push_back(make_pair(fss[1][i+index_children],none));
            lf[2].push_back(make_pair(fss[2][i+index_children],optional));
        }
    }

    HyperLink *rh[3];
    switch(hlt){
        case AND :
            rh[0] = new AND_HyperLink(fss[0][index_parent], lf[0]);
            rh[1] = new AND_HyperLink(fss[1][index_parent], lf[1]);
            rh[2] = new AND_HyperLink(fss[2][index_parent], lf[2]);
            break;
        case AND_OPT:
            rh[0] = new AND_HyperLink(fss[0][index_parent], lf[0]);
            rh[1] = new AND_HyperLink(fss[1][index_parent], lf[1]);
            rh[2] = new AND_HyperLink(fss[2][index_parent], lf[2]);
            break;
        case OR:
            rh[0] = new OR_HyperLink(fss[0][index_parent], lf[0]);
            rh[1] = new OR_HyperLink(fss[1][index_parent], lf[1]);
            rh[2] = new AND_HyperLink(fss[2][index_parent], lf[2]);
            break;
        case XOR:
            rh[0] = new XOR_HyperLink(fss[0][index_parent], lf[0]);
            rh[1] = new XOR_HyperLink(fss[1][index_parent], lf[1]);
            rh[2] = new AND_HyperLink(fss[2][index_parent], lf[2]);
            break;
    }

    fss[0][index_parent]->add_hyper_link(rh[0]);
    fss[1][index_parent]->add_hyper_link(rh[1]);
    fss[2][index_parent]->add_hyper_link(rh[2]);

}

void generate_attributes(vector<FeatureModel *> &fms,int nb_attributes){

    for(int i=0;i<fms[0]->nb_features();i++){
        if(fms[0]->get_feature(i)->get_hyperlink_type()==HLTag::leaf){
            string soft_name("C_");
            soft_name+= fms[0]->get_feature(i)->name();
            SoftElement *soft=new Component(soft_name);

            fms[0]->get_feature(i)->add_soft(soft);
            fms[1]->get_feature(i)->add_soft(soft);
            fms[2]->get_feature(i)->add_soft(soft);

            for(int a=0;a<nb_attributes;a++){
                double rvalue=((double)(rand()%100) +1)/100.0;
                soft->set_attribute(a,rvalue);
                fms[0]->get_feature(i)->set_attribute(a,rvalue);
                fms[1]->get_feature(i)->set_attribute(a,rvalue);
                fms[2]->get_feature(i)->set_attribute(a,rvalue);
            }
        }

    }
}

bool generate_ctc(FeatureModel *fm,int nb_constraints){
//incorporation of cross-tree constraints v1: considering all features except the root
    /*set<Clause> clauses;
    Constraint *constraint;

    for(int i=0;i<nb_constraints;i++){

        do{
            int left = (rand()%(fm->nb_features()-1))+1;
            int right;
            do{
                right = (rand()%(fm->nb_features()-1))+1;
            }while(right==left);

            //if(rand()%2==1)
                constraint=new Imply(fm->get_feature(left),{fm->get_feature(right)});
            //else
            //    constraint=new Exclude(fm->get_feature(left),{fm->get_feature(right)});

        }while(clauses.find(constraint->getCNF().front())!=clauses.end());

        clauses.insert(constraint->getCNF().front());

        fm->add_constraint(constraint);
        //cout<<"added"<<endl;
    }*/

//incorporation of cross-tree constraints v2: considering only leaf features
    set<Clause> clauses;
    Constraint *constraint;

    int first_leaf=0;
    for(int i=0;i<fm->nb_features();i++){
        if(fm->get_feature(i)->get_hyperlink_type()==HLTag::leaf){
            first_leaf=i;
            break;
        }
    }

    for(int i=0;i<nb_constraints;i++){

        do{
            int left = (rand()%(fm->nb_features()-first_leaf))+first_leaf;
            int right;
            do{
                right = (rand()%(fm->nb_features()-first_leaf))+first_leaf;
            }while(right==left);

            //if(rand()%2==1)
                constraint=new Imply(fm->get_feature(left),{fm->get_feature(right)});
            //else
            //    constraint=new Exclude(fm->get_feature(left),{fm->get_feature(right)});

        }while(clauses.find(constraint->getCNF().front())!=clauses.end());

        clauses.insert(constraint->getCNF().front());

        fm->add_constraint(constraint);
        //cout<<"added"<<endl;
    }


//validation of the feature model
    /*Solver<BestFS_Iterator> solver;
    Configuration conf(fm);
    if(conf.apply_selection(0) && solver.search_first_solution(&conf))
        return true;
    else
        return false;*/
    return true;
}


void export_to_familiar(string file,FeatureModel *fm){
    VisitorFMLFile visitor(file);
    visitor.visit(fm);
}
