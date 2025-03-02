#ifndef BD_H
#define BD_H

#include <string>

constexpr size_t INIT_SIZE = 1000;

struct data {
    std::string key;
    std::string value;
    data* next;
};

class Database {
public:
    Database();
    ~Database();

    Database(const Database &rhs) = delete;
    Database operator=(const Database &rhs) = delete;
    Database(Database &&rhs) = delete;
    Database operator=(Database &&rhs) = delete;

    bool set(const std::string& key, const std::string& value);
    bool del(const std::string& key);
    [[nodiscard]] std::string get(const std::string& key) const;

private:
    data** database_ = nullptr;
    size_t size_ = INIT_SIZE;

    [[nodiscard]] static size_t getHash(const std::string& key) ;
};

#endif //BD_H
