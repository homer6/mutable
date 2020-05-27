#include "consumers/MutatorConsumer.h"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <cppkafka/cppkafka.h>
using cppkafka::Message;
using cppkafka::TopicPartitionList;

#include <stdexcept>

#include "Mutable.h"

#include "chains/mutable/MutableChain.h"
using mtbl::chains::mtbl::MutableChain;



namespace mtbl::consumers{

	MutatorConsumer::MutatorConsumer( unique_ptr<KafkaConsumer> kafka_consumer, const string& topic, mtbl::Mutable* mutable_app )
		:kafka_consumer(std::move(kafka_consumer)), topic(topic), mutable_app(mutable_app)
	{



	}


	void MutatorConsumer::consume(){

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
	                //if (message.get_key()) {
	                //    cout << message.get_key() << " -> ";
	                //}

	                // Print the payload
	                const string message_payload = message.get_payload();

	                // Now commit the message
	                consumer.commit(message);

	                try{

	                	json message_object = json::parse(message_payload);

		                try{
		                	this->mutate( message_object );
		                }catch( std::runtime_error &e ){
		                	cerr << "Failed to mutate." << endl;
		                }

	                }catch( std::exception &e ){
	                	cerr << "Failed to parse json message." << endl;
	                }

	            }

	        }

	    }

	}




	void MutatorConsumer::mutate( const json& mutate_message ){


		if( mutate_message.count("operation") && mutate_message["operation"] == "mutate" ){

		}else{
			//fail silently
			return;
		}


		string environment_prefix;
		string chain;
		uint64_t version = -1;


		if( mutate_message.count("environment_prefix") ){
			environment_prefix = mutate_message["environment_prefix"].get<string>();
		}else{
			//fail silently (required parameter)
			return;
		}

		if( mutate_message.count("chain") ){
			chain = mutate_message["chain"].get<string>();
		}else{
			//fail silently (required parameter)
			return;
		}

		if( mutate_message.count("version") ){
			version = mutate_message["version"].get<uint64_t>();
		}else{
			//fail silently (required parameter)
			return;
		}


		auto postgres_client = this->mutable_app->createPostgresClient();
		MutableChain mutable_chain( *postgres_client );
		int64_t current_state = mutable_chain.getChainCurrentState( chain );


		if( chain == "mutable" ){
			mutable_chain.runMutations( current_state, version );
		}

	}


}



