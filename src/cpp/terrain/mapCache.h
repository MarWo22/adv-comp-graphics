//
// Created by marti on 20/05/2024.
//

#ifndef MAIN_MAPCACHE_H
#define MAIN_MAPCACHE_H

#include <queue>
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

template<typename Key, typename Value>
class MapCache
{
    int m_size;
    std::queue<Key> m_elements;
    std::unordered_map<Key, Value> m_lookupTable;


public:
    explicit MapCache(int size)
            : m_size(size)
    { }

    bool Exists(const Key &key) const {
        return m_lookupTable.find(key) != m_lookupTable.end();
    }

    Value Retrieve(const Key &key) {
        return m_lookupTable[key];
    }

    std::queue<Key> &Keys() {
        return m_elements;
    }


    Value Add(const Key &key, const Value &value) {
        if (m_elements.size() >= m_size) {
            Key oldestKey = m_elements.front();
            m_elements.pop();
            m_lookupTable.erase(oldestKey);
        }
        m_elements.push(key);

        m_lookupTable[key] = value;
        return m_lookupTable[key];
    }

    Value Add(const Key &key, const Value &value, Value *removedItem) {
        if (m_elements.size() >= m_size) {
            Key oldestKey = m_elements.front();
            m_elements.pop();
            *removedItem = m_lookupTable[oldestKey];
            m_lookupTable.erase(oldestKey);
        }
        m_elements.push(key);

        m_lookupTable[key] = value;
        return m_lookupTable[key];
    }


};

template<typename Key, typename Value, typename Hash = std::hash<Key>, typename Equal = std::equal_to<Key>>
class CustomMapCache
{
    int m_size;
    std::queue<Key> m_elements;
    std::unordered_map<Key, Value, Hash, Equal> m_lookupTable;


public:
    explicit CustomMapCache(int size)
            : m_size(size)
    { }

    bool Exists(const Key &key) const {
        return m_lookupTable.find(key) != m_lookupTable.end();
    }

    Value Retrieve(const Key &key) {
        return m_lookupTable[key];
    }


    Value Add(const Key &key, const Value &value) {
        if (m_elements.size() >= m_size) {
            Key oldestKey = m_elements.front();
            m_elements.pop();
            m_lookupTable.erase(oldestKey);
        }
        m_elements.push(key);

        m_lookupTable[key] = value;
        return m_lookupTable[key];
    }

    Value Add(const Key &key, const Value &value, Value *removedItem) {
        if (m_elements.size() >= m_size) {
            Key oldestKey = m_elements.front();
            m_elements.pop();
            *removedItem = m_lookupTable[oldestKey];
            m_lookupTable.erase(oldestKey);
        }
        m_elements.push(key);

        m_lookupTable[key] = value;
        return m_lookupTable[key];
    }

    std::queue<Key> &Keys() {
        return m_elements;
    }

    void Clear() {
        m_lookupTable.clear();
    }


};


#endif //MAIN_MAPCACHE_H
