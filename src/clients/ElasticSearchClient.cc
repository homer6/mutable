#include "clients/ElasticSearchClient.h"

#include "utils/Common.h"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <stdexcept>


namespace mtbl::client{

	ElasticSearchClient::ElasticSearchClient( const string& connection_string, const string& username, const string& password, bool secure, int port )
		:connection_string(connection_string), username(username), password(password), secure(secure), port(port)
	{

		using namespace mtbl::utils::common;

		// connect to elasticsearch
			vector<string> hostname_parts = mtbl::utils::common::split_string( this->connection_string, ':' );
			if( hostname_parts.size() == 2 ){
				this->hostname = hostname_parts[0];
				this->port = std::stoi( hostname_parts[1] );
			}else if( hostname_parts.size() == 1 ){
				this->hostname = hostname_parts[0];
			}else{
				throw std::runtime_error( "Unexpected elasticsearch target hostname: " + this->connection_string );
			}

		if( this->secure ){
			this->http_client.reset( new httplib::SSLClient( this->hostname.c_str(), this->port ) );
		}else{
			this->http_client.reset( new httplib::Client( this->hostname.c_str(), this->port ) );
		}

	}



	bool ElasticSearchClient::sendRecords( const string& index_name, const vector<json>& records ) const{

		if( records.size() == 0 ){
			return true;
		}

		const string post_path = "/" + index_name + "/_bulk";


		string request_body = "{\"index\":{\"_index\":\"" + index_name + "\",\"_type\":\"_doc\"}}\n";

    	for( const json& json_object : records ){

        	request_body += json_object.dump() + "\n";

        }

        try{

        	httplib::Headers request_headers = this->getRequestHeaders();
        	return this->sendPost( post_path, request_headers, request_body, "application/x-ndjson" );

        }catch( const std::exception& e ){

        	cerr << "mutable: failed to send records to elasticsearch: " + string(e.what()) << endl;

        }

		return false;

	}



	bool ElasticSearchClient::createIndex( const string& index_name, const json& index_definition ) const{

		if( index_name.size() == 0 || index_definition.size() == 0 ){
			return false;
		}

		const string http_path = "/" + index_name;

        try{

        	httplib::Headers request_headers = this->getRequestHeaders();
        	return this->sendPut( http_path, request_headers, index_definition.dump() );

        }catch( const std::exception& e ){

        	cerr << "mutable: failed to creat elasticsearch index: " + string(e.what()) << endl;

        }

		return false;

	}




	bool ElasticSearchClient::sendPost( const string& path, const httplib::Headers& request_headers, const string& message_body, const string& content_type ) const{

		return this->handleResponse( this->http_client->Post( path.c_str(), request_headers, message_body, content_type.c_str() ) );

	}

	bool ElasticSearchClient::sendPut( const string& path, const httplib::Headers& request_headers, const string& message_body, const string& content_type ) const{

		return this->handleResponse( this->http_client->Put( path.c_str(), request_headers, message_body, content_type.c_str() ) );

	}



	bool ElasticSearchClient::handleResponse( std::shared_ptr<httplib::Response> es_response ) const{

		if( es_response ){

			if( es_response->status >= 200 && es_response->status < 300 ){

				return true;

			}else{

				json bad_response_object = json::object();

				bad_response_object["description"] = "Elasticsearch non-200 response.";
				bad_response_object["body"] = es_response->body;
				bad_response_object["status"] = es_response->status;
				bad_response_object["headers"] = json::object();

				for( auto &header : es_response->headers ){
					bad_response_object["headers"][header.first] = header.second;
				}

				const string error_message = bad_response_object.dump();

				cerr << error_message << endl;

				throw std::runtime_error( error_message );

			}

		}else{

			cerr << "mutable: No response object." << endl;
			throw std::runtime_error( "Elasticsearch client: No response object." );

		}

		return false;

	}




	httplib::Headers ElasticSearchClient::getRequestHeaders() const{

		httplib::Headers request_headers{
			{ "Host", this->hostname },
			{ "User-Agent", "mutable" }
		};

		if( this->username.size() ){
			const string basic_auth_credentials = mtbl::utils::common::encodeBase64( this->username + ":" + this->password );
			request_headers.insert( { "Authorization", "Basic " + basic_auth_credentials } );
		}

		return request_headers;

	}


}










			


