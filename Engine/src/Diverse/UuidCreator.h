#pragma once
#include <string>
#include <stduuid/uuid.h>


class UuidCreator
{
public:
	static uuids::uuid MakeID();
	static uuids::uuid from_string(std::string str) { return uuids::uuid::from_string(str).value(); }
	static bool IsInitialized(uuids::uuid id) { return id != unInitializedID; }

private:
	static uuids::uuid unInitializedID;
	
};

namespace logger
{
	inline std::string to_string(uuids::uuid id) { return uuids::to_string(id); }
}

