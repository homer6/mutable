#pragma once

#include <string>
using std::string;

#include "mysql+++/mysql+++.h"

#include "utils/Url.h"
using homer6::Url;

namespace mtbl::client{


	class MysqlClient{

		public:

			struct Config{
				Config( const string& connection_string )
					:url(connection_string)
				{
				}

				homer6::Url url;
				unsigned int timeout = 5;  // timeout: The connect timeout in seconds.
				bool auto_reconnect = true;

				string getDatabaseName() const{
					const string path = this->url.getPath();
					if( path.size() > 0 ){
						return path.substr( 1 );
					}
					return "";
				}

			};

			MysqlClient( Config config );

		
			void executeStatement( const string& statement );


		protected:
			daotk::mysql::connection connection;

	};


}