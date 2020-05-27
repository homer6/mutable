#pragma once

#include <string>
using std::string;

#include <memory>
using std::unique_ptr;

#include "clients/KafkaConsumer.h"
using ::mtbl::client::KafkaConsumer;

#include "json.hpp"
using json = nlohmann::json;

namespace mtbl{
	class Mutable;
}


namespace mtbl::consumers{



	class MutatorConsumer{

		public:
			MutatorConsumer( unique_ptr<KafkaConsumer> kafka_consumer, const string& topic, mtbl::Mutable* mutable_app );

			void consume();

			void mutate( const json& mutate_message );


		protected:
			unique_ptr<KafkaConsumer> kafka_consumer;
			const string topic;
			mtbl::Mutable* mutable_app = nullptr;


	};


}
