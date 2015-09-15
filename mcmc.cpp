#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>

#include "data.h"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;


class MH {
public:
    MH() {

    }

    void run(const int n) {
        for (int i=0; i<s.size(); i++) {
            s[i].initialize();
        }

        for (int i=1; i<n_steps; ++i) {
            for (int s_i=0; s_i<s.size(); s_i++) {
                double y = s[s_i].sample();

                double old = s[s_i].last();
                double r = std::min(normal_custom(y)/normal_custom(old), 1.0);
                if (random_(0, 1) < r)
                    s[s_i].insert(y);
                else
                    s[s_i].insert(old);

            }
        }

        for (int i=0; i<s.size(); i++) {
            s[i].stats();
        }
    }
};


int main()
{

    for (int i=0; i<n_students; i++) {
        s[i].set_params(0.1*i, 0.1);
    }

    MH h;
    h.run(n_steps);

    std::cout << s[0].mean() << std::endl;
    std::cout << s[0].stdev() << std::endl;
    plt::hist(s[0].chain(), 50);
    plt::hist(s[2].chain(), 50);
    plt::hist(s[3].chain(), 50);
    plt::show();


        /*
        // show the step_n :: difference
        std::cout << std::right;
        if (n % (n_steps/200) == 0) {
            int done = (n*100.0/n_steps);
            std::cout << "[" << std::setw(3) << done << "%]: " << diff << "\r" << std::flush;
        }


    std::cout << std::flush << std::endl;
    std::cout << "Done." << std::endl;
    */
}
