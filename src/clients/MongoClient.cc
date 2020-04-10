#include "clients/MongoClient.h"

#include <vector>
using std::vector;

#include <iostream>
using std::cout;
using std::endl;

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>


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


}
}