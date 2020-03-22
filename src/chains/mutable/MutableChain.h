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


		protected:
			::mtbl::client::PostgresClient &postgres_client;


	};


}
}
}