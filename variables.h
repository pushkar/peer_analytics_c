#ifndef _VARIABLES_H_
#define _VARIABLES_H_

#include <iostream>
#include <cstdarg>
#include <random>
#include <string>

std::random_device rd;
std::mt19937 gen(rd());

float random_(int min, int max) {
    std::uniform_real_distribution<> d(min, max);
    return d(gen);
}


class Chain {
protected:
    std::vector<double> x;
    double mu;
    double std;

public:
    Chain() {

    }

    void stats() {
        double sum = std::accumulate(x.begin(), x.end(), 0.0);
        mu = sum / x.size();

        double sq_sum = std::inner_product(x.begin(), x.end(), x.begin(), 0.0);
        std = std::sqrt(sq_sum / x.size() - mu*mu);
    }

    double mean() {
        return mu;
    }

    double stdev() {
        return std;
    }

    void insert(double n) {
        x.push_back(n);
    }

    void initialize(double value) {
        x.clear();
        insert(value);
    }

    double last() {
        return x.back();
    }

    int size() {
        return x.size();
    }

    std::vector<double> chain() {
        return x;
    }

    void burn(int n) {
        x.erase(x.begin(), x.begin()+n);
    }

};

class Normal : public Chain {
    int observed;
    std::normal_distribution<> dist;

public:
    Normal() {
        observed = 0;
    }

    void set_params(double mean, double sigma) {
        std::normal_distribution<> dist(mean, sigma);
    }

    void initialize() {
        x.clear();
        insert(dist.mean());
    }

    double sample() {
        return dist(gen);
    }
};


// -1 = Not Assigned
class Observation {
    int value;
    int value_given;
    float proficiency, hardness;
public:
    Observation() {
        value = -1;
        value_given = -1;
    }

    void set(float _proficiency, float _hardness) {
        proficiency = _proficiency;
        hardness = _hardness;
        //if ((1.0 / (1 + exp(-proficiency-hardness) )) > 0)
        if (_proficiency > _hardness)
            value = 1;
        else
            value = 0;
    }

    void set(int _value) {
        value_given = _value;
    }

    float diff() {
        if (value_given != -1) {
            return pow(value-value_given, 2);
        }
        return 0;
    }

};

#endif
