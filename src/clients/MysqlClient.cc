#include "clients/MysqlClient.h"

#include <stdexcept>


namespace mtbl{
namespace client{

	MysqlClient::MysqlClient( const string& hostname, unsigned int port, const string& username, const string& password, const string& dbname, unsigned int timeout, bool auto_reconnect )
		:connection( daotk::mysql::connect_options{ hostname, username, password, dbname, timeout, auto_reconnect, "", "", port } )
	{

		if( !this->connection ){
			throw std::runtime_error("Could not connect to mysql.");
		}

		// connect_options
		// https://github.com/daotrungkien/mysql-modern-cpp/blob/master/mysql%2B%2B%2B/mysql%2B%2B%2B.h#L725

	}



	void MysqlClient::executeStatement( const string& statement ){

		this->connection.exec( statement );

	}




}
}