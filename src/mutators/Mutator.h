#pragma once

namespace mtbl{

namespace mutator{
	

	/*
	* A mutator object performs a single mutation in a mutation chain.
	*
	*/
	class Mutator{

		virtual bool beforeMutateForward();
		virtual bool mutateForward();
		virtual bool afterMutateForward();

		virtual bool beforeMutateBackward();
		virtual bool mutateBackward();
		virtual bool afterMutateBackward();

		virtual bool onErrorMutateForward();
		virtual bool onSuccessMutateForward();

		virtual bool onErrorMutateBackward();
		virtual bool onSuccessMutateBackward();		

		virtual bool onError();
		virtual bool onSuccess();

	};

}
}
