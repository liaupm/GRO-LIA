#include "cg/PlasmidPool.hpp"

using namespace cg;

PlasmidPool::~PlasmidPool()
{
    for(std::map<std::string, const Plasmid*>::iterator it = plasmids.begin(); it != plasmids.end(); ++it)
        delete it->second;

    for(std::map<std::string, const Operon*>::iterator it = operons.begin(); it != operons.end(); ++it)
        delete it->second;

    for(std::map<uint64_t, const Protein*>::iterator it = proteins.begin(); it != proteins.end(); ++it)
        delete it->second;
}

const Plasmid* PlasmidPool::createPlasmid(const std::vector<const Operon*>& operons, const std::string& name)
{
    std::string key;
    for(unsigned int o = 0; o < operons.size(); o++)
        key += operons[o]->getKey();

    std::map<std::string, const Plasmid*>::iterator it = plasmids.find(key);
    if(it == plasmids.end())
    {
        Plasmid* plasmid = new Plasmid(key, name, operons);
        plasmids.insert(std::pair<std::string, const Plasmid*>(key, plasmid));
        return plasmid;
    }

    return it->second;
}

const Operon* PlasmidPool::createOperon(const Promoter& promoter, const std::vector<Gene>& genes, const std::string& name)
{
    std::string key = "·" + std::to_string(operons.size()) + "b" + std::to_string(promoter.getBreakingState());
    for(unsigned int g = 0; g < genes.size(); g++)
        key += genes[g].getProtein()->getKey();

    Operon* operon = new Operon(key, name, promoter, genes);
    operons.insert(std::pair<std::string, const Operon*>(key, operon));
    return operon;
}

const Protein* PlasmidPool::createProtein(float degradationTimeMean, float degradationTimeDeviation, const std::string& name)
{
    Protein* protein = new Protein(proteins.size(), name, degradationTimeMean, degradationTimeDeviation);
    proteins.insert(std::pair<uint64_t, const Protein*>(protein->getID(), protein));
    return protein;
}

const Operon* PlasmidPool::mutatePromoter(const Operon* operon, Promoter::BreakingState breakingState)
{
    std::string key = operon->getKey();
    for(unsigned int i = 0; i < key.size(); i++)
        if(key[i] == 'b')
        {
            key[i + 1] = 48 + breakingState;
            break;
        }

    std::map<std::string, const Operon*>::iterator it = operons.find(key);
    if(it == operons.end())
    {
        Promoter promoter = operon->getPromoter();
        promoter.setBreakingState(breakingState);

        Operon* newOperon = new Operon(key, operon->getName(), promoter, operon->getGenes());
        operons.insert(std::pair<std::string, const Operon*>(key, newOperon));
        return newOperon;
    }

    return it->second;
}

const Operon* PlasmidPool::mutateGenes(const Operon* operon, const std::vector<Gene>& genes)
{
    std::string key = operon->getKey().substr(4, std::string::npos);
    for(unsigned int i = 0; i < genes.size(); i++)
        key += genes[i].getProtein()->getKey();

    std::map<std::string, const Operon*>::iterator it = operons.find(key);
    if(it == operons.end())
    {
        Operon* newOperon = new Operon(key, operon->getName(), operon->getPromoter(), genes);
        operons.insert(std::pair<std::string, const Operon*>(key, newOperon));
        return newOperon;
    }

    return it->second;
}

std::vector<const Protein*> PlasmidPool::getProteins(uint64_t proteinIDs) const
{
    std::vector<const Protein*> proteinList;
    for(unsigned int i = 0; i < 64; i++)
    {
        const Protein* protein = getProtein((static_cast<uint64_t>(1) << i) & proteinIDs);
        if(protein)
            proteinList.push_back(protein);
    }
    return proteinList;
}