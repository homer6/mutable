#include "mutators/Mutator.h"


namespace mtbl{
namespace mutator{

	bool Mutator::beforeMutateForward(){ return true; }
	bool Mutator::mutateForward(){ return true; }
	bool Mutator::afterMutateForward(){ return true; }

	bool Mutator::beforeMutateBackward(){ return true; }
	bool Mutator::mutateBackward(){ return true; }
	bool Mutator::afterMutateBackward(){ return true; }

	bool Mutator::onErrorMutateForward(){ return true; }
	bool Mutator::onSuccessMutateForward(){ return true; }

	bool Mutator::onErrorMutateBackward(){ return true; }
	bool Mutator::onSuccessMutateBackward(){ return true; }

	bool Mutator::onError(){ return true; }
	bool Mutator::onSuccess(){ return true; }

}
}