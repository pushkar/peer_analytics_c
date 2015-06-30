#include <random>
#include <string>

std::random_device rd;
std::mt19937 gen(rd());

float random_(int min, int max) {
    std::uniform_real_distribution<> d(min, max);
    return d(gen);
}


class Student {
    float old_proficiency;
    float proficiency;
    int proficiency_observed;
    std::normal_distribution<> dist;

public:
    Student() {
        proficiency_observed = 0;
    }

    void set_normal_distribution(float mu, float sigma) {
        std::normal_distribution<> dist(mu, sigma);
        proficiency = dist(gen);
    }

    float get_proficiency() {
        return proficiency;
    }

    void set_proficiency(float h) {
        proficiency = h;
        proficiency_observed = 1;
    }

    void step() {
        old_proficiency = proficiency;
        if(proficiency_observed == 0)
            proficiency = dist(gen);
    }

    void accept() {

    }

    void reject() {
        proficiency = old_proficiency;
    }
};

class Question {
    float old_hardness;
    float hardness;
    int hardness_observed;
    std::normal_distribution<> dist;

public:
    Question() {
        hardness_observed = 0;
    }

    void set_normal_distribution(float mu, float sigma) {
        std::normal_distribution<> dist(mu, sigma);
        hardness = dist(gen);
    }

    float get_hardness() {
        return hardness;
    }

    void set_hardness(float h) {
        hardness = h;
        hardness_observed = 1;
    }

    void step() {
        old_hardness = hardness;
        if(hardness_observed == 0)
            hardness = dist(gen);
    }

    void accept() {

    }

    void reject() {
        hardness = old_hardness;
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
