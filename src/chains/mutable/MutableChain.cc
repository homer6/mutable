#include "chains/mutable/MutableChain.h"

#include <pqxx/pqxx>

#include <stdexcept>

namespace mtbl{
namespace chains{
namespace mtbl{

	MutableChain::MutableChain( ::mtbl::client::PostgresClient &postgres_client )
		:postgres_client( postgres_client )
	{



	}


	bool MutableChain::runMutation( const int64_t target_state, const bool mutate_forward ){

		pqxx::work transaction( postgres_client.postgres_connection );


		if( target_state == 1 ){

			if( mutate_forward ){

				
				transaction.exec( R"MTBL_STATEMENT( 

					CREATE TABLE mutable_chains(
						id SERIAL PRIMARY KEY,
						name VARCHAR(50) UNIQUE NOT NULL,
						current_state INTEGER NOT NULL,
						created_at TIMESTAMP NOT NULL,
						updated_at TIMESTAMP NOT NULL
					);

				)MTBL_STATEMENT" );


				transaction.exec( R"MTBL_STATEMENT( 

					INSERT INTO mutable_chains ( name, current_state, created_at, updated_at ) VALUES ( 'mutable', 1, NOW(), NOW() );

				)MTBL_STATEMENT" );


				transaction.commit();


			}else{

				//mutate backward

				transaction.exec( R"MTBL_STATEMENT( 

					DROP TABLE mutable_chains;

				)MTBL_STATEMENT" );


				transaction.commit();


			}

		}


		return true;

	}


}
}
}