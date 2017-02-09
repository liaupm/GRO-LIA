#ifndef CG_OPERON_HPP
#define CG_OPERON_HPP

#include "cg/Promoter.hpp"
#include "cg/Gene.hpp"

#include <vector>
#include <string>

namespace cg
{
    class Operon
    {
        public:
            Operon(const std::string& key, const std::string& name, const Promoter& promoter, const std::vector<Gene>& genes);
            virtual ~Operon();

            const std::string& getKey() const;
            const std::string& getName() const;
            const Promoter& getPromoter() const;
            const std::vector<Gene>& getGenes() const;

        private:
            std::string key;
            std::string name;
            Promoter promoter;
            std::vector<Gene> genes;
    };



    inline Operon::Operon(const std::string& key, const std::string& name, const Promoter& promoter, const std::vector<Gene>& genes)
    : key(key)
    , name(name)
    , promoter(promoter)
    , genes(genes)
    {
    }

    inline Operon::~Operon()
    {
    }

    inline const std::string& Operon::getKey() const
    {
        return key;
    }

    inline const std::string& Operon::getName() const
    {
        return name;
    }

    inline const Promoter& Operon::getPromoter() const
    {
        return promoter;
    }

    inline const std::vector<Gene>& Operon::getGenes() const
    {
        return genes;
    }
}

#endif //CG_OPERON_HPP
