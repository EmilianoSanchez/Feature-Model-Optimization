Each makefile build a different project:

-"makefile_configuration_adaptor" build the "Configuration Adaptor" project. It includes the file "Main_ConfigurationAdaptor.cpp" and the files in the folders "FeatureModelFramework", "ConfigurationSelectionAlgorithm" and "ConfigurationAdaptor"
This project represents the Configuration Adapter module, part of the Self-Adaptive Platform.
The binary ("./bin_conf_adapter/Release/Configuration Adapter") requires the files "FML-0.9.9.X.jar" (FAMILIAR program), "VS.fml" (feature model definition for FAMILIAR) and "test_case.cm" (information of software components and parameters, events, etc) in the same folder. FAMILIAR program is not included here because of its size (~60 MB)

-"makefile_evaluation" build the "Evaluation" project, that includes the file "Main_Evaluation.cpp" and the files in the folders "FeatureModelFramework", "ConfigurationSelectionAlgorithm" and "Evaluation".
This project represents the evaluation of the Configuration Selection Algorithm. It executes the set of experiments.
The binary ("./bin_evaluation/Release/Evaluation") only requires that the command "openoffice.org" can be executed by console to load the results (that are saved in file "./bin_evaluation/Release/results.csv").

