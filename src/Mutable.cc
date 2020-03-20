#include "Mutable.h"

#include "json.hpp"
using json = nlohmann::json;

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.hpp"

#include <iostream>
using std::cout;
using std::endl;


namespace mtbl{

	Mutable::Mutable( int, char** ){


	}



	void Mutable::run(){

		json running_dialogue{
			{ "hey", "there" },
			{ "how", "are" },
			{ "you", "!" }
		};

		cout << running_dialogue.dump(4) << endl;

	}


}