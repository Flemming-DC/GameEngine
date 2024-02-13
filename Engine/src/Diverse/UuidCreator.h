#pragma once
#include <string>
#include <stduuid/uuid.h>


class UuidCreator
{
public:
	//static void Setup();

	static uuids::uuid MakeID(); //{ return (*generator)(); }
	static uuids::uuid from_string(std::string str) { return uuids::uuid::from_string(str).value(); }
	static std::string to_string(uuids::uuid id) { return uuids::to_string(id); }


private:
	//static uuids::uuid_random_generator* generator;
};

