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


namespace mtbl{
namespace client{



	class ElasticSearchClient{

		public:
			ElasticSearchClient( const string& connection_string, const string& username = "", const string& password = "", bool secure = true, int port = 9200 );

			bool sendRecords( const string& index_name, const vector<json>& records ) const;
			bool createIndex( const string& index_name, const json& index_definition ) const;

			bool sendPost( const string& path, const httplib::Headers& request_headers, const string& message_body = "", const string& content_type = "application/json" ) const;
			bool sendPut( const string& path, const httplib::Headers& request_headers, const string& message_body = "", const string& content_type = "application/json" ) const;



		protected:
			httplib::Headers getRequestHeaders() const;
			bool handleResponse( std::shared_ptr<httplib::Response> es_response ) const;


			string connection_string;
			string username;
			string password;			
			bool secure = true;
			int port = 9200;

			string hostname;
			std::unique_ptr<httplib::Client> http_client;
			

	};




}
}