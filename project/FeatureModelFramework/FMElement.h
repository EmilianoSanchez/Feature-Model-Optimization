#ifndef FMElement_H
#define FMElement_H

#include "Visitor.h"

//! \var Tag Enumeration for status of a link/feature
typedef enum {none, mandatory, optional,root} Tag;

//! \var Tag Enumeration for status of a hyper link
typedef enum {leaf,xor_group,or_group,and_group} HLTag;



/*! \class FMElement
 * \brief Abstract class for all elements that compose a Feature Model (features,
 * links, nodes...)
 */
class FMElement
{
public :
    // Visitor pattern, (e.g. to translate into FAMILIAR)
    virtual void accept(Visitor * v) = 0;
};

#endif
