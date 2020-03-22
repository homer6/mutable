#pragma once

#include <string>
using std::string;


namespace mtbl{

namespace mutator{
	

	/*
	* A mutator object performs a single mutation in a mutation chain.
	*
	*/
	class Mutator{

		public:

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


		protected:
			string chain_name;
			int64_t chain_ordinal = 0;


	};

}
}
