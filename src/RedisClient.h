#ifndef REDIS_CLIENT_H
#define REDIS_CLIENT_H

#include <string>
#include <vector>
#include <hiredis/hiredis.h>

class RedisClient
{
public:
    RedisClient();
    ~RedisClient();
    
    bool Connect(const std::string& host, int port, const std::string& password = "");
    void Disconnect();
    bool IsConnected() const;
    std::string GetLastError() const;
    
    bool Get(const std::string& key, std::string& value);
    bool Set(const std::string& key, const std::string& value);
    bool Del(const std::string& key);
    bool Keys(const std::string& pattern, std::vector<std::string>& keys);
    bool GetType(const std::string& key, std::string& type);
    
private:
    redisContext* m_context;
    bool m_connected;
    std::string m_lastError;
};

#endif // REDIS_CLIENT_H
