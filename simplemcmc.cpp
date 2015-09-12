#include <random>
#include <string>
#include "matplotlibcpp.h"

using namespace std;
namespace plt = matplotlibcpp;

int main()
{
    int n = 5000; // number of data points

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(5, .1);

    vector<double> x(n);
    for(int i=0; i<n; ++i) {
        x.at(i) = d(gen);
    }

    plt::hist(x, 100);
    // show plots
    plt::show();
}
