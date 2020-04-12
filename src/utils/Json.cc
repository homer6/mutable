#include "utils/Json.h"


#include <map>
using std::map;


namespace mtbl{
namespace utils{


	json file_get_json( const string& filename ){

		return json::parse( file_get_contents(filename) );

	}


	void file_write_json( const string& filename, const json& json, size_t indent ){

		File output_file( filename );
		output_file.write( json.dump(indent) );

	}



	json sort_json_array( const json& input_json_array, const string sort_field ){

		json array_sorted = json::array();

		map<string,json> json_map;

		for( const json& object : input_json_array ){

			if( object.count(sort_field) ){

				const string key = object[sort_field].get<string>();

				json_map[ key ] = object;

			}else{

				throw std::runtime_error( "Key not found in record: " + sort_field );

			}

		}


		for( auto& object_pair : json_map ){

			array_sorted.push_back( object_pair.second );

		}


		return array_sorted;

	}


}
}