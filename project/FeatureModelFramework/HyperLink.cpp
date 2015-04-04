#include <iostream>

#include "HyperLink.h"
#include "Feature.h"

using namespace std;

// Construction of the FM
void HyperLink::add_feature(Feature* f, Tag t)
{
    Link *li;
    switch (t){
        case none :
            li = new Link(f);
            break;
        case mandatory :
            li = new MandLink(f);
            break;
        case optional :
            li = new OptLink(f);
            break;
    }
    f->add_link(li);
    li->connect(this);
    _links.push_back(li);
}
