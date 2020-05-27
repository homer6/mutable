#include "chains/mutable/MutableChain.h"

#include <pqxx/pqxx>

#include <stdexcept>

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

namespace mtbl{
namespace chains{
namespace mtbl{

	MutableChain::MutableChain( ::mtbl::client::PostgresClient &postgres_client )
		:postgres_client( postgres_client )
	{
		this->chain_name = "mutable";


	}


	bool MutableChain::runMutation( const int64_t target_state, const bool mutate_forward ){

		pqxx::work transaction( this->postgres_client.postgres_connection );


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

				transaction.commit();

				this->createChain( this->chain_name );
				this->updateChainState( this->chain_name, target_state );


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



	void MutableChain::createChain( const string& chain_name ){

		try{
			pqxx::work transaction( this->postgres_client.postgres_connection );
			transaction.exec0( "INSERT INTO mutable_chains ( name, current_state, created_at, updated_at ) VALUES ( " + transaction.quote(chain_name) + ", 0, NOW(), NOW() )" );
		}catch( std::runtime_error& e ){
			cerr << e.what() << endl;
		}

	}


	void MutableChain::removeChain( const string& chain_name ){

		try{
			pqxx::work transaction( this->postgres_client.postgres_connection );
			transaction.exec0( "DELETE FROM mutable_chains WHERE name = " + transaction.quote(chain_name) );
		}catch( std::runtime_error& e ){
			cerr << e.what() << endl;
		}

	}


	int64_t MutableChain::getChainCurrentState( const string& chain_name ){

		int64_t current_state = -1;

		try{
			pqxx::work transaction( this->postgres_client.postgres_connection );
			pqxx::row current_row = transaction.exec1( "SELECT current_state FROM mutable_chains WHERE name = " + transaction.quote(chain_name) );
			current_state = current_row[0].as<int64_t>();
		}catch( std::runtime_error& e ){
			cerr << e.what() << endl;
		}

		return current_state;

	}


	void MutableChain::updateChainState( const string& chain_name, int64_t new_state ){

		try{
			pqxx::work transaction( this->postgres_client.postgres_connection );
			transaction.exec0( "UPDATE mutable_chains SET current_state = " + transaction.quote(new_state) + " WHERE name = " + transaction.quote(chain_name) );
		}catch( std::runtime_error& e ){
			cerr << e.what() << endl;
		}

	}


}
}
}