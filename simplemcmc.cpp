#include <random>
#include <string>
#include <math.h>
#include "matplotlibcpp.h"

using namespace std;
namespace plt = matplotlibcpp;


void find_mean_stdev(vector<double>* c) {
    double sum = std::accumulate(c->begin(), c->end(), 0.0);
    double mean = sum / c->size();

    double sq_sum = std::inner_product(c->begin(), c->end(), c->begin(), 0.0);
    double stdev = std::sqrt(sq_sum / c->size() - mean * mean);

    std::cout << "Mean is " << mean << std::endl;
    std::cout << "Var is " << stdev << std::endl;
}

double normal_custom(double x) {
    double mean = 4;
    double sigma = 0.1;
    return exp(-(x-mean)*(x-mean)/(2*sigma*sigma))/(sigma*sqrt(2*M_PI));
}

int main()
{
    int n = 5000; // number of data points
    std::random_device rd;
    std::mt19937 gen(rd());

    // Monte Carlo
    std::normal_distribution<> norm(5.0, 0.1);
    vector<double> c(n);
    for(int i=0; i<n; ++i) {
        c.at(i) = norm(gen);
    }

    find_mean_stdev(&c);
    plt::hist(c, 50);
    plt::show();

    // Metropolis-Hastings Algorithm
    double initial = 2.0;

    vector<double> x(n);
    x.at(0) = initial + 1; // JLT

    for(int i=1; i<n; ++i) {
        std::normal_distribution<> proposal(x.at(i-1), 0.1);
        double y = proposal(gen);
        // Its ok to use the same generator, but you have to recreate the distribution each time.
        double r = min(normal_custom(y)/normal_custom(x.at(i-1)), 1.0);
        if (rand() / double(RAND_MAX) < r)
            x.at(i) = y;
        else
            x.at(i) = x.at(i-1);
        // Could do log likelihood here to avoid divisions, but it works!
    }

    find_mean_stdev(&x);
    plt::hist(x, 50);
    plt::show();
}
