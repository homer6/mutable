#pragma once

#include <string>
using std::string;

#include <memory>
using std::unique_ptr;

#include "clients/KafkaConsumer.h"
using ::mtbl::client::KafkaConsumer;

namespace mtbl::consumers{


	class EchoConsumer{

		public:
			EchoConsumer( unique_ptr<KafkaConsumer> kafka_consumer, const string& topic );

			void consume();


		protected:
			unique_ptr<KafkaConsumer> kafka_consumer;
			const string topic;

	};


}
