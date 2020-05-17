#pragma once

#include <string>
using std::string;

#include "clients/MongoClient.h"
using ::mtbl::client::MongoClient;

#include <cppkafka/cppkafka.h>


namespace mtbl::tailer{


	class MongoTailer{

		public:
			MongoTailer( const string& mongo_connection_string, const string& kafka_brokers_list, const string& database_name );

			void tailDatabase();
			void tailCollection( const string collection_name );


		protected:
			const string mongo_connection_string;
			const string kafka_brokers_list;
			const string database_name;

	};



}