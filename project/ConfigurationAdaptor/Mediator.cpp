#include "Mediator.h"
#include "../FeatureModelFramework/Feature.h"
#include "../FeatureModelFramework/Visitor.h"
#include "../FeatureModelFramework/HyperLink.h"
#include "../FeatureModelFramework/FeatureModel.h"
#include "../FeatureModelFramework/Constraint.h"
#include "../FeatureModelFramework/Configuration.h"
#include "SoftElement.h"
#include "Event.h"
#include "Communication.h"
#include "Persistence.h"

using namespace std;

Mediator::Mediator(): VSpec("VSpec"),VImpl("VImpl") {};
Mediator::~Mediator() {};

void Mediator::initialize() {
    //!VImpl and VSpec are created
    VS_model(); // read C++ format

    //!FAMILIAR process is initialized
    cerr<< "**** COMMUNICATION WITH FAMILIAR" <<endl;
    fml_comm = new FML_Comm();

    //!Attributes are loaded and ComponentManager is initialized
    cerr<< "**** LOAD FEATURE AND COMPONENT ATTRIBUTES" <<endl;
    load(cm,VImpl);
    cm.initialize();
    cerr<< "**** COMPONENTS" <<endl;
    cerr<<cm<<endl;

    //!VImpl and VSpec are loaded in FAMILIAR
    vector<string> commands
    {"run \"VS.fml\" "};
    cerr<< "**** TO FAMILIAR" <<endl;
    send_to_Familiar ((*fml_comm),commands);
/*
    cerr<< "**** FAMILIAR FORMAT" <<endl;
    test_VisitorFamiliar(VSpec);
    test_VisitorFamiliar(VImpl);

    cerr<< "**** BITSET FORMAT" <<endl;
    cerr << test_Visitorbitset(VSpec) <<endl;

    cerr<< "**** COMPONENTS" <<endl;
    cerr<<cm<<endl;
*/

};

Configuration Mediator::get_initial_specialization(){
    return get_new_specialization();
};

Configuration Mediator::get_new_specialization(){
    //!The thread waits until the following event is triggered
    Event event=cm.wait_for_event();

    //!If the event is of type 'end', an empty configuration is returned, meaning the termination of the system
    if(event.name=="End")
        return Configuration();

    //!For the rest of events, FAMILIAR commands are performed from the set of feature actions associated to the event.
    for(list<pair<string,FMAction> >::iterator iter=event.fm_actions.begin();iter!=event.fm_actions.end();iter++){
        string command;
        switch(iter->second){
            case FMAction::DESELECT:
                command+="deselect ";
                break;
            case FMAction::SELECT:
                command+="select ";
                break;
            case FMAction::UNSELECT:
                command+="unselect ";
                break;
        }
        command+= iter->first;
        command+= " in DEPLOYMENT";

        send1_to_Familiar ((*fml_comm),command);
    }

    //!The new partial configuration is parsed and returned by the method
    vector<string> results= send1_to_Familiar ((*fml_comm),"DEPLOYMENT");
    string config = results[0];
    cerr<< "\nNew configuration/sub-model "<< config <<endl;
    cerr<< "**** FROM FAMILIAR" <<endl;
    Configuration c(&VImpl,receive_from_Familiar(config));
    return c;

};

void Mediator::finalize() {
    //!FAMILIAR process is killed
    cerr << "**** KILLING FAMILIAR" << endl;
    fml_comm->killFML();
    delete fml_comm;

    //!Component Manager is finalized
    save(cm,VImpl,"./save.cm");
    cm.finalize();
};

Configuration Mediator::get_current_configuration(){
    return Configuration(&VImpl,VImpl.get_current_configuration());
}



