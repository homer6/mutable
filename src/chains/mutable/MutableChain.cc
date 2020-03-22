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

					CREATE TABLE chains(
						id SERIAL PRIMARY KEY,
						name VARCHAR(50) UNIQUE NOT NULL,
						description VARCHAR(50) NULL,
						admin_email VARCHAR(355) NOT NULL,
						created_at TIMESTAMP NOT NULL,
						updated_at TIMESTAMP
					);

				)MTBL_STATEMENT" );


				transaction.exec( R"MTBL_STATEMENT( 

					CREATE TABLE links(
						id SERIAL PRIMARY KEY,
						chain_id INTEGER REFERENCES chains(id),
						link_ordinal INTEGER NOT NULL,
						up_filename TEXT NULL,
						up_file_hash TEXT NULL,
						down_filename TEXT NULL,
						down_file_hash TEXT NULL,
						created_at TIMESTAMP NOT NULL,
						updated_at TIMESTAMP,

						CONSTRAINT chain_id_fkey FOREIGN KEY (chain_id)
							REFERENCES chains(id) MATCH SIMPLE
							ON UPDATE CASCADE
							ON DELETE CASCADE
					);

				)MTBL_STATEMENT" );



				transaction.exec( R"MTBL_STATEMENT( 

					INSERT INTO chains ( name, description, admin_email, created_at ) VALUES ( 'mutable', 'Mutable''s internal chain.', 'YOUR_EMAIL_HERE', NOW() );

				)MTBL_STATEMENT" );


				transaction.commit();


			}else{

				//mutate backward

				transaction.exec( R"MTBL_STATEMENT( 

					DROP TABLE chains;

				)MTBL_STATEMENT" );


				transaction.exec( R"MTBL_STATEMENT( 

					CREATE TABLE links;

				)MTBL_STATEMENT" );


				transaction.commit();


			}

		}


		return true;

	}


}
}
}