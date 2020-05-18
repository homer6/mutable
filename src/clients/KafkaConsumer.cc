#include "clients/KafkaConsumer.h"


namespace mtbl::client{

	KafkaConsumer::KafkaConsumer( const string connection_string, const string consumer_group )
		:consumer( 
			cppkafka::Configuration{
				{ "metadata.broker.list", connection_string },
				{ "group.id", consumer_group },
				{ "enable.auto.commit", false }
			}
		)
	{



	}


}