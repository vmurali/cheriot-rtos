#include "service.h"
#include <compartment.h>
#include <token.h>
#include <locks.hh>
#include <debug.hh>

using Debug = ConditionalDebug<true, "Service">;

using namespace CHERI;
using namespace std;

constexpr int SIZE = 16;

typedef int32_t KeyT;

optional<pair<KeyT, int32_t>> entries[SIZE] = {};
FlagLockPriorityInherited lock;

KeyT getFullKey(Sealed<UserKeyT> sealedUserKey, int16_t key) {
  SKey staticKey = STATIC_SEALING_TYPE(SealingType);
  int16_t userKey = token_unseal(staticKey, sealedUserKey)->value;
  return (userKey << 16 | key);
}

bool __cheri_compartment("service") addOrUpdate(Sealed<UserKeyT> sealedUserKey, int16_t key, int32_t val) {
  KeyT fullKey = getFullKey(sealedUserKey, key);
  optional<int> emptyIdx = nullopt;
  LockGuard lockGuard(lock);
  for (int i = 0; i < SIZE; i++) {
    if(entries[i]) {
      Debug::log("may update: {}: {} {} but {} {}", i, fullKey, val, entries[i]->first, entries[i]->second);
      if(entries[i]->first == fullKey) {
        entries[i]->second = val;
        return true;
      }
    } else {
      Debug::log("addOrUpdate empty slot: {}: {} {}", i, fullKey, val);
      emptyIdx = i;
    }
  }
  if(emptyIdx) {
    entries[*emptyIdx] = {fullKey, val};
    Debug::log("add: {}: {} {}", *emptyIdx, fullKey, val);
    return true;
  }
  return false;
}

optional<int32_t> __cheri_compartment("service") read(Sealed<UserKeyT> sealedUserKey, int16_t key) {
  KeyT fullKey = getFullKey(sealedUserKey, key);
  LockGuard lockGuard(lock);
  for (int i = 0; i < SIZE; i++) {
    if(entries[i]) {
      Debug::log("read: {}: {} but {} {}", i, fullKey, entries[i]->first, entries[i]->second);
      if(entries[i]->first == fullKey) {
        return entries[i]->second;
      }
    } else {
      Debug::log("notread: {}: {}", i, fullKey);
    }
  }
  return nullopt;
}

void __cheri_compartment("service") erase(Sealed<UserKeyT> sealedUserKey, int16_t key) {
  KeyT fullKey = getFullKey(sealedUserKey, key);
  LockGuard lockGuard(lock);
  for (int i = 0; i < SIZE; i++) {
    if(entries[i]) {
      Debug::log("erase: {}: {} but {} {}", i, fullKey, entries[i]->first, entries[i]->second);
      if(entries[i]->first == fullKey) {
        entries[i] = nullopt;
      }
    } else {
      Debug::log("noterase: {}: {}", i, fullKey);
    }
  }
}
