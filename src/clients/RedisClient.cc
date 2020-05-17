#include "clients/RedisClient.h"


namespace mtbl::client{

	RedisClient::RedisClient( const string connection_string )
		:redis_connection( connection_string )
	{



	}



	void RedisClient::set( const string& key, const string& value ){

		this->redis_connection.set( key, value );

	}



	string RedisClient::get( const string& key ){

		string returned_value;

		auto value = this->redis_connection.get( key );

		if( value ){		
			returned_value = *value;
		}

		return returned_value;

	}

}