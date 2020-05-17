#include "clients/PostgresClient.h"


namespace mtbl::client{

	PostgresClient::PostgresClient( const string connection_string )
		:postgres_connection( connection_string )
	{



	}

}