void Mediator::set_initial_configuration(Configuration &initial_configuration){

    /**This section builds a list of component actions that the component manager performs to initialize the processing chain, according to the given initial_configuration.
     * At the beginning, no software elements are running. Therefore, for each concrete SELECTED feature, its associated software element (Component or Parameter) is initialized.
     */
    list<CMAction> cm_actions;
    for(int i=0;i<initial_configuration.nb_features();i++){
        if(initial_configuration[i]==FeatureState::SELECTED){
            SoftElement *se=initial_configuration.get_feature(i)->get_soft();
            if(se!=NULL){
                switch(se->get_type()){
                    case COMPONENT:
                        cm_actions.push_back(CMAction(se->get_name(),CMActionType::START_COMPONENT));
                        break;
                    case PARAMETER:
                        cm_actions.push_back(CMAction(se->get_name(),CMActionType::ACTIVATE_PARAMETER));
                        break;
                }
            }
        }
    }

    //!VImpl is updated to the new full configuration
    pair<vector<string>, vector<string> > features=initial_configuration.get_add_remove_features();
    VImpl.configure(features.first,features.second);
    //!Component manager is instructed to reconfigure the processing chain
    cm.reconfigure(cm_actions);
}


//build the list of actions that the component manager performs to reconfigure the processing chain, according to the given new_configuration and the current_configuration
void Mediator::set_new_configuration(Configuration &new_configuration){

    Configuration current=get_current_configuration();
    /**This section builds a list of component actions that the component manager performs to initialize the processing chain, according to the given new configuration and the current configuration.
     * For each concrete feature, its associated software element can be started or stopped according to its state in the current component configuration.
     */
    list<CMAction> cm_actions;
    for(int i=0;i<new_configuration.nb_features();i++){
        SoftElement *se=current.get_feature(i)->get_soft();
        if(current[i]!=new_configuration[i] && se!=NULL){
            if(current[i]==FeatureState::DESELECTED)
                switch(se->get_type()){
                    case COMPONENT:
                        cm_actions.push_back(CMAction(se->get_name(),CMActionType::START_COMPONENT));
                        break;
                    case PARAMETER:
                        cm_actions.push_back(CMAction(se->get_name(),CMActionType::ACTIVATE_PARAMETER));
                }
            else
                switch(se->get_type()){
                    case COMPONENT:
                        cm_actions.push_back(CMAction(se->get_name(),CMActionType::STOP_COMPONENT));
                        break;
                    case PARAMETER:
                        cm_actions.push_back(CMAction(se->get_name(),CMActionType::DESACTIVATE_PARAMETER));
                }
        }
    }

    //!VImpl is updated to the new full configuration
    pair<vector<string>, vector<string> > features=new_configuration.get_add_remove_features();
    VImpl.configure(features.first,features.second);
    //!Component manager is instructed to reconfigure the processing chain
    cm.reconfigure(cm_actions);
}


