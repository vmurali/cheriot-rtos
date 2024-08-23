#pragma once
#include <compartment.h>
#include <token.h>

using namespace CHERI;
using namespace std;

struct UserKeyT {
  int16_t value;
};

bool __cheri_compartment("service") addOrUpdate(Sealed<UserKeyT> sealedUserKey, int16_t key, int32_t val);

optional<int32_t> __cheri_compartment("service") read(Sealed<UserKeyT> sealedUserKey, int16_t key);

void __cheri_compartment("service") erase(Sealed<UserKeyT> sealedUserKey, int16_t key);
