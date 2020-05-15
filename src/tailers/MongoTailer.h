#pragma once

#include <string>
using std::string;

#include "clients/MongoClient.h"
using ::mtbl::client::MongoClient;

#include <cppkafka/cppkafka.h>


namespace mtbl{
namespace tailer{

	class MongoTailer{

		public:
			MongoTailer( MongoClient& mongo_client, cppkafka::Producer& kafka_producer, const string& database_name );

			void tail();


		protected:
			MongoClient& mongo_client;
			cppkafka::Producer& kafka_producer;
			const string database_name;

	};




}
}