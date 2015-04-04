//#include <unistd.h>

#include <iostream>
#include <fstream>
//#include <string>
//#include <vector>
using namespace std;

#include "Communication.h"

// Send a series of commands to FAMILIAR
vector<string> send_to_Familiar(FML_Comm& fml_comm, const vector<string>& cmds)
//const char *file)
{
    vector<string> answer;
    answer = fml_comm.exec(cmds);

    //cerr << "send_to_Familiar RESULTS" << endl;
    //for (size_t i = 0; i < answer.size(); ++i)
//	cerr << answer[i] << endl;

    return answer;
}

vector<string> send1_to_Familiar(FML_Comm& fml_comm, const string& cmd)
{
    vector<string> answer;
    answer = fml_comm.exec1(cmd);
    for(vector<string>::iterator it=answer.begin();it!=answer.end();it++){
        (*it).erase(0,5);
    }
    //cerr << "send1_to_Familiar RESULTS" << endl;
    //for (size_t i = 0; i < answer.size(); ++i)
//	cerr << answer[i] << endl;

    return answer;
}

/*
void receive_Event(FML_Comm& fml_comm, const string& evt)
{
    string new_action = react(evt);
    if (new_action !="")
    {
	cerr << "On "<< evt << ": " << new_action << endl;
	send1_to_Familiar(fml_comm,new_action);
    }
}
*/
