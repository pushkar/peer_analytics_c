#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>

#include "data.h"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

double true_mean = 0.5;
double true_sigma = 0.1;

std::vector<double> data_x;
std::vector<double> data_y;
int data_size = 150;

double normal_custom(double x, double mean, double sigma) {
    return exp(-(x-mean)*(x-mean)/(2*sigma*sigma))/(sigma*sqrt(2*M_PI));
}

double log_normal_custom(double x, double mean, double sigma) {
    //return log(1/(2*sigma*sigma)) + (-(x-mean)*(x-mean));
    return log(normal_custom(x, mean, sigma));
}

double likelihood(double mean) {
    std::vector<double> pred;
    for (int i = 0; i < data_size; i++) {
        pred.push_back(normal_custom(data_x.at(i), mean, 0.1));
    }
    double sum = 0;
    for (int i = 0; i < data_size; i++) {
        sum += log_normal_custom(data_y.at(i), pred.at(i), 0.1);
    }
    return sum;
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

class MH {
public:
    MH() {

    }

    void run(const int n, const int burn_n) {
        for (int i=0; i<s.size(); i++) {
            s[i].initialize();
        }
        for (int i=0; i<q.size(); i++) {
            q[i].initialize();
        }

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
            s[i].burn(burn_n);
            s[i].stats();
        }
        for (int i=0; i<q.size(); i++) {
            q[i].burn(burn_n);
            q[i].stats();
        }
        std::cout << std::flush << std::endl;
        std::cout << "Done." << std::endl;
    }
};


int main()
{
    for (int i = 0; i < data_size; i++) {
        data_x.push_back(random_(0, 1));
        data_y.push_back(normal_custom(data_x.at(i), true_mean, true_sigma));
    }

    for (int i=0; i<n_students; i++) {
        s[i].set_params(0.1*i, 0.1);
        std::cout << "Proficiency of Student " << i << ": " << s[i].last() << std::endl;
    }
    for (int i=0; i<n_questions; i++) {
        q[i].set_params(0.1*i, 0.1);
        std::cout << "Hardness of Question " << i << ": " << q[i].last() << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Data" << std::endl;

    for (int i=0; i<n_students; i++) {
        for (int j=0; j<n_questions; j++) {
            if (s[i].last() > q[j].last())
                o[i*n_students+j].set(i, j, 1);
            else
                o[i*n_students+j].set(i, j, 0);
            std::cout << o[i*n_students+j].get_response() << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;

    MH h;
    h.run(n_steps, n_burn);

    for (int i=0; i<n_students; i++) {
        std::cout << "Student " << i << ": "<< s[i].mean() << std::endl;
        //plt::hist(s[i].chain(), 50);
    }

    for (int i=0; i<n_questions; i++) {
        std::cout << "Question " << i << ": "<< q[i].mean() << std::endl;
        //plt::hist(q[i].chain(), 50);

    }

    std::cout << std::endl;
    std::cout << "Posterior Data" << std::endl;

    for (int i=0; i<n_students; i++) {
        for (int j=0; j<n_questions; j++) {
            if (s[i].mean() > q[j].mean()) {
                if (o[i*n_students+j].get_response() != 1)
                    std::cout << "1 ";
            }
            else {
                if (o[i*n_students+j].get_response() != 0)
                    std::cout << "1 ";
            }
        }
        std::cout << "Mispredictions are denoted by 1." << std::endl;
    }

    std::cout << std::endl;
    plt::show();
}
