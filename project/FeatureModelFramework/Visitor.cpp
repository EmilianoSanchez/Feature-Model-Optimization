#include "Visitor.h"
#include "HyperLink.h"
#include "Constraint.h"
#include "FeatureModel.h"

/*! \file  Visitor.cpp
 * \brief Implement "visiting" of feature models for different purposes
 */

using namespace std;

// 1st purpose: Translation into FAMILIAR
void VisitorFamiliar::visit (Feature* fe)
    {
	cout << fe->name();
	//cout << " " << fe->get_index();
	HyperLink* dec= fe->decomposition();
	if (dec)
	{
	    cout << ": ";
	    dec->accept(this);
	}
	cout << ";" << endl;
	if (dec)
	{
	    for (list<Link*>::iterator itl = dec->links().begin();
		 itl!=dec->links().end(); itl++)
		if ((*itl)->end()->decomposition())
		    (*itl)->end()->accept(this);
	}
    }

void VisitorFamiliar::visit (Link* l)
    {
	cout << " " << l->end()->name();
	//cout << " " << l->end()->get_index();
    }

void VisitorFamiliar::visit (OptLink* ol)
    {
	cout << " [" << ol->end()->name() << "]";
	//cout << " " << ol->end()->get_index();
    }

void VisitorFamiliar::visit(AND_HyperLink* hl)
    {
	list<Link *>::iterator it;
	for (it=hl->links().begin() ; it != hl->links().end(); ++it)
	    (*it)->accept(this);
    }
void VisitorFamiliar::visit(OR_HyperLink* hl)
    {
	list<Link*> llk =hl->links();
	int slk = llk.size()-1, i;
	list<Link *>::iterator it;
	cout << "(" ;
	for (it=llk.begin(), i =0 ; it != llk.end(); ++it, ++i)
	{
	    (*it)->accept(this);
	    if (i<slk) cout << "|" ;
	}
	cout << ")+";
    }
void VisitorFamiliar::visit(XOR_HyperLink* hl)
    {
	list<Link*> llk =hl->links();
	int slk = llk.size()-1, i;
	list<Link *>::iterator it;
	cout << "(" ;
	for (it=llk.begin(), i =0 ; it != llk.end(); ++it, ++i)
	{
	    (*it)->accept(this);
	    if (i<slk) cout << "|" ;
	}
	cout << ")";
    }

void VisitorFamiliar::visit(Imply* cti)
{
    list<Feature*> lfr = cti->right() ;
    cout << cti->left()->name() << " -> " ;

    auto itr = lfr.begin();
    if (lfr.size() > 1)
	cout << '(' ;
    cout <<(*itr)->name();
    itr++;
    for (; itr != lfr.end();++itr)
	cout << '&' << (*itr)->name();
    if (lfr.size() > 1)
	cout << ')' ;
    cout <<";" <<endl;
}

void VisitorFamiliar::visit(Exclude* cte)
{
    list<Feature*> lfr = cte->right() ;
    cout << cte->left()->name()<< " -> !";

    auto itr = lfr.begin();
    if (lfr.size() > 1)
	cout << '(' ;
    cout <<(*itr)->name();
    itr++;
    for (; itr != lfr.end();++itr)
	cout << '&' << (*itr)->name();
    if (lfr.size() > 1)
	cout << ')' ;
    cout <<";" <<endl;
}

void VisitorFamiliar::visit(MutualImplication* ctmi)
{
    list<Feature*> lfr = ctmi->right() ;
    cout << ctmi->left()->name() << " <-> " ;

    auto itr = lfr.begin();
    if (lfr.size() > 1)
	cout << '(' ;
    cout <<(*itr)->name();
    itr++;
    for (; itr != lfr.end();++itr)
	cout << '&' << (*itr)->name();
    if (lfr.size() > 1)
	cout << ')' ;
    cout <<";" <<endl;
}
void VisitorFamiliar::visit(MutualExclusion* ctme)
{
    list<Feature*> lfr = ctme->right() ;
    cout << ctme->left()->name() << " -> !" ;

    auto itr = lfr.begin();
    if (lfr.size() > 1)
	cout << '(' ;
    cout <<(*itr)->name();
    itr++;
    for (; itr != lfr.end();++itr)
	cout << '&' << (*itr)->name();
    if (lfr.size() > 1)
	cout << ')' ;
    cout <<";" <<endl;
}

