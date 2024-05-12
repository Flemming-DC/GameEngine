#include "UuidCreator.h"
#include "Errorchecker.h"
#include <stduuid/uuid.h>

using uuids::uuid;

uuid UuidCreator::unInitializedID;// = from_string("dcb8bd08-84c2-45e5-bb8b-07a5fd5bd52b");

/*
uuids::uuid_random_generator* UuidCreator::generator = nullptr;

void UuidCreator::Setup()
{
    std::random_device rd;
    auto seed_data = std::array<int, std::mt19937::state_size> {};
    std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    std::mt19937 unreadableSeeminglyMeaninglessObject(seq);
    //uuids::uuid_random_generator gen{ unreadableSeeminglyMeaninglessObject };

    generator = new uuids::uuid_random_generator({ unreadableSeeminglyMeaninglessObject });
}
*/

uuid UuidCreator::MakeID()
{
    std::random_device rd;
    auto seed_data = std::array<int, std::mt19937::state_size> {};
    std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    std::mt19937 unreadableSeeminglyMeaninglessObject(seq);
    uuids::uuid_random_generator generator{ unreadableSeeminglyMeaninglessObject };

    return generator();
}


uuid UuidCreator::from_string(std::string str) 
{ 
    if (auto id = uuid::from_string(str))
        return id.value();
    else
        RaiseError("cannot interpret '", str, "' as a uuid.");
}