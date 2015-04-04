#ifndef MEDIATOR_INCLUDED
#define MEDIATOR_INCLUDED

/*! \file FML_Mediator.h
 */

#include <utility>
#include <string>
#include <vector>

#include "../FeatureModelFramework/FeatureModel.h"
#include "FML_Comm.h"
#include "../FeatureModelFramework/Configuration.h"
#include "ComponentManagerServiceInterface.h"

/**Mediator is the core of the Configuration Adapter module.
 * It provides a simplified interface used by the main thread (main loop) to
 * initialize, execute and communicate with the rest of modules of the system.
 * Through a 'mediator' object, the main thread manages the whole video surveillance system
 * (or any other component-based system) as a feature model with its correspondent full configuration.
 * By this way, the main loop only interacts with the configuration selection algorithms and the 'mediator' object.
 */

class Mediator
{
public:
    Mediator();
    ~Mediator();

    /**This method:
     *  -builds VSpec, VImpl and loads their attributes,
     *  -initializes the ComponentManager, its software elements and attributes.
     *  -initializes and connects to FAMILIAR
     *   (here, the thread sleeps until FAMILIAR and the component manager start)
     */
    void initialize();

    /**This method returns the initial partial configuration that represents the variability of the component-based system.
     */
    Configuration get_initial_specialization();

    /**this method sets the initial full configuration of VImpl and generates a set of component actions that instructs the component manager to start the initial component configuration*/
    void set_initial_configuration(Configuration &initial_configuration);

    /**This method waits for the next event triggered by the component manager.
     * When an event occurs, it is interpreted as a set of feature actions for FAMILIAR (selection, deselection and unselection of features).
     * Then it parses and returns the new partial configuration reasoned by FAMILIAR.
     * When a event of type 'end' occurs (the system fails or closes normally), an empty configuration is returned.
     */
    Configuration get_new_specialization();

    /**This method return the current configuration of VImpl*/
    Configuration get_current_configuration();

    /**this method sets the new full configuration of VImpl and generates a set of component actions that instructs the component manager to adapt the component configuration*/
    void set_new_configuration(Configuration &new_configuration);

    /**this method close FAMILIAR and Component Manager*/
    void finalize();

    FeatureModel *get_VImpl(){return &VImpl;};
private:
    FeatureModel VSpec;
    FeatureModel VImpl;

    ComponentManager cm;

    FML_Comm *fml_comm;
    void VS_model();
};

#endif // MEDIATOR_INCLUDED
