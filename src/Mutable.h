#pragma once

#include <string>
using std::string;

#include <map>
using std::map;

#include <vector>
using std::vector;

#include "json.hpp"
using json = nlohmann::json;

#include <memory>
using std::unique_ptr;

#include "clients/PostgresClient.h"
using ::mtbl::client::PostgresClient;

#include "clients/MongoClient.h"
using ::mtbl::client::MongoClient;

#include "clients/MysqlClient.h"
using ::mtbl::client::MysqlClient;

#include "clients/ElasticSearchClient.h"
using ::mtbl::client::ElasticSearchClient;

#include "clients/RedisClient.h"
using ::mtbl::client::RedisClient;

#include "clients/KafkaProducer.h"
using ::mtbl::client::KafkaProducer;

#include "clients/KafkaConsumer.h"
using ::mtbl::client::KafkaConsumer;


namespace mtbl{

	namespace client{
		class PostgresClient;
		class MongoClient;
		class MysqlClient;
		class KafkaProducer;
		class KafkaConsumer;
		class RedisClient;
		class ElasticSearchClient;
	}

	class Mutable{
		
		public:
			Mutable( int argc, char** argv );

			void loadEnvironmentVariables();
			string getEnvironmentVariable( const string& variable_name, const string& default_value = "" ) const;
			void setEnvironmentVariable( const string& variable_name, const string& variable_value );

			bool run();

			void publishKafkaMessage( const string topic, const json& message );

			unique_ptr<PostgresClient> createPostgresClient() const;
			unique_ptr<MongoClient> createMongoClient() const;
			unique_ptr<MysqlClient> createMysqlClient() const;
			unique_ptr<KafkaProducer> createKafkaProducer() const;
			unique_ptr<KafkaConsumer> createKafkaConsumer() const;
			unique_ptr<RedisClient> createRedisClient() const;
			unique_ptr<ElasticSearchClient> createElasticSearchClient() const;

			void connectKafkaProducer();


		protected:
			map<string,string> environment_variables;

			string environment_prefix;

			string broker_list;
			string mongo_connection;
			string mysql_connection;			
			string postgres_connection;
			string elasticsearch_connection;
			string redis_connection;

			string command;

			string tail_source;
			string tail_database;

			string consume_topic;
			string consume_consumer_group;
			string consume_type;

			string walk_source;
			string walk_object;

			string mutate_chain;
			int32_t mutate_version = -1;

			string test_name;
			string test_database;

			unique_ptr<client::KafkaProducer> main_producer;

	};

}