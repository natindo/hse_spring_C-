#ifndef BD_H
#define BD_H

#include <string>

struct db {
    std::string key;
    std::string value;
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

    size_t getSize() const;
    size_t getCapacity() const;

private:
    db* database_;
    size_t size_;
    size_t capacity_;

    void reallocate(size_t newSize);
    [[nodiscard]] int findValue(const std::string& key) const;
};

#endif //BD_H
