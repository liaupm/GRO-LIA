#ifndef CG_PLASMIDPOOL_HPP
#define CG_PLASMIDPOOL_HPP

#include "cg/Plasmid.hpp"
#include "cg/Operon.hpp"
#include "cg/Protein.hpp"

#include <map>
#include <vector>
#include <cstdint>
#include <string>
#include <random>

namespace cg
{
    typedef std::minstd_rand RandomEngine;

    class Gene;
    class Promoter;
    class PlasmidPool
    {
        public:
            PlasmidPool(unsigned int randomSeed);
            virtual ~PlasmidPool();

            const Plasmid* createPlasmid(const std::vector<const Operon*>& operons, const std::string& name = "");
            const Operon* createOperon(const Promoter& promoter, const std::vector<Gene>& genes, const std::string& name = "");
            const Protein* createProtein(float degradationTimeMean, float degradationTimeDeviation, const std::string& name = "");

            const Operon* mutatePromoter(const Operon* operon, Promoter::BreakingState breakingState);
            const Operon* mutateGenes(const Operon* operon, const std::vector<Gene>& genes);

            const Plasmid* getPlasmid(const std::string& key) const;
            const Operon* getOperon(const std::string& key) const;
            const Protein* getProtein(uint64_t id) const;

            const Plasmid* getPlasmidByName(const std::string& name) const;
            const Operon* getOperonByName(const std::string& name) const;
            const Protein* getProteinByName(const std::string& name) const;

            const std::map<std::string, const Plasmid*>& getPlasmids() const;
            const std::map<std::string, const Operon*>& getOperons() const;
            const std::map<uint64_t, const Protein*>& getProteins() const;

            std::vector<const Protein*> getProteins(uint64_t proteinIDs) const;

            RandomEngine& getRandomEngine();

        private:
            RandomEngine randomEngine;
            std::map<std::string, const Plasmid*> plasmids;
            std::map<std::string, const Operon*> operons;
            std::map<uint64_t, const Protein*> proteins;
    };



    inline PlasmidPool::PlasmidPool(unsigned int randomSeed)
    : randomEngine(randomSeed)
    {
    }

    inline const Plasmid* PlasmidPool::getPlasmid(const std::string& key) const
    {
        std::map<std::string, const Plasmid*>::const_iterator it = plasmids.find(key);
        if(it != plasmids.end())
            return it->second;

        return nullptr;
    }

    inline const Operon* PlasmidPool::getOperon(const std::string& key) const
    {
        std::map<std::string, const Operon*>::const_iterator it = operons.find(key);
        if(it != operons.end())
            return it->second;

        return nullptr;
    }

    inline const Protein* PlasmidPool::getProtein(uint64_t id) const
    {
        std::map<uint64_t, const Protein*>::const_iterator it = proteins.find(id);
        if(it != proteins.end())
            return it->second;

        return nullptr;
    }

    inline const Plasmid* PlasmidPool::getPlasmidByName(const std::string& name) const
    {
        for(std::map<std::string, const Plasmid*>::const_iterator it = plasmids.cbegin(); it != plasmids.end(); ++it)
            if(it->second->getName() == name)
                return it->second;
        return nullptr;
    }

    inline const Operon* PlasmidPool::getOperonByName(const std::string& name) const
    {
        for(std::map<std::string, const Operon*>::const_iterator it = operons.cbegin(); it != operons.end(); ++it)
            if(it->second->getName() == name)
                return it->second;
        return nullptr;
    }

    inline const Protein* PlasmidPool::getProteinByName(const std::string& name) const
    {
        for(std::map<uint64_t, const Protein*>::const_iterator it = proteins.cbegin(); it != proteins.end(); ++it)
            if(it->second->getName() == name)
                return it->second;
        return nullptr;
    }

    inline const std::map<std::string, const Plasmid*>& PlasmidPool::getPlasmids() const
    {
        return plasmids;
    }

    inline const std::map<std::string, const Operon*>& PlasmidPool::getOperons() const
    {
        return operons;
    }

    inline const std::map<uint64_t, const Protein*>& PlasmidPool::getProteins() const
    {
        return proteins;
    }

    inline RandomEngine& PlasmidPool::getRandomEngine()
    {
        return randomEngine;
    }
}

#endif //CG_PLASMIDPOOL_HPP
