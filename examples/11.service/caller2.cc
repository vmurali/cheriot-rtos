#include "service.h"
#include <debug.hh>

DECLARE_AND_DEFINE_STATIC_SEALED_VALUE(UserKeyT, service, SealingType, userKey, 2);

using Debug = ConditionalDebug<true, "Caller2">;

void __cheri_compartment("caller2") entry() {
  auto sealedUserKey = STATIC_SEALED_VALUE(userKey);
  addOrUpdate(sealedUserKey, 1, 11);
  addOrUpdate(sealedUserKey, 2, 8);
  addOrUpdate(sealedUserKey, 3, 84);
  addOrUpdate(sealedUserKey, 4, 21);
  addOrUpdate(sealedUserKey, 5, 5);
  addOrUpdate(sealedUserKey, 6, 86);
  std::optional<int32_t> ret;
  ret = read(sealedUserKey, 1);
  Debug::log("1 -> {}", ret.has_value() && *ret == 11);
  ret = read(sealedUserKey, 2);
  Debug::log("2 -> {}", ret.has_value() && *ret == 8);
  ret = read(sealedUserKey, 3);
  Debug::log("3 -> {}", ret.has_value() && *ret == 84);
  ret = read(sealedUserKey, 4);
  Debug::log("4 -> {}", ret.has_value() && *ret == 21);
  ret = read(sealedUserKey, 5);
  Debug::log("5 -> {}", ret.has_value() && *ret == 5);
  ret = read(sealedUserKey, 6);
  Debug::log("6 -> {}", ret.has_value() && *ret == 86);
}
