#include "RedisClient.h"
#include <cstring>

RedisClient::RedisClient()
    : m_context(nullptr), m_connected(false)
{
}

RedisClient::~RedisClient()
{
    Disconnect();
}

bool RedisClient::Connect(const std::string& host, int port, const std::string& password)
{
    if (m_connected)
    {
        Disconnect();
    }
    
    m_lastError.clear();
    m_context = redisConnect(host.c_str(), port);
    if (m_context == nullptr || m_context->err)
    {
        if (m_context)
        {
            m_lastError = std::string(m_context->errstr);
            redisFree(m_context);
            m_context = nullptr;
        }
        else
        {
            m_lastError = "Failed to allocate redis context";
        }
        return false;
    }
    
    // Authenticate if password is provided
    if (!password.empty())
    {
        redisReply* reply = (redisReply*)redisCommand(m_context, "AUTH %s", password.c_str());
        if (reply == nullptr || reply->type == REDIS_REPLY_ERROR)
        {
            if (reply)
            {
                m_lastError = std::string(reply->str);
                freeReplyObject(reply);
            }
            else
            {
                m_lastError = "Authentication command failed";
            }
            redisFree(m_context);
            m_context = nullptr;
            return false;
        }
        freeReplyObject(reply);
    }
    
    m_connected = true;
    return true;
}

void RedisClient::Disconnect()
{
    if (m_context)
    {
        redisFree(m_context);
        m_context = nullptr;
    }
    m_connected = false;
}

bool RedisClient::IsConnected() const
{
    return m_connected;
}

std::string RedisClient::GetLastError() const
{
    return m_lastError;
}

bool RedisClient::Get(const std::string& key, std::string& value)
{
    if (!m_connected || !m_context)
        return false;
    
    redisReply* reply = (redisReply*)redisCommand(m_context, "GET %s", key.c_str());
    if (reply == nullptr)
        return false;
    
    bool success = false;
    if (reply->type == REDIS_REPLY_STRING)
    {
        value = std::string(reply->str, reply->len);
        success = true;
    }
    else if (reply->type == REDIS_REPLY_NIL)
    {
        success = false;
    }
    
    freeReplyObject(reply);
    return success;
}

bool RedisClient::Set(const std::string& key, const std::string& value)
{
    if (!m_connected || !m_context)
        return false;
    
    redisReply* reply = (redisReply*)redisCommand(m_context, "SET %s %b", 
                                                   key.c_str(), value.c_str(), value.length());
    if (reply == nullptr)
        return false;
    
    bool success = (reply->type == REDIS_REPLY_STATUS && 
                    strcmp(reply->str, "OK") == 0);
    
    freeReplyObject(reply);
    return success;
}

bool RedisClient::Del(const std::string& key)
{
    if (!m_connected || !m_context)
        return false;
    
    redisReply* reply = (redisReply*)redisCommand(m_context, "DEL %s", key.c_str());
    if (reply == nullptr)
        return false;
    
    bool success = (reply->type == REDIS_REPLY_INTEGER && reply->integer > 0);
    
    freeReplyObject(reply);
    return success;
}

bool RedisClient::Keys(const std::string& pattern, std::vector<std::string>& keys)
{
    if (!m_connected || !m_context)
        return false;
    
    keys.clear();
    
    redisReply* reply = (redisReply*)redisCommand(m_context, "KEYS %s", pattern.c_str());
    if (reply == nullptr)
        return false;
    
    bool success = false;
    if (reply->type == REDIS_REPLY_ARRAY)
    {
        for (size_t i = 0; i < reply->elements; ++i)
        {
            if (reply->element[i]->type == REDIS_REPLY_STRING)
            {
                keys.push_back(std::string(reply->element[i]->str, reply->element[i]->len));
            }
        }
        success = true;
    }
    
    freeReplyObject(reply);
    return success;
}

bool RedisClient::GetType(const std::string& key, std::string& type)
{
    if (!m_connected || !m_context)
        return false;
    
    redisReply* reply = (redisReply*)redisCommand(m_context, "TYPE %s", key.c_str());
    if (reply == nullptr)
        return false;
    
    bool success = false;
    if (reply->type == REDIS_REPLY_STATUS)
    {
        type = std::string(reply->str, reply->len);
        success = true;
    }
    
    freeReplyObject(reply);
    return success;
}
