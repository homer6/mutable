#include "clients/MongoClient.h"

#include <vector>
using std::vector;

#include <iostream>
using std::cout;
using std::endl;

#include <algorithm>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include "json.hpp"
using json = nlohmann::json;


namespace mtbl{
namespace client{

	MongoClient::MongoClient( const string connection_string )
		:mongo_connection( mongocxx::uri{connection_string} )
	{

		this->client_session = std::make_unique<mongocxx::client_session>( this->mongo_connection.start_session() );

	}


	void MongoClient::test(){

	    bsoncxx::builder::stream::document document{};

	    auto collection = this->mongo_connection["testdb"]["testcollection"];
	    document << "hello" << "world";

	    collection.insert_one( document.view() );
	    auto cursor = collection.find( {} );

	    for( auto&& doc : cursor ){
	        std::cout << bsoncxx::to_json(doc) << std::endl;
	    }

	}


	vector<string> MongoClient::getDatabases(){

		auto cursor = this->mongo_connection.list_databases( *this->client_session );

		vector<string> databases;

		for( auto&& doc : cursor ){

			json document = json::parse( bsoncxx::to_json(doc) );

			databases.push_back( document["name"].get<string>() );
			//std::cout << bsoncxx::to_json(doc) << std::endl;
		}

		std::sort( databases.begin(), databases.end() );

		return databases;

	}


	vector<string> MongoClient::getCollections( const string& database_name ){

		vector<string> collection_names = this->mongo_connection[database_name].list_collection_names( *this->client_session );
		std::sort( collection_names.begin(), collection_names.end() );
		return collection_names;

	}


}
}