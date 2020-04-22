#pragma once

#include <string>
using std::string;

#include <map>
using std::map;

#include <vector>
using std::vector;


namespace mtbl{

	class Mutable{
		
		public:
			Mutable( int argc, char** argv );

			void loadEnvironmentVariables();
			string getEnvironmentVariable( const string& variable_name, const string& default_value = "" ) const;
			void setEnvironmentVariable( const string& variable_name, const string& variable_value );

			bool run();



		protected:
			map<string,string> environment_variables;

	};

}