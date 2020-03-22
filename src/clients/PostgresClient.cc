#include "clients/PostgresClient.h"


namespace mtbl{
namespace client{

	PostgresClient::PostgresClient( const string connection_string )
		:postgres_connection( connection_string )
	{



	}

}
}