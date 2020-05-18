#include "clients/KafkaProducer.h"


namespace mtbl::client{

	KafkaProducer::KafkaProducer( const string connection_string )
		:producer( cppkafka::Configuration{ { "metadata.broker.list", connection_string } } )
	{



	}


}