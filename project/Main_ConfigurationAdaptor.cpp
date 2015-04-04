#include <iostream>
using namespace std;

#include "ConfigurationAdaptor/Mediator.h"
#include "FeatureModelFramework/Configuration.h"
#include "ConfigurationAdaptor/Solver.h"

/** this method represents the main loop of the Configuration Adaptor, the core of the Runtime Adaptation System.
 *  A 'mediator' is an object that provides a simplified interface and allows to use the rest of the modules of the system in a unified way.
 */
int main(){
    ///Initialization of subsystems (FAMILIAR, Component Manager and Configuration selector algorithm):
    Mediator mediator;

    mediator.initialize();

    ///This object represents the configuration selection algorithm that selects the initial configuration (load time)
    ///and the new configurations when an event change the current variability of the system (run time)
    Solver solver(mediator.get_VImpl());

    ///Initial partial configuration of the 'Implementation Feature Model' given by FAMILIAR
    Configuration new_conf = mediator.get_initial_specialization();

    cout<<"initial specialization (partial configuration processed by FAMILIAR): \n"<<new_conf<<endl;

    /** In theory the following condition is always true because, according to the reachability property verified at design time, all valid specifications (partial configurations)
     *  have at least one valid configuration
     */
    if(solver.select_initial_configuration(new_conf)){

        cout<<"initial selected configuration: \n"<<new_conf<<endl;

        mediator.set_initial_configuration(new_conf);

        /**Main loop. When the system close (because of failure or normal termination), 'get_new_specialization' method must return an empty configuration.
         * This method sleeps the current thread of the Configurator Adapter until a new event is triggered in the ComponentManager, implying
         * a context change and therefore a possible change in the partial configuration that represents the current variability of the system.
         * the object 'new_conf' references this new partial configuration.
         */
        for(new_conf = mediator.get_new_specialization();new_conf.get_fm()!=NULL;new_conf = mediator.get_new_specialization()){

            cout<<"new specialization (partial configuration processed by FAMILIAR): \n"<<new_conf<<endl;
            Configuration current=mediator.get_current_configuration();
            cout<<"current configuration: \n"<<current<<endl;

            if(!solver.select_new_configuration(new_conf,current)){
                cout<<"the new specialization is invalid."<<endl;
                break;
            }

            cout<<"new selected configuration: \n"<<new_conf<<endl;

            mediator.set_new_configuration(new_conf);

        }

    }else
        cout<<"the initial specialization is invalid."<<endl;// In theory this condition would never be reach, because of familiar and the initial validations of the models.

    ///Finalization of subsystems: FAMILIAR, Component Manager and the Configuration Adaptor itself.
    mediator.finalize();

    return 0;

}
