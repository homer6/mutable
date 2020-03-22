#pragma once

#include <string>
using std::string;


namespace mtbl{
namespace chains{

	class Chain{

		public:
			virtual bool runMutation( const int64_t target_state = -1, const bool mutate_forward = true ) = 0;
			virtual bool runMutations( const int64_t actual_state = -1, const int64_t desired_state = -1 );


	};


}
}