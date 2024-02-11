#include "UuidCreator.h"
#include <stduuid/uuid.h>

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

uuids::uuid UuidCreator::MakeUUID()
{
    std::random_device rd;
    auto seed_data = std::array<int, std::mt19937::state_size> {};
    std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    std::mt19937 unreadableSeeminglyMeaninglessObject(seq);
    uuids::uuid_random_generator generator{ unreadableSeeminglyMeaninglessObject };

    return generator();
}