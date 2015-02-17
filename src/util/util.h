#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

class Util
{
public:
	static std::string& trim(std::string& str)
	{
		std::string::size_type pos = str.find_first_not_of(" \r\n\t");
		if (pos != 0)
			str.erase(0, pos);

		pos = str.find_last_not_of(" \r\n\t");
		if (pos != str.size() - 1)
			str.erase(pos + 1, str.size() - 1 - pos);

		return str;
	}

	template <typename T>
	static bool inArray(T value, std::vector<T> arr)
	{
		for (auto& x : arr) {
			if (value == x) {
				return true;
			}
		}

		return false;
	}
};

#endif