#pragma once

#include <string>
using std::string;

#include <memory>

#include <vector>
using std::vector;


#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.hpp>

#include "json.hpp"
using json = nlohmann::json;


#include "utils/Url.h"
using homer6::Url;


namespace mtbl::client{


	class ElasticSearchClient{

		public:

			struct Config{
				Config( const string& connection_string )
					:url(connection_string)
				{
				}

				homer6::Url url;
			};

			ElasticSearchClient( Config config );

			bool sendRecords( const string& index_name, const vector<json>& records ) const;
			bool createIndex( const string& index_name, const json& index_definition ) const;

			bool sendPost( const string& path, const httplib::Headers& request_headers, const string& message_body = "", const string& content_type = "application/json" ) const;
			bool sendPut( const string& path, const httplib::Headers& request_headers, const string& message_body = "", const string& content_type = "application/json" ) const;



		protected:
			httplib::Headers getRequestHeaders() const;
			bool handleResponse( std::shared_ptr<httplib::Response> es_response ) const;

			string hostname;
			string username;
			string password;			
			bool secure = true;
			int port = 9200;
			
			std::unique_ptr<httplib::Client> http_client;
			

	};


}