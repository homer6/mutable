#pragma once

#include <string>
using std::string;

#include "json.hpp"
using json = nlohmann::json;

#include "utils/File.h"

namespace mtbl::utils{

	json file_get_json( const string& filename );
	void file_write_json( const string& filename, const json& json, size_t indent = 4 );

	json sort_json_array( const json& input_json_array, const string sort_field );

}