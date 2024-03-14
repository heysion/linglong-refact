// This file is generated by tools/codegen.sh
// DO NOT EDIT IT.

// clang-format off

//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     BuilderConfig.hpp data = nlohmann::json::parse(jsonString);

#pragma once

#include <optional>
#include <nlohmann/json.hpp>
#include "linglong/api/types/v1/helper.hpp"

namespace linglong {
namespace api {
namespace types {
namespace v1 {
/**
* Configuration file for ll-builder.
*/

using nlohmann::json;

/**
* Configuration file for ll-builder.
*/
struct BuilderConfig {
std::string remoteRepoEndpoint;
std::string remoteRepoName;
int64_t version;
};
}
}
}
}

// clang-format on