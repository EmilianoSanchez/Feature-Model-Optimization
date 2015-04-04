#ifndef PERSISTENCEMANAGER_H_INCLUDED
#define PERSISTENCEMANAGER_H_INCLUDED

#define DEFAULT_COMPONENT_MANAGER_INPUT "./test_case.cm"
#define DEFAULT_COMPONENT_MANAGER_OUTPUT "./save.cm"

#include <string>
#include "ComponentManagerServiceInterface.h"
#include "../FeatureModelFramework/FeatureModel.h"
#include "Solver.h"

/**This method is responsible of incorporating software elements, events and attributes to the component manager and VImpl feature model,
 * from the data stored in the given file.
 */
bool load(ComponentManager &cm,FeatureModel &fm,std::string file_name=DEFAULT_COMPONENT_MANAGER_INPUT);

/** Opposite to load method, this generates a file with the attributes of software elements, events and features.
 */
bool save(ComponentManager &cm,FeatureModel &fm,std::string file_name=DEFAULT_COMPONENT_MANAGER_OUTPUT);


#endif // PERSISTENCEMANAGER_H_INCLUDED