void VisitorFamiliar::visit(FeatureModel *fm)
{
    cout << fm->name() << " = FM (";
    visit(&fm->root());
    for(std::list<Constraint *>::iterator itc=fm->get_constraints().begin();itc!=fm->get_constraints().end();itc++)
        (*itc)->accept(this);
    std::cout <<")\n";
}

// 3rd purpose: Translation into an .fml file

VisitorFMLFile::VisitorFMLFile(string file){
    fichero.open(file, ios::out);
}

VisitorFMLFile::~VisitorFMLFile(){
    fichero.close();
}

void VisitorFMLFile::visit (Feature* fe)
    {
	fichero << fe->name();
	//cout << " " << fe->get_index();
	HyperLink* dec= fe->decomposition();
	if (dec)
	{
	    fichero << ": ";
	    dec->accept(this);
	}
	fichero << ";" << endl;
	if (dec)
	{
	    for (list<Link*>::iterator itl = dec->links().begin();
		 itl!=dec->links().end(); itl++)
		if ((*itl)->end()->decomposition())
		    (*itl)->end()->accept(this);
	}
    }

void VisitorFMLFile::visit (Link* l)
    {
	fichero << " " << l->end()->name();
	//cout << " " << l->end()->get_index();
    }

void VisitorFMLFile::visit (OptLink* ol)
    {
	fichero << " [" << ol->end()->name() << "]";
	//cout << " " << ol->end()->get_index();
    }

void VisitorFMLFile::visit(AND_HyperLink* hl)
    {
	list<Link *>::iterator it;
	for (it=hl->links().begin() ; it != hl->links().end(); ++it)
	    (*it)->accept(this);
    }
void VisitorFMLFile::visit(OR_HyperLink* hl)
    {
	list<Link*> llk =hl->links();
	int slk = llk.size()-1, i;

	list<Link *>::iterator it;
	fichero << "(" ;
	for (it=llk.begin(), i =0 ; it != llk.end(); ++it, ++i)
	{
	    (*it)->accept(this);
	    if (i<slk) fichero << "|" ;
	}
	fichero << ")+";
    }
void VisitorFMLFile::visit(XOR_HyperLink* hl)
    {
	list<Link*> llk =hl->links();
	int slk = llk.size()-1, i;

	list<Link *>::iterator it;
	fichero << "(" ;
	for (it=llk.begin(), i =0 ; it != llk.end(); ++it, ++i)
	{
	    (*it)->accept(this);
	    if (i<slk) fichero << "|" ;
	}
	fichero << ")";
    }

void VisitorFMLFile::visit(Imply* cti)
{
    list<Feature*> lfr = cti->right() ;
    fichero << cti->left()->name() << " -> " ;

    auto itr = lfr.begin();
    if (lfr.size() > 1)
	fichero << '(' ;
    fichero <<(*itr)->name();
    itr++;
    for (; itr != lfr.end();++itr)
	fichero << '&' << (*itr)->name();
    if (lfr.size() > 1)
	fichero << ')' ;
    fichero <<";" <<endl;
}

void VisitorFMLFile::visit(Exclude* cte)
{
    list<Feature*> lfr = cte->right() ;
    fichero << cte->left()->name()<< " -> !";

    auto itr = lfr.begin();
    if (lfr.size() > 1)
	fichero << '(' ;
    fichero <<(*itr)->name();
    itr++;
    for (; itr != lfr.end();++itr)
	fichero << '&' << (*itr)->name();
    if (lfr.size() > 1)
	fichero << ')' ;
    fichero <<";" <<endl;
}

void VisitorFMLFile::visit(MutualImplication* ctmi)
{
    list<Feature*> lfr = ctmi->right() ;
    fichero << ctmi->left()->name() << " <-> " ;

    auto itr = lfr.begin();
    if (lfr.size() > 1)
	fichero << '(' ;
    fichero <<(*itr)->name();
    itr++;
    for (; itr != lfr.end();++itr)
	fichero << '&' << (*itr)->name();
    if (lfr.size() > 1)
	fichero << ')' ;
    fichero <<";" <<endl;
}
void VisitorFMLFile::visit(MutualExclusion* ctme)
{
    list<Feature*> lfr = ctme->right() ;
    fichero << ctme->left()->name() << " <-> !" ;

    auto itr = lfr.begin();
    if (lfr.size() > 1)
	fichero << '(' ;
    fichero <<(*itr)->name();
    itr++;
    for (; itr != lfr.end();++itr)
	fichero << '&' << (*itr)->name();
    if (lfr.size() > 1)
	fichero << ')' ;
    fichero <<";" <<endl;
}

