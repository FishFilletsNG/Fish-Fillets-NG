#ifndef HEADER_RESCACHE_H
#define HEADER_RESCACHE_H

#include "NoCopy.h"
#include "ResourcePack.h"

#include "Log.h"

template <class T>
class CacheEntry {
    public:
    std::string name;
    T value;
    int refcount;

    CacheEntry() {
        name = "";
        value = NULL;
        refcount = 0;
    }
};

/**
 * A fixed size cache for any resources.
 */
template <class T>
class ResCache : public NoCopy {
    private:
        typedef std::vector<class CacheEntry<T>*> t_entries;
        t_entries m_entries;
        ResourcePack<T> *m_unloader;
        unsigned int m_next_pos;
    public:
        /**
         * Creates a cache with the given capacity.
         * The given unloader has to have disabled caching
         * to prevent an infinite loop.
         */
        ResCache(int capacity, ResourcePack<T> *new_unloader) {
            m_entries.resize(capacity);
            for (unsigned int i = 0; i < m_entries.size(); i++) {
                m_entries[i] = new CacheEntry<T>();
            }

            m_next_pos = 0;
            m_unloader = new_unloader;
        }

        ~ResCache() {
            for (unsigned int i = 0; i < m_entries.size(); i++) {
                delete m_entries[i];
            }
            delete m_unloader;
        }

        /**
         * Returns a found value or NULL.
         * The returned item should be released via release().
         */
        T get(const std::string &name) {
            CacheEntry<T> *entry = getEntry(name);
            if (!entry) {
                return NULL;
            }

            entry->refcount++;
            return entry->value;
        }

        /**
         * Notes a new value.
         * The caller should release it later via release().
         */
        void put(const std::string &name, T value) {
            CacheEntry<T>* entry = findNextUnusedEntry();
            if (!entry) {
                LOG_DEBUG(ExInfo("cannot fit into cache")
                        .addInfo("name", name));
                return;
            }

            if (entry->value != NULL) {
                m_unloader->unloadRes(entry->value);
            }
            entry->name = name;
            entry->value = value;
            entry->refcount = 1;
        }

        /**
         * Releases or takes responsibility for the given value.
         */
        void release(T value) {
            CacheEntry<T> *found = getByValue(value);
            if (found) {
                found->refcount -= 1;
                if (found->refcount < 0) {
                    LOG_WARNING(ExInfo("extra release of a cache entry"));
                    found->refcount = 0;
                }
            } else {
                m_unloader->unloadRes(value);
            }
        }

    private:
        /**
         * Returns the matching CacheEntry or NULL.
         */
        CacheEntry<T> *getEntry(const std::string &name) {
            for (unsigned int i = 0; i < m_entries.size(); i++) {
                CacheEntry<T> *entry = m_entries[i];
                if (entry->value != NULL && entry->name == name) {
                    return entry;
                }
            }

            return NULL;
        }

        /**
         * Returns the matching CacheEntry or NULL.
         */
        CacheEntry<T> *getByValue(T value) {
            for (unsigned int i = 0; i < m_entries.size(); i++) {
                CacheEntry<T> *entry = m_entries[i];
                if (entry->value == value) {
                    return entry;
                }
            }

            return NULL;
        }

        /**
         * Returns a next unused CacheEntry or NULL when all entries are full.
         */
        CacheEntry<T>* findNextUnusedEntry() {
            for (unsigned int attempt = 0; attempt < m_entries.size(); attempt++) {
                CacheEntry<T> *entry = m_entries[m_next_pos];
                m_next_pos = (m_next_pos + 1) % m_entries.size();

                if (entry->refcount <= 0) {
                    return entry;
                }
            }

            return NULL;
        }
};

#endif

