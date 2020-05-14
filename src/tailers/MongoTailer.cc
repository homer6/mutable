#include "tailers/MongoTailer.h"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <chrono>
#include <thread>

#include "json.hpp"
using json = nlohmann::json;

namespace mtbl{
namespace tailer{

	MongoTailer::MongoTailer( MongoClient& mongo_client, cppkafka::Producer& kafka_producer, const string& database_name, const string& collection_name )
		:mongo_client(mongo_client), kafka_producer(kafka_producer), database_name(database_name), collection_name(collection_name)
	{



	}



	void MongoTailer::tail(){


		auto collection = this->mongo_client.mongo_connection[ this->database_name ][ this->collection_name ];

		auto change_stream_it = collection.watch();

		while( 1 ){

			for( const bsoncxx::document::view &document_view : change_stream_it ){

				//cout << bsoncxx::to_json(document_view) << endl;

				json json_object = json::parse( string(bsoncxx::to_json(document_view)) );
				cout << json_object.dump(4) << endl;

			}

			std::this_thread::sleep_for( std::chrono::seconds(1) );

		}



		/*
		const string message_contents = message.dump();
		producer.produce( cppkafka::MessageBuilder(topic).partition(-1).payload(message_contents) );
		producer.flush();
		*/

	}





}
}