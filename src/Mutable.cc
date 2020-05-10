#include "Mutable.h"

#include "json.hpp"
using json = nlohmann::json;

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.hpp"

#include <iostream>
using std::cout;
using std::endl;

#include <stdexcept>

extern char **environ;

#include "clients/PostgresClient.h"
using ::mtbl::client::PostgresClient;

#include "clients/MongoClient.h"
using ::mtbl::client::MongoClient;

#include "clients/MysqlClient.h"
using ::mtbl::client::MysqlClient;

#include "chains/mutable/MutableChain.h"
using mtbl::chains::mtbl::MutableChain;


namespace mtbl{

	Mutable::Mutable( int, char** ){

		this->loadEnvironmentVariables();

	}



	bool Mutable::run(){

		/*

		MongoClient mongo_client( this->getEnvironmentVariable("MONGO_CONNECTION") );

		mongo_client.test();
		mongo_client.listDatabases();

		*/


		/*
		MysqlClient mysql_client( 
			this->getEnvironmentVariable("MYSQL_HOST"),
			std::stol( this->getEnvironmentVariable("MYSQL_PORT", "3306") ),
			this->getEnvironmentVariable("MYSQL_USERNAME"),
			this->getEnvironmentVariable("MYSQL_PASSWORD"),
			this->getEnvironmentVariable("MYSQL_DATABASE")
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
		PostgresClient postgres_client( this->getEnvironmentVariable("POSTGRES_CONNECTION") );
		

		MutableChain mutable_chain( postgres_client );

		mutable_chain.runMutations( 0, 1 );

		*/
		
		json running_dialogue{
			{ "hey", "there" },
			{ "how", "are" },
			{ "you", "!" }
		};

		cout << running_dialogue.dump(4) << endl;
		

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



}