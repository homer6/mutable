#pragma once

#include <string>
using std::string;

#include <sw/redis++/redis++.h>


namespace mtbl::client{


	class RedisClient{

		public:
			RedisClient( const string connection_string );

			void set( const string& key, const string& value );
			string get( const string& key );


		protected:
			sw::redis::Redis redis_connection;

	};


}