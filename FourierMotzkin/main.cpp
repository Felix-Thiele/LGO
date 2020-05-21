#include "FourierMotzkin.h"
#include "input.h"
#include <vector>
#include <iostream>

int main() {
	std::vector<std::vector<double>> a = read_file();
	std::vector<double> dim = a[0];
	std::vector<double> c = a[1];
	std::vector<double> b = a[2];
	std::vector<std::vector<double>> mat = {a.begin() + 3, a.end()};;

	fm(mat, b,dim);
}