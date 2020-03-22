#include "chains/Chain.h"

#include <stdexcept>

namespace mtbl{
namespace chains{


	bool Chain::runMutations( const int64_t actual_state, const int64_t desired_state ){

		if( desired_state < 0 ){
			throw std::runtime_error( "Desired state must be non-negative." );
		}

		if( actual_state < 0 ){
			throw std::runtime_error( "Actual state must be non-negative." );
		}

		if( desired_state == actual_state ){
			//no work needed
			return true;
		}

		int64_t current_state = actual_state;

		if( actual_state < desired_state ){
			//proceed forward
			while( current_state <= desired_state ){
				current_state++;
				if( !this->runMutation( current_state, true ) ) return false;
			}
			return true;			
		}

		if( actual_state > desired_state ){
			//proceed backward
			while( current_state >= desired_state ){
				current_state--;
				if( !this->runMutation( current_state, false ) ) return false;
			}
			return true;			
		}

		return false;

	}


}
}