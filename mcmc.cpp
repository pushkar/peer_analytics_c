#include <map>
#include <random>
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>

#include "variables.h"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

// Variables
std::vector<Normal> s;
std::vector<Normal> q;
std::vector<Observation> o;

class Gibbs {
public:
    Gibbs() {

    }

    double likelihood_z(int s_id, int q_id) {
        double s_id_sample = 0;
        double q_id_sample = 0;

        double sum_new = 0;
        for (int i=0; i<o.size(); i++) {
            double s_ = s[o[i].get_sid()].last();
            double q_ = q[o[i].get_qid()].last();
            if (o[i].get_sid() == s_id) {
                s_ = s[s_id].sample();
                s_id_sample = s_;
            }
            if (o[i].get_qid() == q_id) {
                q_ = q[q_id].sample();
                q_id_sample = q_;
            }
            sum_new -= (log(1 + exp(-(s_-q_))) + (1-o[i].get_response())*(s_-q_));
            sum_new -= 0.5*s_*s_;
            sum_new -= 0.5*q_*q_;
        }

        double sum_old = 0;
        for (int i=0; i<o.size(); i++) {
            double s_ = s[o[i].get_sid()].last();
            double q_ = q[o[i].get_qid()].last();
            sum_old -= (log(1 + exp(-(s_-q_))) + (1-o[i].get_response())*(s_-q_));
            sum_old -= 0.5*s_*s_;
            sum_old -= 0.5*q_*q_;
        }

        double r = exp(sum_new - sum_old);
        if (random_(0, 1) < r) {
            for (int i=0; i<s.size(); i++) {
                if (i==s_id)
                    s[i].insert(s_id_sample);
                else
                    s[i].insert(s[i].last());
            }
            for (int i=0; i<q.size(); i++) {
                if (i==q_id)
                    q[i].insert(q_id_sample);
                else
                    q[i].insert(q[i].last());
            }
        }
        else {
            for (int i=0; i<s.size(); i++) {
                s[i].insert(s[i].last());
            }
            for (int i=0; i<q.size(); i++) {
                q[i].insert(q[i].last());
            }

        }
    }

    void run(const int n_steps, const int burn_steps) {
        for (int i=0; i<s.size(); i++) {
            s[i].initialize();
        }
        for (int i=0; i<q.size(); i++) {
            q[i].initialize();
        }

        // This is for predicting IRT stuff,
        // Proficiency of students and hardness of questions.
        for (int n=1; n<n_steps; ++n) {
            for (int _i=0; _i<s.size(); _i++) {
                likelihood_z(_i, -1);
                for (int _j=0; _j<q.size(); _j++) {
                    likelihood_z(-1, _j);
                    }
                }
            // show the step_n :: difference
            std::cout << std::right;
            if (n % (n_steps/200) == 0) {
                int done = (n*100.0/n_steps);
                std::cout << "[" << std::setw(3) << done << "%]: " << "\r" << std::flush;
            }

        }

        for (int i=0; i<s.size(); i++) {
            s[i].burn(burn_steps);
            s[i].stats();
        }
        for (int i=0; i<q.size(); i++) {
            q[i].burn(burn_steps);
            q[i].stats();
        }
        std::cout << std::flush;// << std::endl;
    }
};


int main()
{
    const int n_steps = 20000;
    const int n_burn = 4000;

    std::cout << "Students, Questions, Errors, Confidence " << std::endl;

    for (int n_students=10; n_students<11; n_students++) {
        for (int n_questions=5; n_questions<20; n_questions++) {
            s.clear();
            q.clear();
            o.clear();

            for (int i=0; i<n_students; i++) {
                Normal norm;
                norm.set_params(random_(0, 5), 0.1);
                s.push_back(norm);
            }
            for (int i=0; i<n_questions; i++) {
                Normal norm;
                norm.set_params(random_(0, 5), 0.1);
                q.push_back(norm);
            }

            for (int i=0; i<n_students; i++) {
                for (int j=0; j<n_questions; j++) {
                    Observation obs;
                    if (s[i].last() > q[j].last())
                        obs.set(i, j, 1);
                    else
                        obs.set(i, j, 0);
                    o.push_back(obs);
                }
            }

            Gibbs h;
            h.run(n_steps, n_burn);

            int error_sum = 0;
            for (int i=0; i<o.size(); i++) {
                double proficiency = s[o[i].get_sid()].mean();
                double hardness = q[o[i].get_qid()].mean();
                if (proficiency > hardness && o[i].get_response() == 0)
                    error_sum++;
                if (proficiency < hardness && o[i].get_response() == 1)
                    error_sum++;
            }

            double var = 0;
            for (int i=0; i<s.size(); i++) {
                var += (1.0/s[i].mean_variance());
            }
            for (int i=0; i<q.size(); i++) {
                var += (1.0/q[i].mean_variance());
            }
            var = sqrt(1.0/var);

            std::cout << n_students << ", ";
            std::cout << n_questions << ", ";
            std::cout << error_sum << ", ";
            std::cout << var << std::endl;

        }  // n_questions
    } // n_students

}
