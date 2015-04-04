#ifndef _FML_COMM_H_
#define _FML_COMM_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <signal.h>
/*!
 * \file FML_Comm.h
 *
 * \brief Communication between C++ code and Familiar running in daemon mode (\c -I)
 * \note Work for Unix/Posix only.
 */

/*! \class FML_Comm
 * \brief The class to communicate with Familiar.
 *
 * The communication is performed through two Posix "named pipes" (\c toFML and 
 * \c fromFML) which are created if needed. 
 */
class FML_Comm
{
private:

    std::ofstream _toFML;	//!< Stream to send commands to Familiar
    std::ifstream _fromFML;	//!< Stream to receive results from Familiar
    pid_t _pidFML;		//!> Process id of Familiar

public:

    class All {};
    class Cannot_Create_Pipe : public All {};
    class Cannot_Open_Pipe : public All {};
    class FML_Died : public All {};
    class Cannot_Fork : public All {};

    //! Familiar constructor
    FML_Comm(); 

    //! Execute a command in Familiar and yield the result
    std::vector<std::string> exec(const std::vector<std::string>&);
    std::vector<std::string> exec1(const std::string&);

    //! Terminate Familiar--- and close pipes?
    void killFML() 
    {
        kill(_pidFML, SIGKILL);
	//_toFML.close();
	//_fromFML.close();
    }
};

#endif
