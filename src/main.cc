#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include "Mutable.h"

#include <mongocxx/instance.hpp>

#include <stdexcept>


int main( int argc, char** argv ){
	

	try{

		mongocxx::instance mongo_instance{}; //must be initialized exactly once in a program

		mtbl::Mutable mtbl( argc, argv );

		if( !mtbl.run() ){

			cerr << "Mutable run failed." << endl;

			return -3;
		}

		return 0;

	}catch( std::exception &e ){

		cerr << "Mutable exception caught: " << e.what() << endl;

		return -1;

	}catch( ... ){

		cerr << "Mutable unknown exception caught." << endl;

		return -2;

	}



}


