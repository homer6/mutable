#pragma once

#include <string>
using std::string;

#include <vector>
using std::vector;


namespace mtbl{

	class Mutable{
		
		public:
			Mutable( int argc, char** argv );

			void run();

	};

}