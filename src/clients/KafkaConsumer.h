#pragma once

#include <string>
using std::string;

#include <cppkafka/cppkafka.h>


namespace mtbl::client{

	class KafkaConsumer{

		public:
			KafkaConsumer( const string connection_string, const string consumer_group );

			cppkafka::Consumer consumer;

	};

}