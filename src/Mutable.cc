#include "Mutable.h"

#define CPPHTTPLIB_OPENSSL_SUPPORT
#define CPPHTTPLIB_ZLIB_SUPPORT
#include "httplib.hpp"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <stdexcept>

extern char **environ;

#include "clients/PostgresClient.h"
using ::mtbl::client::PostgresClient;

#include "clients/MongoClient.h"
using ::mtbl::client::MongoClient;

#include "clients/MysqlClient.h"
using ::mtbl::client::MysqlClient;

#include "tailers/MongoTailer.h"
using mtbl::tailer::MongoTailer;

#include "chains/mutable/MutableChain.h"
using mtbl::chains::mtbl::MutableChain;

#include <cppkafka/cppkafka.h>

#include "args.hxx"


namespace mtbl{

	Mutable::Mutable( int argc, char** argv ){

		this->loadEnvironmentVariables();

		args::ArgumentParser arg_parser( "mutable", "https://github.com/homer6/mutable" );
		
		args::Group commands( arg_parser, "commands" );

		args::Command tail_command( commands, "tail", "captures data changes (CDC) from a database and sends them to kafka" );
		args::Positional<string> tail_source( tail_command, "source", "type of database to tail" );
		args::Positional<string> tail_database( tail_command, "database", "the database name to tail" );

		args::Command consume_command( commands, "consume", "consume from kafka" );
		args::Positional<string> consume_topic( consume_command, "topic", "topic to consume from" );
		args::Positional<string> consume_consumer_group( consume_command, "consumer_group", "kafka consumer group" );
		args::Positional<string> consume_type( consume_command, "type", "type of consumer" );

		args::Command walk_command( commands, "walk", "instruct the walker to walk" );
		args::Positional<string> walk_source( walk_command, "source", "type of database to walk" );
		args::Positional<string> walk_object( walk_command, "object", "type of object (table, collection) to walk" );

		args::Command mutate_command( commands, "mutate", "delare a chain to be a mutation state" );
		args::Positional<string> mutate_chain( mutate_command, "chain", "name of chain to mutate" );
		args::Positional<int32_t> mutate_version( mutate_command, "version", "declared " );

		args::Command test_command( commands, "test", "perform a test" );
		args::Positional<string> test_name( test_command, "name", "name of the test" );
		args::ValueFlag<string> test_database( test_command, "database", "", {'d', "database"} );


		args::Group arguments( arg_parser, "arguments", args::Group::Validators::DontCare, args::Options::Global );
		args::HelpFlag help_arguments( arguments, "help", "help", {'h', "help"} );


		this->environment_prefix = this->getEnvironmentVariable( "ENVIRONMENT_PREFIX" );

		this->broker_list = this->getEnvironmentVariable( "BROKER_LIST", "127.0.0.1:9092" );
		this->mongo_connection = this->getEnvironmentVariable( "MONGO_CONNECTION" );

		this->mysql_host = this->getEnvironmentVariable( "MYSQL_HOST" );
		this->mysql_port = this->getEnvironmentVariable( "MYSQL_PORT" );
		this->mysql_username = this->getEnvironmentVariable( "MYSQL_USERNAME" );
		this->mysql_password = this->getEnvironmentVariable( "MYSQL_PASSWORD" );
		this->mysql_database = this->getEnvironmentVariable( "MYSQL_DATABASE" );

		this->postgres_connection = this->getEnvironmentVariable( "POSTGRES_CONNECTION" );


		try{

			arg_parser.ParseCLI(argc, argv);

			if( tail_command ){
				this->command = "tail";
				this->tail_source = args::get( tail_source );
				this->tail_database = args::get( tail_database );
			}

			if( consume_command ){
				this->command = "consume";
				this->consume_topic = args::get( consume_topic );
				this->consume_consumer_group = args::get( consume_consumer_group );
				this->consume_type = args::get( consume_type );
			}

			if( walk_command ){
				this->command = "walk";
				this->walk_source = args::get( walk_source );
				this->walk_object = args::get( walk_object );
			}

			if( mutate_command ){
				this->command = "mutate";
				this->mutate_chain = args::get( mutate_chain );
				this->mutate_version = args::get( mutate_version );
			}

			if( test_command ){
				this->command = "test";
				this->test_name = args::get( test_name );
				this->test_database = args::get( test_database );
			}


		}catch( args::Help ){

			std::cout << arg_parser;

		}catch( args::Error& e ){

			std::cerr << e.what() << std::endl << arg_parser;			

		}

	}



