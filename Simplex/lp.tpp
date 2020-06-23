#include <vector>
#include <algorithm> 
#include <ios>
#include <iomanip>
#include <iostream>
#include <fstream>
#include "lp.h"




template <typename T>
tableau<T>::tableau() : nrow(0), ncol(0), state(EMPTY)
{}

template <typename T>
tableau<T>::tableau(std::ifstream &f){
	if(!f.is_open()){
		throw "File could not be opened";
	}

	// read the first two lines. These are reserved to show the number of rows and columns in the LP
	f >> nrow >> ncol;
	
	// resize the vectors of tableau to the given sizes. Here we directly add the slack variables to the matrix giving us a lp in standard equality form and also a trivial base.
	A.resize(nrow);
	for (auto &v : A){
		v.resize(ncol + 1);
	}
	bounds.resize(nrow);
	costs.resize(ncol+1);
	base.resize(nrow);
	nonbase.resize(ncol);

	// read the costs
	costs[0] = 0;
	for(unsigned int i = 0; i < ncol; i++){
		f >> costs[i+1];
	}
	obj = costs;

	// read the bounds
	for(unsigned int i = 0; i < nrow; i++){
		f >> bounds[i];
	}

	// read the matrix
	for(unsigned int i = 0; i < nrow; i++){
		A[i][0] = bounds[i];
		for(unsigned int j = 1; j <= ncol; j++){
			f >> A[i][j];
			A[i][j] = -A[i][j];
		} 
	}

	// current base
	for(unsigned int i = 0; i < nrow; i++){
		base[i] = ncol + i;
	}
	for(unsigned int i = 0; i < ncol;i++){
		nonbase[i] = i;
	}

	state = INDETERMINATE;
}

template <typename T>
void tableau<T>::print_state(){
	std::cout << "State: ";
	if(state & EMPTY)
		std::cout << "EMPTY ";
	if(state & INDETERMINATE)
		std::cout << "INDETERMINATE ";
	if(state & SOLVED)
		std::cout << "SOLVED ";
	if(state & UNBOUNDED)
		std::cout << "FEASIBLE ";
	if(state & INFEASIBLE)
		std::cout << "INFEASIBLE ";
	std::cout << std::endl;
}

template <typename T>
void print_vector(std::vector <T> const &a) {
   std::cout << "[ ";
   for(int i=0; i < static_cast<int>(a.size()); i++)
      std::cout << a.at(i) << " ";
   std::cout << "] ";
}

template <typename T>
void print_vec_vec(std::vector <std::vector <T>> const &a) {
   std::cout << "[ ";
   for(int i=0; i < static_cast<int>(a.size()); i++)
      print_vector(a[i]);
   std::cout << "] ";
}

template <typename T>
void tableau<T>::print_tableau(){
	std::cout  << "\nBase: ";
	print_vector(base);
	std::cout  << "  Nonbase: ";
	print_vector(nonbase);
	std::cout  << "\n";
	std::cout  << "Tableau Matrix: \n";
	print_vec_vec(A);
	std::cout  << "\n";
	std::cout  << "objective function: \n";
	print_vector(costs);
	std::cout  << "\n\n";
}


template <typename T>
bool tableau<T>::is_optimal(){
	for(unsigned int i=1; i < costs.size(); i++){
		if(costs[i] > 0){
			return false;
		} 
	}
	return true;
}

// helper methods
template <typename T>
std::vector<T>  div(std::vector <T> a, T b) {
   for(unsigned int i=0; i < a.size(); i++)
      a[i] = a[i]/b;
  	return a;
}

template <typename T>
std::vector<T>  rev(std::vector <T> a, T b, std::vector <T> c) {
   for(unsigned int i=0; i < a.size(); i++)
      a[i] = a[i]+b*c[i];
  	return a;
}

template <typename T>
void tableau<T>::swap(){
	// we use blands rule

	unsigned int index_1 = 0;
	unsigned int index_2 = 0;
	for(unsigned int i = costs.size()-1; i > 0; i--){
		if(costs[i] > 0){
			index_1 = i;
		}
	}
	if(index_1 == 0){
		return;
	}

	T temp = std::numeric_limits<T>::lowest();
	for(unsigned int i = 0; i < base.size(); i++){
		if (A[i][index_1] < 0 && temp < A[i][0]/A[i][index_1]){
			index_2 = i+1;
			temp = A[i][0]/A[i][index_1];
		}
	}
	if(index_2 == 0){
		// the problem is unbounded
		state = UNBOUNDED;
		return;
	}
	index_2 -= 1;

	unsigned int switch_1 = nonbase[index_1-1];
	unsigned int switch_2 = base[index_2];

	std::replace (base.begin(), base.end(), switch_2, switch_1);
	std::replace (nonbase.begin(), nonbase.end(), switch_1, switch_2);

	T factor = A[index_2][index_1];

	A[index_2][index_1] = -1;
	A[index_2] = div(A[index_2],-factor);
	//A[index_2] = static_cast< std::vector<T>>(A[index_2])*factor;

	for (unsigned int i = 0; i < A.size(); i++){
		if(i != index_2){
			factor = A[i][index_1];
			A[i][index_1] = 0;
			A[i] = rev(A[i],factor,A[index_2]);//A[i]+factor*A[index_2];
		}
	}


	factor = costs[index_1];
	costs[index_1] = 0;
	costs = rev(costs,factor,A[index_2]);

}



template <typename T>
std::vector<T> tableau<T>::get_solution(){
	std::cout << "\n";
	std::vector<T> res = {};
	for(unsigned int i = 0; i < ncol; i++){
		if(std::find(base.begin(), base.end(), i) != base.end()){
			res.push_back(A[std::distance(base.begin(), std::find(base.begin(), base.end(), i))][0]);
		} else{
			res.push_back(0);
		}
	}
	for(unsigned int i = ncol; i < ncol+nrow; i++){
		if(std::find(base.begin(), base.end(), i) != base.end()){
			if (A[std::distance(base.begin(), std::find(base.begin(), base.end(), i))][0] < 0){
				std::cout << "The Problem is not feasible";
				return res;
			}
		}
	}
	std::cout << "The solution vector ist : ";
	print_vector(res);
	std::cout << "giving us a solution of : ";
	int sum = obj[0];
	for(unsigned int i = 0; i<res.size(); i++){
		sum += res[i]*obj[i+1];
	}
	std::cout << sum;
	std::cout << "\n";
	return res;
}

template <typename T>
int tableau<T>::simplex(){
	bool run = true;
	while(run){
		//print_tableau();
		swap();
		if(state==UNBOUNDED){
			run = false;
		}
		if (is_optimal()){
			state = SOLVED;
			run = false;
		}
	}
	//print_tableau();
	return state;
}
