#ifndef LP_H
#define LP_H

#include<vector>
#include <algorithm>    // std::replace
#include<iostream>
#include<fstream>
#include<limits>

enum STATE
{
	EMPTY = 1,
	INDETERMINATE = 2,
	SOLVED = 4,
	UNBOUNDED = 8,
	INFEASIBLE = 16
};

double eps = 0.00001;

template <typename T>
using Matrix = std::vector<std::vector<T>>;


template <typename T>
class tableau{
	public:
		
		tableau();
		tableau(std::ifstream &f);

		void print_state();

		void print_tableau();

		bool is_optimal();

		void swap();

		std::vector<T> get_solution();

		int simplex();

		int state;

	private:

		std::vector<int> base;
		std::vector<int> nonbase;

		Matrix<T> A;
		std::vector<T> bounds;
		std::vector<T> costs;
		std::vector<T> obj;
		unsigned int nrow, ncol;

};

#include "lp.tpp"

#endif