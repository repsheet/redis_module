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
 * REPSHEET.BLACKLIST <address> <reason>
 * Creates a Repsheet blacklist entry for the specified address
 *
 */
int BlacklistCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
  if (argc != 3) {
    return RedisModule_WrongArity(ctx);
  }

  RedisModule_AutoMemory(ctx);

  RedisModuleString *keyString =
      RedisModule_CreateStringPrintf(ctx, "%s:repsheet:ip:blacklisted", str(argv[1]));
  RedisModuleKey *key = RedisModule_OpenKey(ctx, keyString, REDISMODULE_READ | REDISMODULE_WRITE);

  int keytype = RedisModule_KeyType(key);
  if (keytype != REDISMODULE_KEYTYPE_STRING && keytype != REDISMODULE_KEYTYPE_EMPTY) {
    return RedisModule_ReplyWithError(ctx, REDISMODULE_ERRORMSG_WRONGTYPE);
  }

  RedisModuleCallReply *rep = RedisModule_Call(ctx, "SET", "ss", keyString, argv[2]);
  RMUTIL_ASSERT_NOERROR(ctx, rep);

  if (RedisModule_CallReplyType(rep) == REDISMODULE_REPLY_NULL) {
    RedisModule_ReplyWithNull(ctx);
    return REDISMODULE_OK;
  }

  RedisModule_ReplyWithCallReply(ctx, rep);

  return REDISMODULE_OK;
}

int RedisModule_OnLoad(RedisModuleCtx *ctx) {
  if (RedisModule_Init(ctx, "repsheet", 1, REDISMODULE_APIVER_1) == REDISMODULE_ERR) {
    return REDISMODULE_ERR;
  }

  RMUtil_RegisterWriteCmd(ctx, "repsheet.blacklist", BlacklistCommand);

  return REDISMODULE_OK;
}
