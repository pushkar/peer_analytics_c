#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>
#include <array>
#include <algorithm>

#include "variables.h"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

int main()
{

    // Variables
    const int n_steps = 20000;
    const int n_students = 10;
    const int n_questions = 100;
    const int n_answered_questions = 20;

    std::array<Student, n_students> s;
    std::array<Question, n_questions> q;
    std::array<Observation, n_students*n_questions> o;

    // Initialization
    for (int i = 0; i < n_students; i++) {
        s[i].set_normal_distribution(0.5, 0.1);
    }
    for (int i = 0; i < n_questions; i++) {
        q[i].set_normal_distribution(0.5, 0.1);
    }
    for (int i = 0; i < n_students; i++) {
        for (int j = 0; j < n_answered_questions; j++) {
            int k = std::round(random_(0, n_questions));
            o[i*n_questions+k].set(1);
        }
    }

    // MCMC
    float diff = 0, old_diff = 10000;

    for (int n = 0; n < n_steps; n++) {
        diff = 0;
        for (int i = 0; i < n_students; i++) {
            for (int j = 0; j < n_questions; j++) {
                // For every student and question
                // find a new value
                s[i].step();
                q[j].step();
                // Calculate the observation for each
                int k = i*n_questions+j;
                o[k].set(s[i].get_proficiency(), q[j].get_hardness());
                diff += o[k].diff();
            }
        }

        if (diff > old_diff) { // reject new valuess
            for (int i = 0; i < n_students; i++) s[i].reject();
            for (int j = 0; j < n_questions; j++) q[j].reject();
            diff = old_diff;
        }
        else
            old_diff = diff;

        // show the step_n :: difference
        std::cout << std::right;
        if (n % (n_steps/200) == 0) {
            int done = (n*100.0/n_steps);
            std::cout << "[" << std::setw(3) << done << "%]: " << diff << "\r" << std::flush;
        }
    }

    std::cout << std::flush << std::endl;
    std::cout << "Done." << std::endl;
}
