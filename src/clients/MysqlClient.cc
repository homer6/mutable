#include "clients/MysqlClient.h"

#include <stdexcept>


namespace mtbl::client{


	MysqlClient::MysqlClient( Config config )
		:connection( daotk::mysql::connect_options{ config.url.getHost(), config.url.getUsername(), config.url.getPassword(), config.getDatabaseName(), config.timeout, config.auto_reconnect, "", "", config.url.getPort() } )
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