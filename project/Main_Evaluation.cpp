#include "./Evaluation/Experiments.h"

/** execution of experiments
 */
int main(){
    initialize_linear_weighted_functions();

    experiment_2_A();
    experiment_2_B();
    experiment_2_C();

    experiment_3_B();
    experiment_3_C();

    experiment_1_A();
    return 0;
}
