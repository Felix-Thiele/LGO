#include <vector>
#include <iostream>
#include <cassert>
#include <algorithm> 

void print_vector(std::vector <double> const &a) {
   std::cout << "[ ";
   for(int i=0; i < static_cast<int>(a.size()); i++)
      std::cout << a.at(i) << " ";
   std::cout << "] ";
}

void print_vec_vec(std::vector <std::vector <double>> const &a) {
   std::cout << "[ ";
   for(int i=0; i < static_cast<int>(a.size()); i++)
      print_vector(a[i]);
   std::cout << "] ";
}


void fm_simp(std::vector<std::vector<double>> &mat, std::vector<double> &b, std::vector<std::vector<double>> &certificate){
	// this function takes an inequality system and reduces it to another one in one less variable
	std::vector<double> lessthan 		= {};
	std::vector<double> greaterthan 	= {};
	std::vector<double> same		 	= {};


	std::vector<std::vector<double>> new_mat = {};
	std::vector<double> new_b = {};
	//isolate a variable
	for (int i = 0; i < static_cast<int>(mat.size()); i++){
		double first = mat[i].front();
		mat[i].erase(mat[i].begin());
		if (first != 0){
			b[i] = -1*b[i]/first;
			for (int j = 0; j < static_cast<int>(mat[i].size()); j++){
				mat[i][j] = -1*mat[i][j]/first;
			}
			for (int j = 0; j < static_cast<int>(certificate[i].size()); j++){
				certificate[i][j] = -1*certificate[i][j]/first;
			}
			if (first < 0){
				lessthan.push_back(i);
			}else{
				greaterthan.push_back(i);
			}
		}else{
			same.push_back(i);
		}
	}

	std::vector<std::vector<double>> new_cert = {};

	//create the new inequalities
	for (int i = 0; i < static_cast<int>(lessthan.size()); i++){
		for (int j = 0; j < static_cast<int>(greaterthan.size()); j++){
			std::vector<double> cur;
			std::vector<double> temp;
			assert(mat[lessthan[i]].size() == mat[greaterthan[j]].size());
			for (int t=0; t < static_cast<int>(mat[lessthan[i]].size()); t++){
				cur.push_back(mat[lessthan[i]][t]-mat[greaterthan[j]][t]);
			}
			for (int t=0; t < static_cast<int>(certificate[lessthan[i]].size()); t++){
				temp.push_back(certificate[lessthan[i]][t]-certificate[greaterthan[j]][t]);
			}
			new_mat.push_back(cur);
			new_b.push_back(b[lessthan[i]]-b[greaterthan[j]]);
			new_cert.push_back(temp);
		}		
	}
	// the old inequalities that were independant form the variable are added to the new system
	for(int i = 0; i < static_cast<int>(same.size()); i++){
		new_mat.push_back(mat[same[i]]);
		new_b.push_back(b[same[i]]);
		new_cert.push_back(certificate[same[i]]);
	}

	mat = new_mat;
	b = new_b;
	certificate = new_cert;
}

double findsolution(std::vector<std::vector<double>> mat, std::vector<double> b, std::vector<double> values){
	//given a system of inequality systems, where consecutive system have one variable less, find a solution
	if (static_cast<int>(values.size()) != 0){
		for (int var = static_cast<int>(values.size())-1; var >= 0; var--) {
		 	for (int line = 0; line < static_cast<int>(mat.size()); line++) {
		 		double back = mat[line].back();
		 		mat[line].pop_back();
		 		b[line] -= back*values[var];
			}
		}
	}
	double gr = -999;
	double sm = 999;
	for (int line = 0; line < static_cast<int>(b.size()); line++) {
		if (mat[line][0] != 0){
			b[line] /= mat[line][0];
			if (mat[line][0]>0){
				if (b[line]<sm){
					sm = b[line];	
				}
			} else {
				if (b[line]>gr){
					gr = b[line];	
				}
			}
		}
	}
	return (sm+gr)/2;
}

bool fm(std::vector<std::vector<double>> mat, std::vector<double> b, std::vector<double> dim){

	std::vector<std::vector<double>> b_s = {};
	std::vector<std::vector<std::vector<double>>> mat_s = {};

	b_s.push_back(b);
	mat_s.push_back(mat);
	// create id matrix
	std::vector<std::vector<double>> certificate = {};
	for(int i = 0; i < static_cast<int>(mat.size()); i++){
		std::vector<double> cur;
		for(int j = 0; j < static_cast<int>(mat.size()); j++){
			if (j==i){
				cur.push_back(1);
			} else {
				cur.push_back(0);
			}
		}
		certificate.push_back(cur);
	}
	// repeat variable reduction until no variables are left
	for(int i = 1; i < dim[1]+1; i++){
		fm_simp(mat, b, certificate);
		b_s.push_back(b);
		mat_s.push_back(mat);
	}

	if (std::all_of(b.cbegin(), b.cend(), [](int i){ return i > 0; }))  
    { 
    	// calculate the values of the variables
		std::vector<double> values = {};
		for (int i = static_cast<int>(b_s.size())-2; i >=0; i--){
			double v = findsolution(mat_s[i], b_s[i], values);
			values.insert(values.begin(), v);
		}
		print_vector(values);
        return true;
    } else{
    	int i = 0;
    	while(b[i] >0){
    		i++;
    	}
    	std::cout << "empty ";
    	print_vector(certificate[i]);
    	return false;
    }
}

