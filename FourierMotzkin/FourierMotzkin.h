#ifndef FOURIERMOTZKIN_H
#define FOURIERMOTZKIN_H

#include <vector>

void print_vector(std::vector <double> const &a);
void print_vec_vec(std::vector <std::vector <double>> const &a);
void fm_simp(std::vector<std::vector<double>> &mat, std::vector<double> &b);
bool fm(std::vector<std::vector<double>> mat, std::vector<double> b, std::vector<double> dim);

#endif