void VisitorFMLFile::visit(FeatureModel *fm)
{
    fichero << fm->name() << " = FM (";
    visit(&fm->root());
    for(std::list<Constraint *>::iterator itc=fm->get_constraints().begin();itc!=fm->get_constraints().end();itc++)
        (*itc)->accept(this);
    fichero <<")\n";
}

///////////////

// 2nd purpose: Translation into a bitset
/*
void VisitorBitset::visit (Feature* fe)
{
	if (! fe->link()) // otherwise done by links
	{
	    cout << fe->name()<< " 1 "<<fe->get_index()<< endl;
	    bbyte.set(fe->get_index());
	}
	HyperLink* dec= fe->decomposition();
	if (dec)
	    dec->accept(this);
    }

void VisitorBitset::visit (Link* l)
    {
	cout << l->end()->name()<<" " <<l->back()->bitvalue()<<" "
	     <<l->end()->get_index()<< endl;
	bbyte.set(l->end()->get_index(),l->back()->bitvalue());
//strbyte.push_back(l->back()->bitvalue());
    }

void VisitorBitset::visit (OptLink* ol)
    {
	cout << ol->end()->name()<<" 0 "<<ol->end()->get_index()<<endl;
	//strbyte.push_back('0');//cout << "0" ;
	bbyte.set(ol->end()->get_index(),false);
    }

void VisitorBitset::visit(AND_HyperLink* hl)
    {
	list<Link *>::iterator it;
	for (it=hl->links().begin() ; it != hl->links().end(); ++it)
	{
	    (*it)->accept(this);
	    if ((*it)->end()->decomposition())
		(*it)->end()->accept(this);
	}
    }
void VisitorBitset::visit(OR_HyperLink* hl)
    {
	cout <<" OR "<<hl->start()->name()<<" "<< hl->start()->get_index()<<endl;
	list<Link *>::iterator it;
	for (it=hl->links().begin() ; it !=  hl->links().end(); ++it)
	{
	    (*it)->accept(this);
	    if ((*it)->end()->decomposition())
		(*it)->end()->accept(this);
	}
    }

void VisitorBitset::visit(XOR_HyperLink* hl)
    {
	cout <<" XOR "<< hl->start()->name()<<" "<< hl->start()->get_index()<<endl;
	list<Link*> llk =hl->links();
	list<Link *>::iterator it;
	for (it=llk.begin(); it != llk.end(); ++it)
	{
	    (*it)->accept(this);
	    //cout << " " <<(*it)->end()->name() <<(*it)->end()->get_index()<<endl;
	    if ((*it)->end()->decomposition())
		(*it)->end()->accept(this);
	}
    }

void VisitorBitset::visit(Imply *cti)
{
    cout << " Imply " <<endl;
}
void VisitorBitset::visit(Exclude *cti)
{
    cout << " Exclude " <<endl;
}
void VisitorBitset::visit(MutualImplication *cti)
{
    cout << " MutualImplication " <<endl;
}
void VisitorBitset::visit(MutualExclusion *cti)
{
    cout << " MutualExclusion " <<endl;
}

void VisitorBitset::visit(FeatureModel *fm)
{
    visit(&fm->root());
    //for (auto it = features.begin(); it != features.end(); it++)
//	cout << it->first << " = " << it->second->name()<<endl;
    //fm->set_map(features);
}
*/


/*
////////////////////
// temporary test functions
boost::dynamic_bitset<> test_Visitorbitset(FeatureModel& fm)
{

    VisitorBitset vb(fm.nb_features());
    fm.accept(&vb);
    return vb.bbyte;
}

void test_VisitorFamiliar(FeatureModel& fm)
{
    VisitorFamiliar vf;
    cout << "Familiar format: \n" ;
    fm.accept(&vf);

}
*/
