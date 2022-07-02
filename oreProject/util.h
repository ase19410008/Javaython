#pragma once

#include <cstdlib>
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <list>
#include <memory>
using namespace std;

namespace ore {
#define ORE_MAX_TOKEN_LEN 255
	inline string clampToken(const char* ext) {
		string str(ext);
		if (ORE_MAX_TOKEN_LEN < str.size()) {
			str.erase(ORE_MAX_TOKEN_LEN);
		}
		return str;
	}
}