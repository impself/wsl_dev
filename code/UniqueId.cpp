/*
生成唯一的标识符（ID）
https://mp.weixin.qq.com/s/Lx7proUAmMe_mKmghfuI5g
*/
#include <iostream>
#include <chrono>
/*
使用时间戳生成唯一ID原理　利用当前系统的时间戳来生成唯一ID。
常见的时间戳格式有Unix时间戳（从1970年1月1日以来的秒数）和精确到毫秒甚至微秒的时间戳。　
优缺点　优点：实现简单，生成的ID具有递增性，时间戳本身具有高精度。
缺点：如果多次生成ID的时间点非常接近（例如，生成ID的代码运行在同一毫秒内），则可能生成重复的ID。
*/
std::string generateUniqueId()
{
    // 获取当前的系统时间戳，单位为毫秒
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    long long timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    return std::to_string(timestamp);
}
/*
UUID（Universally Unique Identifier）是一个广泛使用的标准，用于生成唯一的标识符。
UUID的长度为128位，通常以16进制字符串表示，
形式如：550e8400-e29b-41d4-a716-446655440000。　UUID可以通过各种算法生成，其中最常用的算法是基于随机数生成的算法。
优缺点　优点：生成的UUID几乎可以保证全球唯一，使用广泛，且不依赖于系统的时钟。
缺点：UUID字符串较长，占用空间大，对于小范围使用场景来说，可能是过度设计。
*/
#include <iostream>
#include <random>
#include <iomanip>
#include <sstream>

std::string generateUUID()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 15);

    std::stringstream ss;
    ss << std::hex;
    for (int i = 0; i < 8; ++i)
    {
        ss << dist(gen);
    }
    ss << "-";
    for (int i = 0; i < 4; ++i)
    {
        ss << dist(gen);
    }
    ss << "-";
    for (int i = 0; i < 4; ++i)
    {
        ss << dist(gen);
    }
    ss << "-";
    for (int i = 0; i < 4; ++i)
    {
        ss << dist(gen);
    }
    ss << "-";
    for (int i = 0; i < 12; ++i)
    {
        ss << dist(gen);
    }

    return ss.str();
}
/*
使用序列号生成唯一ID在一些应用场景中，可以使用自增的序列号生成唯一ID。序列号的生成方式简单且易于理解，常见于数据库或消息队列中。　
优点：实现简单，生成的ID是递增的，易于维护和调试。
缺点：如果系统重启或分布式系统中的多个节点同时生成ID，可能会出现冲突。
*/
#include <iostream>
#include <atomic>

class IDGenerator
{
public:
    IDGenerator() : current_id(0) {}

    std::string generateUniqueId()
    {
        current_id.fetch_add(1);
        return std::to_string(current_id);
    }

private:
    std::atomic<long long> current_id; // 使用原子操作确保线程安全
};

/*
基于机器ID和时间戳生成唯一ID对于分布式系统，常常会根据机器的唯一标识（如MAC地址、IP地址等）结合时间戳来生成唯一ID。
这样的ID生成策略常见于分布式ID生成系统如Snowflake。　
优点：适用于分布式系统，可以保证每个节点生成的ID唯一，并且具有时间递增性。
缺点：实现较为复杂，涉及到机器ID、时间戳的处理，可能需要全局统一的时钟。
*/
#include <iostream>
#include <chrono>
#include <random>
#include <bitset>

class SnowflakeIdGenerator
{
public:
    SnowflakeIdGenerator(int nodeId) : nodeId(nodeId) {}

    long long generateUniqueId()
    {
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = now.time_since_epoch();
        long long timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

        return (timestamp << 22) | (nodeId << 12) | (random() & 0xFFF);
    }

private:
    int nodeId; // 机器ID
    std::random_device rd;

    int random()
    {
        std::uniform_int_distribution<int> dist(0, 4095);
        return dist(rd);
    }
};
/*
哈希值生成唯一id
*/
// #include <iostream>
// #include <sstream>
// #include <iomanip>
// #include <openssl/md5.h>

// std::string generateHashId(const std::string &input)
// {
//     unsigned char digest[MD5_DIGEST_LENGTH];
//     MD5_CTX md5Context;
//     MD5_Init(&md5Context);
//     MD5_Update(&md5Context, input.c_str(), input.length());
//     MD5_Final(digest, &md5Context);

//     std::stringstream ss;
//     for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
//     {
//         ss << std::setw(2) << std::setfill('0') << std::hex << (int)digest[i];
//     }
//     return ss.str();
// }

int main()
{
    std::cout << "Unique ID: " << generateUniqueId() << std::endl;

    std::cout << "Generated UUID: " << generateUUID() << std::endl;

    IDGenerator idGen;
    std::cout << "Unique ID: " << idGen.generateUniqueId() << std::endl;

    SnowflakeIdGenerator generator(1); // 机器ID为1
    std::cout << "Generated Unique ID: " << generator.generateUniqueId() << std::endl;

    return 0;
}