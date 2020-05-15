#include "tailers/MongoTailer.h"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <chrono>
#include <thread>

#include <map>
using std::map;



#include "json.hpp"
using json = nlohmann::json;

namespace mtbl{
namespace tailer{

	MongoTailer::MongoTailer( MongoClient& mongo_client, cppkafka::Producer& kafka_producer, const string& database_name )
		:mongo_client(mongo_client), kafka_producer(kafka_producer), database_name(database_name)
	{



	}



	void MongoTailer::tail(){

		int x = 0;

		vector<string> collection_names;

		using change_stream_options = mongocxx::options::change_stream;


		change_stream_options base_options;
		base_options.max_await_time( std::chrono::milliseconds{100} );

		while( 1 ){

			collection_names = this->mongo_client.getCollections( this->database_name );

			while( x < 100 ){  //check for new collections every 100 iterations

				for( const string& collection_name : collection_names ){

					auto collection = this->mongo_client.mongo_connection[ this->database_name ][ collection_name ];
					change_stream_options options{base_options};

					auto change_stream = collection.watch(options); 

					for( const bsoncxx::document::view &document_view : change_stream ){

						cout << bsoncxx::to_json(document_view) << endl;

						//json json_object = json::parse( string(bsoncxx::to_json(document_view)) );
						//cout << json_object.dump(4) << endl;

					}

					cout << x << ": " << collection_name << endl;

				}



				x++;

			}

			std::this_thread::sleep_for( std::chrono::seconds(1) );

			x = 0;

		}



		/*
		const string message_contents = message.dump();
		producer.produce( cppkafka::MessageBuilder(topic).partition(-1).payload(message_contents) );
		producer.flush();
		*/

	}





}
}