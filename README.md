# Repsheet Redis Module

## Module Commands

* `REPSHEET.BLACKLIST` - Blacklist an IP Address

## Quick Start Guide

1. Build the module by running `make`
2. Run redis loading the module: `/path/to/redis-server --loadmodule ./module.so`

Now run `redis-cli` and try the commands:

```
127.0.0.1:6379> REPSHEET.BLACKLIST 1.1.1.1 manual
OK
127.0.0.1:6379> keys *
1) "1.1.1.1:repsheet:ip:blacklisted"
127.0.0.1:6379> get 1.1.1.1:repsheet:ip:blacklisted
"manual"
```
