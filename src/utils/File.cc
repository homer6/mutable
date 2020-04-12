#include "utils/File.h"

#include <iostream>
using std::endl;

#include <fstream>
#include <sstream>

#include <stdio.h>  //remove
#include <sys/stat.h>


namespace mtbl{
namespace utils{

	string file_get_contents( const string& filename ){

		std::ifstream current_file( filename );
		std::stringstream buffer;
		buffer << current_file.rdbuf();
		return buffer.str();

	}



	bool file_remove( const string& filename ){
			
		if( filename.size() ){
			if( remove(filename.c_str()) != 0 ){
				return false;
			}else{
				return true;
			}
		}

		return false;

	}


	json file_get_json( const string& filename ){

		return json::parse( file_get_contents(filename) );

	}



	uint64_t File::getSize(){

		struct stat st;
		if( stat( this->filename.c_str(), &st ) != 0 ){
			return 0;
		}
		return (uint64_t)st.st_size;

	}



	string File::getContents(){

		return file_get_contents( this->filename );

	}



	bool File::remove(){

		return file_remove( this->filename );

	}



	bool File::write( const string& contents ){

		if( this->filename.size() ){
			std::ofstream current_file( this->filename, std::ofstream::binary | std::ofstream::trunc );
			current_file << contents;
			current_file.close();
			return true;
		}
		return false;

	}



	bool File::append( const string& appended_contents ){

		if( this->filename.size() ){
			std::ofstream current_file( this->filename, std::ofstream::binary | std::ofstream::app );
			current_file << appended_contents;
			current_file.close();
			return true;
		}
		return false;

	}



	const string& File::getFilename() const{

		return this->filename;

	}


}
}

