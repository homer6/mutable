#include "tailers/MongoTailer.h"


namespace mtbl{
namespace tailer{

	MongoTailer::MongoTailer( MongoClient& mongo_client, cppkafka::Producer& kafka_producer )
		:mongo_client(mongo_client), kafka_producer(kafka_producer)
	{



	}



	void MongoTailer::tail(){

		/*
		const string message_contents = message.dump();
		producer.produce( cppkafka::MessageBuilder(topic).partition(-1).payload(message_contents) );
		producer.flush();
		*/

	}





}
}