#ifndef HEADER_RESOURCEPACK_H
#define HEADER_RESOURCEPACK_H

#include "Log.h"
#include "Random.h"
#include "ResourceException.h"

#include <string>
#include <map>

/**
 * Share resources.
 */
template <class T>
class ResourcePack {
    private:
    typedef std::multimap<std::string,T> t_reses;
    typedef typename t_reses::iterator t_resIterator;
    t_reses m_reses;
    protected:
    virtual void unloadRes(T res) = 0;

    public:
    virtual ~ResourcePack() {}
    //-----------------------------------------------------------------
    /**
     * Free all resources.
     * NOTE: we cannot call virtual functions from desctructor
     */
    void removeAll()
        {
            t_resIterator end = m_reses.end();
            for (t_resIterator i = m_reses.begin(); i != end; ++i) {
                unloadRes(i->second);
            }
        }
    //-----------------------------------------------------------------
    /**
     * Unload all resources with this name.
     */
    void removeRes(const std::string &name)
        {
            std::pair<t_resIterator, t_resIterator> range =
                m_reses.equal_range(name);
            while (range.first != range.second) {
                unloadRes(range.first->second);
                ++(range.first);
            }
            m_reses.erase(name);
            LOG_DEBUG(ExInfo("removed resources")
                    .addInfo("name", name));
        }

    //-----------------------------------------------------------------
    /**
     * Store resource under this name.
     */
    void addRes(const std::string &name, T res)
    {
        m_reses.insert(
                std::pair<std::string,T>(name, res));
        LOG_DEBUG(ExInfo("added resource")
                .addInfo("name", name));
    }
    //-----------------------------------------------------------------
    /**
     * Get resource with this name.
     */
    T getRes(const std::string &name, int rank=0)
    {
        std::pair<t_resIterator, t_resIterator> range =
            m_reses.equal_range(name);
        for (int i = 0; i < rank && range.first != range.second; ++i) {
            ++(range.first);
        }
        if (m_reses.end() == range.first) {
            throw ResourceException(ExInfo("no such resource at index")
                    .addInfo("name", name)
                    .addInfo("index", rank));
        }
        return range.first->second;
    }
    //-----------------------------------------------------------------
    /**
     * Get resource at random index.
     */
    T getRandomRes(const std::string &name)
    {
        typename t_reses::size_type count = m_reses.count(name);
        return getRes(name, Random::randomInt(count));
    }
    //-----------------------------------------------------------------
    /**
     * Count resources with this name.
     */
    int countRes(const std::string &name)
    {
        return m_reses.count(name);
    }
};

#endif

