/*!
 * \file FML_Comm.cpp
 *
 * \brief Communication between C++ code and Familiar running in daemon mode (\c -I)
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>

#include <cassert>
#include <cstdlib>

#include "FML_Comm.h"

using namespace std;

//! Name of the pipe to send to Familiar
const char *toFML_name = "toFML";

//! Name of the pipe to receive from Familiar
const char *fromFML_name = "fromFML";

//! Opening mode for the pipes
const mode_t MODE = S_IRUSR | S_IWUSR;

//! Command to start Familiar
const char *jarFML = "./FML-0.9.9.X.jar";

//! Familiar prompt
const string PROMPT = "fml> ";

static bool create_pipe(const char *name)
{
    int rc = mkfifo(name, MODE);
    if (rc == 0)
        return true;
    if (errno == EEXIST)
    {
        struct stat st;
        if (stat(name, &st) < 0)
            assert(false);
        if (not S_ISFIFO(st.st_mode))
        {
            cerr << "*** " << name << " exists and is not a pipe!" << endl;
            return false;
        }
        else
            return true;
    }
    return false;
}

/*  Constructor
 *  - create the named pipes
 *  - create the IOstreams
 *  - start Familiar
 */
FML_Comm::FML_Comm()
{
    //cerr << "FML_Comm::FML_Comm" << endl;
    if (not create_pipe(toFML_name) or not create_pipe(fromFML_name))
        throw Cannot_Create_Pipe();

    switch (_pidFML = fork())
    {
    case -1:
        throw Cannot_Fork();
    case 0:
        close(0);
        close(1);
        open(toFML_name, O_RDONLY);
        open(fromFML_name, O_WRONLY);
        //cout<< "antes"<<endl;
        execlp("java","-Xmx1024M", "-jar",jarFML);
        //cout<< "despues"<<endl;
        //execlp("java", "FML", "-jar", jarFML, "-I", NULL);
        cerr << "Cannot execute Familiar" << endl;
        exit(1);
    default:
         break;
    }

    _toFML.open(toFML_name);
    _fromFML.open(fromFML_name);
    if (not _toFML.is_open() or not _fromFML.is_open())
        throw Cannot_Open_Pipe();

    sleep(3);
    //cerr << "Wait 1st prompt" << endl;

    //cerr << "<<<< recv first: " << first_prompt << endl;
    cerr << "Communication with FAMILIAR established" << endl;

    string first_prompt;
    getline(_fromFML, first_prompt);
    getline(_fromFML, first_prompt);
    getline(_fromFML, first_prompt);
    //getline(_fromFML, first_prompt);

    //assert(first_prompt == PROMPT);
}


vector<string> FML_Comm::exec1(const string& cmd)
{
    string linein;
    vector<string> answer;

    _toFML << cmd << endl;
    cerr << ">>>> send: " << cmd << endl;

    getline(_fromFML, linein);
    cerr << "<<<< recv: " << linein << endl;
    answer.push_back(linein);

    /*while (getline(_fromFML, linein))
    {
        cerr << "<<<< recv: " << linein << endl;
        if (linein == PROMPT)
            break;
        else
            answer.push_back(linein);
    }*/
    if (not _fromFML)
    {
	cerr << "Familiar died unexpectedly" << endl;
	throw FML_Died();
    }

    return answer;
}


vector<string> FML_Comm::exec(const vector<string>& cmds)
{
    string linein;
    vector<string> answer;
    //cerr << "FML_Comm::exec" << endl;

    for (size_t i = 0; i < cmds.size(); ++i)
    {
        _toFML <<  cmds[i]  << endl;
        //cerr << ">>>> send: " << cmds[i] << endl;

        //se corta en el getline, como si llegara al final del stream. Nunca retorna el PROMP (fml>) :s
        while (getline(_fromFML, linein))
        {
            answer.push_back(linein);
            if (linein.substr(0,10)=="DEPLOYMENT" || linein.substr(0,10)=="fml> DEPLO" )//if (linein == PROMPT)
                break;
        }
    }

    if (not _fromFML)
    {
	cerr << "Familiar died unexpectedly" << endl;
	throw FML_Died();
    }

    return answer;
}





/*
int main(int argc, char *argv[])
{
    FML_Comm fml_comm;

    cerr << "Start" << endl;
    for (int i = 1; i < argc; ++i)
    {
        ifstream in(argv[i]);
        string cmd;
        vector<string> cmds;
        vector<string> answer;

        while (getline(in, cmd))
            cmds.push_back(cmd);
        answer = fml_comm.exec(cmds);

        cout << "RESULTS" << endl;
        for (size_t i = 0; i < answer.size(); ++i)
            cout << answer[i] << endl;
        sleep(3);
    }

    cerr << "Killing Familiar" << endl;
    fml_comm.killFML();
    return 0;
}

*/


