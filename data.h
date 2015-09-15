#ifndef _DATA_H_
#define _DATA_H_

#include <map>
#include <random>
#include <array>
#include "variables.h"

// Variables
const int n_steps = 20000;
const int n_students = 10;
const int n_questions = 100;
const int n_answered_questions = 20;

std::array<Normal, n_students> s;
std::array<Normal, n_questions> q;
std::array<Observation, n_students*n_questions> o;

#endif