	bool Mutable::run(){

		
		json running_dialogue{
			{ "hey", "there" },
			{ "how", "are" },
			{ "you", "!" }
		};


		if( this->command == "mutate" ){

			if( this->mutate_version < 1 ){
				cerr << "Mutate version must be positive." << endl;
				return false;
			}

			json mutation_delcaration{
				{ "operation", "mutate" },
				{ "environment_prefix", this->environment_prefix },
				{ "chain", this->mutate_chain },
				{ "version", this->mutate_version }
			};

			this->publishKafkaMessage( "mutable_control", mutation_delcaration );

		}



		if( this->command == "walk" ){

			json walk_command{
				{ "operation", "walk" },
				{ "environment_prefix", this->environment_prefix },
				{ "source", this->walk_source },
				{ "object", this->walk_object }
			};

			this->publishKafkaMessage( "mutable_control", walk_command );

		}



		if( this->command == "tail" ){

			if( this->tail_source == "mongo" ){

				if( this->tail_database.size() == 0 ){
					cerr << "Tail database must be set." << endl;
					return false;
				}

				MongoTailer tailer( this->mongo_connection, this->broker_list, this->tail_database );
				tailer.tailDatabase();

			}else{

				cerr << "Unknown tail source. Supported sources \'mongo'." << endl;
				return false;

			}

		}



		if( this->command == "test" ){

			if( this->test_name == "mongo_insert" ){

				MongoClient mongo_client( this->mongo_connection );
				mongo_client.test();

			}else if( this->test_name == "mongo_list_databases" ){

				MongoClient mongo_client( this->mongo_connection );
				mongo_client.getDatabases();

			}else if( this->test_name == "mongo_list_collections" ){

				MongoClient mongo_client( this->mongo_connection );
				for( auto& collection_name : mongo_client.getCollections(this->test_database) ){
					cout << collection_name << endl;
				}

			}else if( this->test_name == "mongo_list_all_collections" ){

				MongoClient mongo_client( this->mongo_connection );

				vector<string> databases = mongo_client.getDatabases();

				for( auto& database_name : databases ){
					cout << database_name << endl;

					for( auto& collection_name : mongo_client.getCollections(database_name) ){
						cout << "    " << collection_name << endl;
					}

					cout << endl;

				}


			}else{

				cerr << "Unknown test name. Supported tests: mongo_insert, mongo_list_databases, mongo_list_collections, mongo_list_all_collections." << endl;
				return false;

			}

		}





		/*


		MongoClient mongo_client( this->mongo_connection );

		mongo_client.test();
		mongo_client.listDatabases();

		*/


		/*
		MysqlClient mysql_client( 
			this->mysql_host
			std::stol(this->mysql_port),
			this->mysql_username,
			this->mysql_password,
			this->mysql_database
		);

		mysql_client.executeStatement( R"MTBL_STATEMENT(
			CREATE TABLE pet (
				name VARCHAR(20), 
				owner VARCHAR(20), 
				species VARCHAR(20), 
				gender CHAR(1)
			);
		)MTBL_STATEMENT" );
		*/



		/*
		PostgresClient postgres_client( postgres_connection );
		

		MutableChain mutable_chain( postgres_client );

		mutable_chain.runMutations( 0, 1 );

		*/
		
		//cout << running_dialogue.dump(4) << endl;
		

		return true;

	}




	void Mutable::loadEnvironmentVariables(){

		int i = 0;
		while( environ[i] ){

			string environment_line( environ[i] ); // in the form of "variable=value"
			i++;

			std::string::size_type n = environment_line.find('=');

			if( n == std::string::npos ){
				//not found
				throw std::runtime_error("Unexpected environment format.");
			} else {				
				string variable_name = environment_line.substr(0, n);
				string variable_value = environment_line.substr(n + 1);
				this->environment_variables.insert( std::pair<string,string>(variable_name, variable_value) );
			}

		}

	}



	string Mutable::getEnvironmentVariable( const string& variable_name, const string& default_value ) const{

		if( this->environment_variables.count(variable_name) != 0 ){
			return this->environment_variables.at(variable_name);
		}

		return default_value;

	}



	void Mutable::setEnvironmentVariable( const string& variable_name, const string& variable_value ){

		this->environment_variables.insert( std::pair<string,string>(variable_name,variable_value) );

	}



	void Mutable::publishKafkaMessage( const string topic, const json& message ) const{

		cppkafka::Configuration config = {
			{ "metadata.broker.list", this->broker_list }
		};

		cppkafka::Producer producer(config);

		const string message_contents = message.dump();
		producer.produce( cppkafka::MessageBuilder(topic).partition(-1).payload(message_contents) );
		producer.flush();

	}



}