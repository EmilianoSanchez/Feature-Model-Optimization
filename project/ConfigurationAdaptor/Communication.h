#ifndef _Communication_H_
#define _Communication_H_

/*! \file Communication.h
 * \brief API to communicate with FAMILIAR, in both directions
 * Based on FML_Comm class
 */

#include <string>
#include <vector>

#include "FML_Comm.h"
#include "Event.h"

std::vector<std::string> send_to_Familiar(FML_Comm&,const std::vector<std::string>& );
std::vector<std::string> send1_to_Familiar(FML_Comm&,const std::string& );

// Function defined in spiritparser.cpp, because it/home/lusanche/Dropbox/INRIA/Project/ConfigurationAdapter/FML_Mediator requires parsing
extern std::pair<std::vector<std::string>, std::vector<std::string> > receive_from_Familiar(std::string input);
//void receive_Event(FML_Comm&, const std::string&);

#endif
