#pragma once

#include <string>
using std::string;

#include "mysql+++/mysql+++.h"


namespace mtbl{
namespace client{

	class MysqlClient{

		public:
			// timeout: The connect timeout in seconds.
			MysqlClient( const string& hostname, unsigned int port = 0, const string& username = "", const string& password = "", const string& dbname = "", unsigned int timeout = 5, bool auto_reconnect = true );


			void executeStatement( const string& statement );


		protected:
			daotk::mysql::connection connection;

	};




}
}