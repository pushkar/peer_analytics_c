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

    void initialize() {
        //x.clear();
        //insert(dist.mean());
    }

    void set_params(double mean, double sigma) {
        std::normal_distribution<> dist(mean, sigma);
        x.push_back(mean);
    }

    double sample() {
        return dist(gen);
    }
};


// -1 = Not Assigned
class Observation {
    int s_id, q_id, correct;
public:
    void set(int _s_id, int _q_id, int _correct) {
        s_id = _s_id;
        q_id = _q_id;
        correct = _correct;
    }

    Observation() {
    }

    int get_sid() {
        return s_id;
    }

    int get_qid() {
        return q_id;
    }

    int get_response() {
        return correct;
    }

};

#endif
