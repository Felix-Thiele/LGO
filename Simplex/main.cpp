#include "lp.h"
#include <fstream>
#include <iostream>

void usage(const char* pname){
	std::cout << "Usage: " << pname << " filename" << std::endl;
}

std::ifstream parse_args(int argc, char **argv){
	std::ifstream f;
	if (argc == 1){
		usage(argv[0]);
		return(f);
	}
	auto fname = argv[1];
	f.open(fname);
	return f;
}

int main(int argc, char **argv){
	try{
	auto file = parse_args(argc, argv);
	if(!file.is_open())
		throw "File couldn't be opened";

	auto tabl = tableau<double>(file);
	file.close();

	tabl.simplex();
	if(tabl.state == SOLVED){
		tabl.get_solution();
	}else if (tabl.state == UNBOUNDED){
		std::cout << "This problem is unbounded";
	}



	}catch (char const* e){
		std::cout << e << std::endl;

	}
	return(0);
}