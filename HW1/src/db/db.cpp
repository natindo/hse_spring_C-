#include "db.h"
#include <iostream>

constexpr size_t INIT_SIZE = 10;
// constexpr std::string NOT_FOUND ();

Database::Database() : database_(nullptr), size_(0), capacity_(0) {
    reallocate(INIT_SIZE);
}

Database::~Database() {
    delete[] database_;
}

bool Database::set(const std::string& key, const std::string& value) {
    if (size_ == capacity_) {
        reallocate(size_ * 2);
    }
    const int index = findValue(key);
    if (index > -1) {
        database_[index].value = value;
        return true;
    }
    database_[size_].key = key;
    database_[size_].value = value;
    size_++;
    return true;
}

bool Database::del(const std::string& key) {
    const size_t index = findValue(key);
    if (index == -1) {
        return false;
    }

    for (size_t i = index; i < size_ - 1; i++) {
        database_[i].key = database_[i + 1].key;
        database_[i].value = database_[i + 1].value;
    }
    size_--;
    return true;
}

std::string Database::get(const std::string& key) const {
    const size_t index = findValue(key);
    if (index == -1) {
        return "not found";
    }
    return database_[index].value;
}

void Database::reallocate(const size_t newSize) {
    const auto newDatabase = new db[newSize];
    if (database_ != nullptr) {
        for (size_t i = 0; i < size_; i++) {
            newDatabase[i] = database_[i];
        }
        delete[] database_;
    }
    database_ = newDatabase;
    capacity_ = newSize;
}

int Database::findValue(const std::string& key) const {
    for (int i = 0; i < size_; i++) {
        if (database_[i].key == key) {
            return i;
        }
    }
    return -1;
}

size_t Database::getSize() const {
    return size_;
}

size_t Database::getCapacity() const {
    return capacity_;
}
