#include "../redismodule.h"
#include "../rmutil/util.h"
#include "../rmutil/strings.h"
#include "../rmutil/test_util.h"

static char *str(RedisModuleString *input) {
  size_t len;
  const char *s = RedisModule_StringPtrLen(input, &len);
  char *p = RedisModule_Alloc(len + 1);
  memcpy(p, s, len);
  *(p + len) = 0;
  return p;
}

/*
 * REPSHEET.BLACKLIST <address> <reason> <ttl>
 * Creates a Repsheet blacklist entry for the specified address.
 * If a ttl is provided, the key will be set to expire after <ttl>
 * seconds.
 *
 */
int BlacklistCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
  if (argc != 3 && argc != 4) {
    return RedisModule_WrongArity(ctx);
  }

  RedisModule_AutoMemory(ctx);

  RedisModuleString *keyString = RedisModule_CreateStringPrintf(ctx, "%s:repsheet:ip:blacklisted", str(argv[1]));
  RedisModuleKey *key = RedisModule_OpenKey(ctx, keyString, REDISMODULE_READ | REDISMODULE_WRITE);

  int keyType = RedisModule_KeyType(key);
  if (keyType == REDISMODULE_KEYTYPE_EMPTY || keyType == REDISMODULE_KEYTYPE_STRING) {
    RedisModule_StringSet(key, argv[2]);
    if (argc == 4) {
      long long timeout;
      RedisModule_StringToLongLong(argv[3], &timeout);
      RedisModule_SetExpire(key, timeout * 1000);
    }
  }

  RedisModule_CloseKey(key);
  RedisModule_ReplyWithSimpleString(ctx, "OK");

  return REDISMODULE_OK;
}

/*
 * REPSHEET.WHITELIST <address> <reason> <ttl>
 * Creates a Repsheet whitelist entry for the specified address.
 * If a ttl is provided, the key will be set to expire after <ttl>
 * seconds.
 *
 */
int WhitelistCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
  if (argc != 3 && argc != 4) {
    return RedisModule_WrongArity(ctx);
  }

  RedisModule_AutoMemory(ctx);

  RedisModuleString *keyString = RedisModule_CreateStringPrintf(ctx, "%s:repsheet:ip:whitelisted", str(argv[1]));
  RedisModuleKey *key = RedisModule_OpenKey(ctx, keyString, REDISMODULE_READ | REDISMODULE_WRITE);

  int keyType = RedisModule_KeyType(key);
  if (keyType == REDISMODULE_KEYTYPE_EMPTY || keyType == REDISMODULE_KEYTYPE_STRING) {
    RedisModule_StringSet(key, argv[2]);
    if (argc == 4) {
      long long timeout;
      RedisModule_StringToLongLong(argv[3], &timeout);
      RedisModule_SetExpire(key, timeout * 1000);
    }
  }

  RedisModule_CloseKey(key);
  RedisModule_ReplyWithSimpleString(ctx, "OK");

  return REDISMODULE_OK;
}

/*
 * REPSHEET.MARK <address> <reason> <ttl>
 * Creates a Repsheet marklist entry for the specified address.
 * If a ttl is provided, the key will be set to expire after <ttl>
 * seconds.
 *
 */
int MarkCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
  if (argc != 3 && argc != 4) {
    return RedisModule_WrongArity(ctx);
  }

  RedisModule_AutoMemory(ctx);

  RedisModuleString *keyString = RedisModule_CreateStringPrintf(ctx, "%s:repsheet:ip:marked", str(argv[1]));
  RedisModuleKey *key = RedisModule_OpenKey(ctx, keyString, REDISMODULE_READ | REDISMODULE_WRITE);

  int keyType = RedisModule_KeyType(key);
  if (keyType == REDISMODULE_KEYTYPE_EMPTY || keyType == REDISMODULE_KEYTYPE_STRING) {
    RedisModule_StringSet(key, argv[2]);
    if (argc == 4) {
      long long timeout;
      RedisModule_StringToLongLong(argv[3], &timeout);
      RedisModule_SetExpire(key, timeout * 1000);
    }
  }

  RedisModule_CloseKey(key);
  RedisModule_ReplyWithSimpleString(ctx, "OK");

  return REDISMODULE_OK;
}

int RedisModule_OnLoad(RedisModuleCtx *ctx) {
  if (RedisModule_Init(ctx, "repsheet", 1, REDISMODULE_APIVER_1) == REDISMODULE_ERR) {
    return REDISMODULE_ERR;
  }

  RMUtil_RegisterWriteCmd(ctx, "repsheet.blacklist", BlacklistCommand);
  RMUtil_RegisterWriteCmd(ctx, "repsheet.whitelist", WhitelistCommand);
  RMUtil_RegisterWriteCmd(ctx, "repsheet.mark",      MarkCommand);

  return REDISMODULE_OK;
}
