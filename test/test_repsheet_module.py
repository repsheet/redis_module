import redis


connection = redis.Redis(host='localhost', port=6379)


def test_redis_available():
    assert connection.ping()