void Mediator::VS_model()
{
    Feature *VideoSurveillanceApplication = VSpec.new_feature("VideoSurveillanceApplication",0);
    list<pair<Feature*,Tag>> lf_VideoSurveillanceApplication;
    Feature *Task = VSpec.new_feature("Task",1);
    lf_VideoSurveillanceApplication.push_back (make_pair(Task,mandatory));
    Feature *ObjectOfInterest = VSpec.new_feature("ObjectOfInterest",2);
    lf_VideoSurveillanceApplication.push_back (make_pair(ObjectOfInterest,mandatory));
    Feature *Environment = VSpec.new_feature("Environment",3);
    lf_VideoSurveillanceApplication.push_back (make_pair(Environment,mandatory));
    Feature *QualityOfService = VSpec.new_feature("QualityOfService",4);
    lf_VideoSurveillanceApplication.push_back (make_pair(QualityOfService,mandatory));
    AND_HyperLink *rh_VideoSurveillanceApplication  = new AND_HyperLink(VideoSurveillanceApplication, lf_VideoSurveillanceApplication);
    VideoSurveillanceApplication->add_hyper_link(rh_VideoSurveillanceApplication);
    list<pair<Feature*,Tag>> lf_Task;
    Feature *IntrusionTask = VSpec.new_feature("IntrusionTask",5);
    lf_Task.push_back (make_pair(IntrusionTask,none));
    Feature *CountingTask = VSpec.new_feature("CountingTask",6);
    lf_Task.push_back (make_pair(CountingTask,none));
    Feature *TrackingTask = VSpec.new_feature("TrackingTask",7);
    lf_Task.push_back (make_pair(TrackingTask,none));
    OR_HyperLink *rh_Task  = new OR_HyperLink(Task, lf_Task);
    Task->add_hyper_link(rh_Task);
    list<pair<Feature*,Tag>> lf_QualityOfService;
    Feature *ComputerLoad = VSpec.new_feature("ComputerLoad",8);
    lf_QualityOfService.push_back (make_pair(ComputerLoad,none));
    Feature *ResponseTime = VSpec.new_feature("ResponseTime",9);
    lf_QualityOfService.push_back (make_pair(ResponseTime,none));
    Feature *Quality = VSpec.new_feature("Quality",10);
    lf_QualityOfService.push_back (make_pair(Quality,none));
    OR_HyperLink *rh_QualityOfService  = new OR_HyperLink(QualityOfService, lf_QualityOfService);
    QualityOfService->add_hyper_link(rh_QualityOfService);
    list<pair<Feature*,Tag>> lf_IntrusionTask;
    Feature *WithoutReco = VSpec.new_feature("WithoutReco",11);
    lf_IntrusionTask.push_back (make_pair(WithoutReco,none));
    Feature *WithReco = VSpec.new_feature("WithReco",12);
    lf_IntrusionTask.push_back (make_pair(WithReco,none));
    XOR_HyperLink *rh_IntrusionTask  = new XOR_HyperLink(IntrusionTask, lf_IntrusionTask);
    IntrusionTask->add_hyper_link(rh_IntrusionTask);
    list<pair<Feature*,Tag>> lf_ObjectOfInterest;
    Feature *Any = VSpec.new_feature("Any",13);
    lf_ObjectOfInterest.push_back (make_pair(Any,none));
    Feature *Natural = VSpec.new_feature("Natural",14);
    lf_ObjectOfInterest.push_back (make_pair(Natural,none));
    Feature *Manufactured = VSpec.new_feature("Manufactured",15);
    lf_ObjectOfInterest.push_back (make_pair(Manufactured,none));
    OR_HyperLink *rh_ObjectOfInterest  = new OR_HyperLink(ObjectOfInterest, lf_ObjectOfInterest);
    ObjectOfInterest->add_hyper_link(rh_ObjectOfInterest);
    list<pair<Feature*,Tag>> lf_Natural;
    Feature *Person = VSpec.new_feature("Person",16);
    lf_Natural.push_back (make_pair(Person,none));
    Feature *Animal = VSpec.new_feature("Animal",17);
    lf_Natural.push_back (make_pair(Animal,none));
    XOR_HyperLink *rh_Natural  = new XOR_HyperLink(Natural, lf_Natural);
    Natural->add_hyper_link(rh_Natural);
    list<pair<Feature*,Tag>> lf_Environment;
    Feature *LightLevel = VSpec.new_feature("LightLevel",18);
    lf_Environment.push_back (make_pair(LightLevel,mandatory));
    Feature *LightingConditions = VSpec.new_feature("LightingConditions",19);
    lf_Environment.push_back (make_pair(LightingConditions,mandatory));
    AND_HyperLink *rh_Environment  = new AND_HyperLink(Environment, lf_Environment);
    Environment->add_hyper_link(rh_Environment);
    list<pair<Feature*,Tag>> lf_Quality;
    Feature *Sensitivity = VSpec.new_feature("Sensitivity",20);
    lf_Quality.push_back (make_pair(Sensitivity,none));
    Feature *Precision = VSpec.new_feature("Precision",21);
    lf_Quality.push_back (make_pair(Precision,none));
    OR_HyperLink *rh_Quality  = new OR_HyperLink(Quality, lf_Quality);
    Quality->add_hyper_link(rh_Quality);
    list<pair<Feature*,Tag>> lf_Precision;
    Feature *HighPrecision = VSpec.new_feature("HighPrecision",22);
    lf_Precision.push_back (make_pair(HighPrecision,none));
    Feature *LowPrecision = VSpec.new_feature("LowPrecision",23);
    lf_Precision.push_back (make_pair(LowPrecision,none));
    XOR_HyperLink *rh_Precision  = new XOR_HyperLink(Precision, lf_Precision);
    Precision->add_hyper_link(rh_Precision);
    list<pair<Feature*,Tag>> lf_LightLevel;
    Feature *LowLighting = VSpec.new_feature("LowLighting",24);
    lf_LightLevel.push_back (make_pair(LowLighting,none));
    Feature *HighLighting = VSpec.new_feature("HighLighting",25);
    lf_LightLevel.push_back (make_pair(HighLighting,none));
    XOR_HyperLink *rh_LightLevel  = new XOR_HyperLink(LightLevel, lf_LightLevel);
    LightLevel->add_hyper_link(rh_LightLevel);
    list<pair<Feature*,Tag>> lf_LightingConditions;
    Feature *ArtificialLight = VSpec.new_feature("ArtificialLight",26);
    lf_LightingConditions.push_back (make_pair(ArtificialLight,none));
    Feature *NaturalLight = VSpec.new_feature("NaturalLight",27);
    lf_LightingConditions.push_back (make_pair(NaturalLight,none));
    OR_HyperLink *rh_LightingConditions  = new OR_HyperLink(LightingConditions, lf_LightingConditions);
    LightingConditions->add_hyper_link(rh_LightingConditions);
    VSpec.add_constraint(new Imply(ResponseTime,{TrackingTask}));
    VSpec.add_constraint(new Imply(CountingTask,{HighPrecision}));
    VSpec.add_constraint(new Exclude(CountingTask,{Any}));
    VSpec.add_constraint(new Imply(LowLighting,{WithoutReco}));
    VSpec.add_constraint(new Exclude(WithReco,{Any}));
    VSpec.fill(*VideoSurveillanceApplication,28);

    Feature *VideoSurveillanceSystem = VImpl.new_feature("VideoSurveillanceSystem",0);
    list<pair<Feature*,Tag>> lf_VideoSurveillanceSystem;
    Feature *Acquisition = VImpl.new_feature("Acquisition",1);
    lf_VideoSurveillanceSystem.push_back (make_pair(Acquisition,mandatory));
    Feature *Segmentation = VImpl.new_feature("Segmentation",2);
    lf_VideoSurveillanceSystem.push_back (make_pair(Segmentation,mandatory));
    Feature *Clustering = VImpl.new_feature("Clustering",3);
    lf_VideoSurveillanceSystem.push_back (make_pair(Clustering,mandatory));
    Feature *ShadowRemoval = VImpl.new_feature("ShadowRemoval",4);
    lf_VideoSurveillanceSystem.push_back (make_pair(ShadowRemoval,optional));
    Feature *Tracking = VImpl.new_feature("Tracking",5);
    lf_VideoSurveillanceSystem.push_back (make_pair(Tracking,mandatory));
    AND_HyperLink *rh_VideoSurveillanceSystem  = new AND_HyperLink(VideoSurveillanceSystem, lf_VideoSurveillanceSystem);
    VideoSurveillanceSystem->add_hyper_link(rh_VideoSurveillanceSystem);
    list<pair<Feature*,Tag>> lf_Acquisition;
    Feature *ResolutionLevel = VImpl.new_feature("ResolutionLevel",6);
    lf_Acquisition.push_back (make_pair(ResolutionLevel,mandatory));
    Feature *ColorType = VImpl.new_feature("ColorType",7);
    lf_Acquisition.push_back (make_pair(ColorType,mandatory));
    AND_HyperLink *rh_Acquisition  = new AND_HyperLink(Acquisition, lf_Acquisition);
    Acquisition->add_hyper_link(rh_Acquisition);
    list<pair<Feature*,Tag>> lf_Segmentation;
    Feature *TraversalAlgorithm = VImpl.new_feature("TraversalAlgorithm",8);
    lf_Segmentation.push_back (make_pair(TraversalAlgorithm,mandatory));
    Feature *KernelFunction = VImpl.new_feature("KernelFunction",9);
    lf_Segmentation.push_back (make_pair(KernelFunction,mandatory));
    AND_HyperLink *rh_Segmentation  = new AND_HyperLink(Segmentation, lf_Segmentation);
    Segmentation->add_hyper_link(rh_Segmentation);
    list<pair<Feature*,Tag>> lf_ResolutionLevel;
    Feature *HighResolution = VImpl.new_feature("HighResolution",10);
    lf_ResolutionLevel.push_back (make_pair(HighResolution,none));
    Feature *LowResolution = VImpl.new_feature("LowResolution",11);
    lf_ResolutionLevel.push_back (make_pair(LowResolution,none));
    XOR_HyperLink *rh_ResolutionLevel  = new XOR_HyperLink(ResolutionLevel, lf_ResolutionLevel);
    ResolutionLevel->add_hyper_link(rh_ResolutionLevel);
    list<pair<Feature*,Tag>> lf_ColorType;
    Feature *GreyLevel = VImpl.new_feature("GreyLevel",12);
    lf_ColorType.push_back (make_pair(GreyLevel,none));
    Feature *FullColor = VImpl.new_feature("FullColor",13);
    lf_ColorType.push_back (make_pair(FullColor,none));
    Feature *NearIR = VImpl.new_feature("NearIR",14);
    lf_ColorType.push_back (make_pair(NearIR,none));
    XOR_HyperLink *rh_ColorType  = new XOR_HyperLink(ColorType, lf_ColorType);
    ColorType->add_hyper_link(rh_ColorType);
    list<pair<Feature*,Tag>> lf_Tracking;
    Feature *TrackingColor = VImpl.new_feature("TrackingColor",15);
    lf_Tracking.push_back (make_pair(TrackingColor,none));
    Feature *Texture = VImpl.new_feature("Texture",16);
    lf_Tracking.push_back (make_pair(Texture,none));
    XOR_HyperLink *rh_Tracking  = new XOR_HyperLink(Tracking, lf_Tracking);
    Tracking->add_hyper_link(rh_Tracking);
    list<pair<Feature*,Tag>> lf_TraversalAlgorithm;
    Feature *TraversalParameters = VImpl.new_feature("TraversalParameters",17);
    lf_TraversalAlgorithm.push_back (make_pair(TraversalParameters,mandatory));
    Feature *GridStep = VImpl.new_feature("GridStep",18);
    lf_TraversalAlgorithm.push_back (make_pair(GridStep,mandatory));
    AND_HyperLink *rh_TraversalAlgorithm  = new AND_HyperLink(TraversalAlgorithm, lf_TraversalAlgorithm);
    TraversalAlgorithm->add_hyper_link(rh_TraversalAlgorithm);
    list<pair<Feature*,Tag>> lf_TraversalParameters;
    Feature *WithWindow = VImpl.new_feature("WithWindow",19);
    lf_TraversalParameters.push_back (make_pair(WithWindow,none));
    Feature *WithMask = VImpl.new_feature("WithMask",20);
    lf_TraversalParameters.push_back (make_pair(WithMask,none));
    Feature *Fast = VImpl.new_feature("Fast",21);
    lf_TraversalParameters.push_back (make_pair(Fast,none));
    OR_HyperLink *rh_TraversalParameters  = new OR_HyperLink(TraversalParameters, lf_TraversalParameters);
    TraversalParameters->add_hyper_link(rh_TraversalParameters);
    list<pair<Feature*,Tag>> lf_GridStep;
    Feature *HighGridStep = VImpl.new_feature("HighGridStep",22);
    lf_GridStep.push_back (make_pair(HighGridStep,none));
    Feature *LowGridStep = VImpl.new_feature("LowGridStep",23);
    lf_GridStep.push_back (make_pair(LowGridStep,none));
    XOR_HyperLink *rh_GridStep  = new XOR_HyperLink(GridStep, lf_GridStep);
    GridStep->add_hyper_link(rh_GridStep);
    VImpl.add_constraint(new Imply(TrackingColor,{FullColor}));
    VImpl.add_constraint(new Imply(Texture,{HighResolution}));
    VImpl.fill(*VideoSurveillanceSystem,24);

};
