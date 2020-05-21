#include "input.h"


std::vector<double> split(std::string str){
	std::vector<double> result = {};
	std::string temp = "";
	for (std::string::size_type i=0; i < str.size(); i++){
		if (str[i] == ' '){
			if (temp != ""){
				result.push_back(std::stod(temp));
			}
			temp = "";
		}else{
			temp.push_back(str[i]);
		}
	}
	result.push_back(std::stod(temp));
	return result;
}

std::vector<std::vector<double>> read_file() {
	//this reads the input file and returns a vector of double vectors namely a 
	std::vector<std::vector<double>> a = {};
	std::ifstream file("input.txt");
	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			std::vector<double> result = split(line.c_str());
		    a.push_back(result);
		}
		file.close();
	}
	return a;
}
