#pragma once


#include <string>
using std::string;

#include "json.hpp"
using json = nlohmann::json;

namespace mtbl::utils{


	string file_get_contents( const string& filename );
	bool file_remove( const string& filename );


	class File{

		public:
			File( const string& filename )
				:filename(filename)
			{

			}

			// Returns the entire file's contents
			string getContents();

			// Returns the entire file's size
			uint64_t getSize();

			// Deletes the file
			bool remove();

			// Truncates files and makes the entire file's contents to be "contents".
			bool write( const string& contents );

			// Add "contents" to the end of the file.
			bool append( const string& appended_contents );


			const string& getFilename() const;


		protected:
			string filename;

	};



	/* 
		A TemporaryFile removes (deletes) the file when this object goes out of scope.
	*/
	class TemporaryFile : public File{

		public:
			TemporaryFile( const string& filename )
				:File(filename)
			{

			};

			~TemporaryFile(){

				this->remove();

			};

	};



}

