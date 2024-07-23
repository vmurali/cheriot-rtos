#include "service.h"
#include <debug.hh>

DECLARE_AND_DEFINE_STATIC_SEALED_VALUE(UserKeyT, service, SealingType, userKey, 1);

using Debug = ConditionalDebug<true, "Caller1">;

void __cheri_compartment("caller1") entry() {
  auto sealedUserKey = STATIC_SEALED_VALUE(userKey);
  addOrUpdate(sealedUserKey, 1, 12);
  addOrUpdate(sealedUserKey, 2, 9);
  addOrUpdate(sealedUserKey, 3, 85);
  addOrUpdate(sealedUserKey, 4, 22);
  addOrUpdate(sealedUserKey, 5, 6);
  addOrUpdate(sealedUserKey, 6, 87);
  std::optional<int32_t> ret = true;
  ret = read(sealedUserKey, 1);
  Debug::log("1 -> {}", ret.has_value() && *ret == 12);
  ret = read(sealedUserKey, 2);
  Debug::log("2 -> {}", ret.has_value() && *ret == 9);
  ret = read(sealedUserKey, 3);
  Debug::log("3 -> {}", ret.has_value() && *ret == 85);
  ret = read(sealedUserKey, 4);
  Debug::log("4 -> {}", ret.has_value() && *ret == 22);
  ret = read(sealedUserKey, 5);
  Debug::log("5 -> {}", ret.has_value() && *ret == 6);
  ret = read(sealedUserKey, 6);
  Debug::log("6 -> {}", ret.has_value() && *ret == 87);
}
