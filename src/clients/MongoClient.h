#pragma once

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <memory>
using std::unique_ptr;

#include <mongocxx/client.hpp>
#include <mongocxx/client_session.hpp>



namespace mtbl{
namespace client{

	/*
	Warning: mongocxx::instance mongo_instance{}; must be created exactly once, in your program, before this class can be used.	

	Class representing a client connection to MongoDB.

	Not copyable. 
	Is moveable. 
	Not thread-safe.

	Databases that are created via this client inherit the read_concern, read_preference, and 
	write_concern settings of this client when they are created. The lifetimes of objects 
	created via a client object (databases, collections, cursors, etc...) must be a subset 
	of the lifetime of the client that created them.

	Note that client is not thread-safe. 
	See https://mongodb.github.io/mongo-cxx-driver/mongocxx-v3/thread-safety/ for more details.


	API Docs: http://mongocxx.org/api/mongocxx-3.4.1/namespaces.html

	*/

	class MongoClient{

		public:
			MongoClient( const string connection_string );

			void test();
			vector<string> getDatabases();
			vector<string> getCollections( const string& database_name );


			mongocxx::client mongo_connection;
			unique_ptr<mongocxx::client_session> client_session;

	};




}
}