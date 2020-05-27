#pragma once

#include "chains/Chain.h"

#include "clients/PostgresClient.h"

#include <string>
using std::string;


namespace mtbl{
namespace chains{
namespace mtbl{


	class MutableChain : public Chain{

		public:
			MutableChain( ::mtbl::client::PostgresClient &postgres_client );

			virtual bool runMutation( const int64_t target_state = -1, const bool mutate_forward = true );


			void createChain( const string& chain_name );
			void removeChain( const string& chain_name );

			int64_t getChainCurrentState( const string& chain_name );
			void updateChainState( const string& chain_name, int64_t new_state );

		protected:
			::mtbl::client::PostgresClient &postgres_client;


	};


}
}
}