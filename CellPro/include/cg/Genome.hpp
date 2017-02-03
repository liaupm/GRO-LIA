#ifndef CG_GENOME_HPP
#define CG_GENOME_HPP

#include "cg/PlasmidPool.hpp"
#include "cg/Promoter.hpp"

#include <vector>
#include <cstdint>
#include <algorithm>

#define NOT_COMPUTED_TIME -1.0
#define EXPRESSED_GENE -2.0

namespace cg
{
    class Plasmid;
    class PlasmidPool;
    class Genome
    {
        public:
            class PromoterState
            {
                public:
                    const Operon* operon;
                    Promoter::GateState gateState;
                    float breakingTime;
                    bool breakingValue;

                    PromoterState(const Operon* operon);
                    void computeBreakingTime(float simulationTime, RandomEngine& randomEngine);
            };

            class GeneActivation
            {
                public:
                    const Gene* gene;
                    float activationTime;

                    GeneActivation(const Gene* gene);
                    void computeActivationTime(float simulationTime, RandomEngine& randomEngine);
            };

            class ProteinDegradation
            {
                public:
                    const Protein* protein;
                    float degradationTime;

                    ProteinDegradation(const Protein* protein);
                    void computeDegradationTime(float simulationTime, RandomEngine& randomEngine);
            };

            Genome(PlasmidPool* plasmidPool);
            Genome(Genome& genome, bool mutate);
            virtual ~Genome();

            void add(const Plasmid* plasmid);
            void remove(const Plasmid* plasmid);

            void setProteinState(const Protein* protein, bool value);
            void mutateGenes(const Operon* operon, const std::vector<Gene>& genes); // TODO

            void update(double simulationTime, float elapsepTime);

            PlasmidPool* getPlasmidPool() const;
            const std::vector<const Plasmid*>& getPlasmids() const;
            const std::vector<PromoterState>& getPromoterStates() const;
            const std::vector<GeneActivation>& getGeneActivations() const;
            const std::vector<ProteinDegradation>& getProteinDegradations() const;
            uint64_t getProteins() const;

        private:
            PlasmidPool* plasmidPool;
            std::vector<const Plasmid*> plasmids;
            std::vector<PromoterState> promoterStates;
            std::vector<GeneActivation> geneActivations;
            std::vector<ProteinDegradation> proteinDegradations;
            uint64_t proteins;

            void activateOperon(const Operon* operon);
            void deactivateOperon(const Operon* operon);
            void deactivateGene(const Gene* gene);

            void expressGene(unsigned int index);
            void degradateProtein(unsigned int* index);

            void computeMutations(float simulationTime);
    };



    inline Genome::PromoterState::PromoterState(const Operon* operon)
    : operon(operon)
    , gateState(Promoter::GateState::LOW)
    , breakingTime(NOT_COMPUTED_TIME)
    {
    }

    inline void Genome::PromoterState::computeBreakingTime(float simulationTime, RandomEngine& randomEngine)
    {
        float breakToTrueState = simulationTime + operon->getPromoter().getBreakingToTrueTimeDistribution()(randomEngine);
        float breakToFalseState = simulationTime + operon->getPromoter().getBreakingToFalseTimeDistribution()(randomEngine);
        breakingValue = breakToTrueState < breakToFalseState;
        breakingTime = (breakingValue) ? breakToTrueState : breakToFalseState;
    }

    inline Genome::GeneActivation::GeneActivation(const Gene* gene)
    : gene(gene)
    , activationTime(NOT_COMPUTED_TIME)
    {
    }

    inline void Genome::GeneActivation::computeActivationTime(float simulationTime, RandomEngine& randomEngine)
    {
        activationTime = simulationTime + gene->getActivationTimeDistribution()(randomEngine);
    }

    inline Genome::ProteinDegradation::ProteinDegradation(const Protein* protein)
    : protein(protein)
    , degradationTime(NOT_COMPUTED_TIME)
    {
    }

    inline void Genome::ProteinDegradation::computeDegradationTime(float simulationTime, RandomEngine& randomEngine)
    {
        degradationTime = simulationTime + protein->getDegradationTimeDistribution()(randomEngine);
    }

    inline Genome::Genome(PlasmidPool* plasmidPool)
    : plasmidPool(plasmidPool)
    , proteins(0)
    {
    }

    inline Genome::~Genome()
    {
    }

    inline void Genome::setProteinState(const Protein* protein, bool value)
    {
        uint64_t id = protein->getID();

        if(value)
            proteins |= id;
        else
            proteins = ~(id | ~proteins);
    }

    inline PlasmidPool* Genome::getPlasmidPool() const
    {
        return plasmidPool;
    }

    inline const std::vector<const Plasmid*>& Genome::getPlasmids() const
    {
        return plasmids;
    }

    inline const std::vector<Genome::PromoterState>& Genome::getPromoterStates() const
    {
        return promoterStates;
    }

    inline const std::vector<Genome::GeneActivation>& Genome::getGeneActivations() const
    {
        return geneActivations;
    }

    inline const std::vector<Genome::ProteinDegradation>& Genome::getProteinDegradations() const
    {
        return proteinDegradations;
    }

    inline uint64_t Genome::getProteins() const
    {
        return proteins;
    }
}

#endif //CG_GENOME_HPP
