#pragma once

#include <string>
using std::string;

#include <cppkafka/cppkafka.h>


namespace mtbl::client{

	class KafkaProducer{

		public:
			KafkaProducer( const string connection_string );

			cppkafka::Producer producer;

	};

}