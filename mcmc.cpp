#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>

#include "data.h"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

double true_mean = 0.35;
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

class MH {
public:
    MH() {

    }

    void run(const int n, const int burn_n) {
        for (int i=0; i<s.size(); i++) {
            s[i].initialize();
        }

        for (int i=1; i<n_steps; ++i) {
            for (int s_i=0; s_i<s.size(); s_i++) {
                double y = s[s_i].sample();
                double old = s[s_i].last();
                double r = exp(likelihood(y) - likelihood(old));
                if (random_(0, 1) < r)
                    s[s_i].insert(y);
                else
                    s[s_i].insert(old);
                }


            // show the step_n :: difference
            std::cout << std::right;
            if (i % (n_steps/200) == 0) {
                int done = (i*100.0/n_steps);
                std::cout << "[" << std::setw(3) << done << "%]: " << "\r" << std::flush;
            }

        }

        for (int i=0; i<s.size(); i++) {
            s[i].burn(burn_n);
            s[i].stats();
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
    }

    MH h;
    h.run(n_steps, n_burn);

    std::cout << s[0].mean() << std::endl;
    std::cout << s[0].stdev() << std::endl;
    plt::hist(s[0].chain(), 50);
    plt::show();
}
