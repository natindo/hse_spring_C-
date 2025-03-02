#include "db.h"
#include <iostream>

Database::Database() {
    database_ = new data*[INIT_SIZE];
    for (size_t i = 0; i < INIT_SIZE; i++) {
        database_[i] = nullptr;
    }
}

Database::~Database() {
    if (database_ != nullptr) {
        for (size_t i = 0; i < INIT_SIZE; i++) {
            data* current = database_[i];
            while (current != nullptr) {
                data* toDelete = current;
                current = current->next;
                delete toDelete;
            }
        }
        delete[] database_;
    }
}

bool Database::set(const std::string& key, const std::string& value) {
    if (database_ == nullptr) {
        return false;
    }
    const size_t hash = getHash(key);
    const size_t index = hash % INIT_SIZE;

    data* current = database_[index];
    data* prev = nullptr;

    while (current != nullptr) {
        if (current->key == key) {
            current->value = value;
            return true;
        }
        prev = current;
        current = current->next;
    }

    const auto newData = new data();
    newData->key = key;
    newData->value = value;
    newData->next = nullptr;

    if (prev == nullptr) {
        database_[index] = newData;
    } else {
        prev->next = newData;
    }
    return true;
}

bool Database::del(const std::string& key) {
    if (database_ == nullptr) {
        return false;
    }
    const size_t hash = getHash(key);
    const size_t index = hash % INIT_SIZE;

    data* current = database_[index];
    data* prev = nullptr;

    while (current != nullptr) {
        if (current->key == key) {
            if (prev == nullptr) {
                database_[index] = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

std::string Database::get(const std::string& key) const {
    if (database_ == nullptr) {
        return "";
    }

    const size_t hash = getHash(key);
    const size_t index = hash % size_;

    data* current = database_[index];

    while (current != nullptr) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }
    return "";
}

size_t Database::getHash(const std::string& key) {
    size_t result = 0;
    for (const char c : key) {
        result += c;
    }
    return result;
}
