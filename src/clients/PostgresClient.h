#pragma once

#include <string>
using std::string;

#include <pqxx/pqxx>


namespace mtbl{
namespace client{



	class PostgresClient{

		public:
			PostgresClient( const string connection_string );

			pqxx::connection postgres_connection;

	};




}
}