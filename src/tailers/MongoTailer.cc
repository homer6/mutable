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

using ::mtbl::tailer::MongoTailer;


void worker_thread( int, MongoTailer &mongo_tailer, const string collection_name ){

	mongo_tailer.tailCollection( collection_name );

};




#include "json.hpp"
using json = nlohmann::json;


namespace mtbl::tailer{


	MongoTailer::MongoTailer( const string& mongo_connection_string, const string& kafka_brokers_list, const string& database_name )
		:mongo_connection_string(mongo_connection_string), kafka_brokers_list(kafka_brokers_list), database_name(database_name)
	{






	}


	/*
				MongoClient mongo_client( this->mongo_connection_string );
				cppkafka::Configuration kafka_config = {
					{ "metadata.broker.list", this->broker_list }
				};
				cppkafka::Producer kafka_producer( kafka_config );

				*/



	void MongoTailer::tailDatabase(){

		vector<string> collection_names;

		{

			MongoClient mongo_client( this->mongo_connection_string );
			collection_names = mongo_client.getCollections( this->database_name );

		} //closes mongo connection



		vector<std::thread> threads;

		int worker_id = 0;

		for( const string& collection_name : collection_names ){
			cout << "Starting (" << collection_name << ") thread " << worker_id << endl;
			threads.push_back( std::thread(worker_thread, worker_id, std::ref(*this), collection_name) );
			cout << "Started thread (" << collection_name << ")" << worker_id << endl;
			worker_id++;
		}

		int x = 0;
		for( std::thread &current_thread : threads ){
			cout << "Joining thread " << x << endl;
			current_thread.join();
			x++;
		}


	}



	void MongoTailer::tailCollection( const string collection_name ){

		//int x = 0;
		

		using change_stream_options = mongocxx::options::change_stream;

		change_stream_options base_options;
		base_options.max_await_time( std::chrono::milliseconds{60000} );


		MongoClient mongo_client( this->mongo_connection_string );


		while( 1 ){

			auto collection = mongo_client.mongo_connection[ this->database_name ][ collection_name ];
			change_stream_options options{base_options};

			auto change_stream = collection.watch(options);

			for( const bsoncxx::document::view &document_view : change_stream ){

				cout << bsoncxx::to_json(document_view) << endl;

				//json json_object = json::parse( string(bsoncxx::to_json(document_view)) );
				//cout << json_object.dump(4) << endl;

			}

			//cout << x << ": " << collection_name << endl;

		}




		/*
		const string message_contents = message.dump();
		producer.produce( cppkafka::MessageBuilder(topic).partition(-1).payload(message_contents) );
		producer.flush();
		*/


	}



}