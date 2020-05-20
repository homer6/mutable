#include "consumers/EchoConsumer.h"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <cppkafka/cppkafka.h>
using cppkafka::Message;
using cppkafka::TopicPartitionList;


namespace mtbl::consumers{

	EchoConsumer::EchoConsumer( unique_ptr<KafkaConsumer> kafka_consumer, const string& topic )
		:kafka_consumer(std::move(kafka_consumer)), topic(topic)
	{



	}



	void EchoConsumer::consume(){

		auto& consumer = this->kafka_consumer->consumer;


		// Print the assigned partitions on assignment
		consumer.set_assignment_callback([](const TopicPartitionList& partitions) {
		    cout << "Got assigned: " << partitions << endl;
		});

		// Print the revoked partitions on revocation
		consumer.set_revocation_callback([](const TopicPartitionList& partitions) {
		    cout << "Got revoked: " << partitions << endl;
		});

		// Subscribe to the topic
		consumer.subscribe({ this->topic });


	    cout << "Consuming messages from topic " << this->topic << endl;

	    bool running = true;


	    while( running ){

	        // Try to consume a message
	        Message message = consumer.poll();

	        if( message ){

	            // If we managed to get a message

	            if( message.get_error() ){

	                // Ignore EOF notifications from rdkafka
	                if( !message.is_eof() ){
	                    cout << "[+] Received error notification: " << message.get_error() << endl;
	                }

	            }else{

	                // Print the key (if any)
	                if (message.get_key()) {
	                    cout << message.get_key() << " -> ";
	                }

	                // Print the payload
	                cout << message.get_payload() << endl;

	                // Now commit the message
	                consumer.commit(message);

	            }

	        }

	    }

	}


}



