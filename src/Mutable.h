#pragma once

#include <string>
using std::string;

#include <map>
using std::map;

#include <vector>
using std::vector;

#include "json.hpp"
using json = nlohmann::json;


namespace mtbl{

	class Mutable{
		
		public:
			Mutable( int argc, char** argv );

			void loadEnvironmentVariables();
			string getEnvironmentVariable( const string& variable_name, const string& default_value = "" ) const;
			void setEnvironmentVariable( const string& variable_name, const string& variable_value );

			bool run();

			void publishKafkaMessage( const string topic, const json& message ) const;


		protected:
			map<string,string> environment_variables;

			string environment_prefix;

			string broker_list;
			string mongo_connection;
			string mysql_host;
			string mysql_port;
			string mysql_username;
			string mysql_password;
			string mysql_database;
			string postgres_connection;

			string command;

			string tail_source;

			string consume_topic;
			string consume_consumer_group;
			string consume_type;

			string walk_source;
			string walk_object;

			string mutate_chain;
			int32_t mutate_version = -1;


	};

}