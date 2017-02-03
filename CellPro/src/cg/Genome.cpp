#include "cg/Genome.hpp"

#include <functional>

using namespace cg;

Genome::Genome(Genome& genome, bool mutate)
: plasmidPool(genome.plasmidPool)
, plasmids(genome.plasmids)
, promoterStates(genome.promoterStates)
, geneActivations(genome.geneActivations)
, proteinDegradations(genome.proteinDegradations)
, proteins(genome.proteins)
{
    if(mutate)
    {
        for(unsigned int i = 0; i < promoterStates.size(); i++)
            if(promoterStates[i].operon->getPromoter().getBreakingState() == Promoter::BreakingState::NOT_BROKEN)
                promoterStates[i].breakingTime = NOT_COMPUTED_TIME;
    }
}

void Genome::add(const Plasmid* plasmid)
{
    for(unsigned int i = 0; i < plasmids.size(); i++)
        if(plasmids[i] == plasmid)
            return;

    plasmids.push_back(plasmid);
    for(unsigned int i = 0; i < plasmid->getOperons().size(); i++)
        promoterStates.push_back(PromoterState(plasmid->getOperons()[i]));
}

void Genome::remove(const Plasmid* plasmid)
{
    std::vector<PromoterState>::iterator ipt = promoterStates.begin();
    for(std::vector<const Plasmid*>::iterator ip = plasmids.begin(); ip < plasmids.end(); ++ip)
    {
        const std::vector<const Operon*>& operons = (*ip)->getOperons();
        if(*ip == plasmid)
        {
            for(unsigned int o = 0; o < operons.size(); o++)
                deactivateOperon(operons[o]);

            promoterStates.erase(ipt, ipt + operons.size());
            plasmids.erase(ip);

            return;
        }
        ipt += operons.size();
    }
}

void Genome::update(double simulationTime, float elapsepTime)
{
    /* Check promoters mutations and compute them */
    bool mutations = false;
    for(unsigned int p = 0; p < promoterStates.size(); p++)
    {
        if(promoterStates[p].breakingTime == NOT_COMPUTED_TIME)
        {
            if(promoterStates[p].operon->getPromoter().getBreakingState() == Promoter::BreakingState::NOT_BROKEN)
                promoterStates[p].computeBreakingTime(simulationTime - elapsepTime, plasmidPool->getRandomEngine());
            else
                promoterStates[p].breakingTime = std::numeric_limits<float>::infinity();
        }
        else if(simulationTime >= promoterStates[p].breakingTime)
            mutations = true;
    }
    if(mutations)
        computeMutations(simulationTime);

    /* Compute the falling edges and rising edges of promoters */
    for(unsigned int p = 0; p < promoterStates.size(); p++)
    {
        const Promoter& promoter = promoterStates[p].operon->getPromoter();
        promoterStates[p].gateState = promoter.nextState(promoterStates[p].gateState, proteins);

        if(promoterStates[p].gateState == Promoter::GateState::LOW_TO_HIGH)
            activateOperon(promoterStates[p].operon);

        else if(promoterStates[p].gateState == Promoter::GateState::HIGH_TO_LOW)
            deactivateOperon(promoterStates[p].operon);
    }

    /* Compute the protein degradations */
    for(unsigned int d = 0; d < proteinDegradations.size(); d++)
    {
        if(proteinDegradations[d].degradationTime == NOT_COMPUTED_TIME)
            proteinDegradations[d].computeDegradationTime(simulationTime - elapsepTime, plasmidPool->getRandomEngine());

        if(simulationTime >= proteinDegradations[d].degradationTime)
            degradateProtein(&d);
    }

    /* Compute the genes activations */
    for(unsigned int a = 0; a < geneActivations.size(); a++)
    {
        if(geneActivations[a].activationTime == NOT_COMPUTED_TIME)
            geneActivations[a].computeActivationTime(simulationTime - elapsepTime, plasmidPool->getRandomEngine());

        if(simulationTime >= geneActivations[a].activationTime && geneActivations[a].activationTime != EXPRESSED_GENE)
            expressGene(a);
    }
}

void Genome::activateOperon(const Operon* operon)
{
    const std::vector<Gene>& genes = operon->getGenes();
    for(unsigned int g = 0; g < genes.size(); g++)
        geneActivations.push_back(GeneActivation(&genes[g]));
}

void Genome::deactivateOperon(const Operon* operon)
{
    const std::vector<Gene>& genes = operon->getGenes();
    for(unsigned int g = 0; g < genes.size(); g++)
        deactivateGene(&genes[g]);
}

void Genome::deactivateGene(const Gene* gene)
{
    bool currentGeneExpressing = false;
    for(unsigned int a = 0; a < geneActivations.size();)
    {
        if(geneActivations[a].gene == gene)
        {
            if(geneActivations[a].activationTime == EXPRESSED_GENE)
                currentGeneExpressing = true;
            geneActivations[a] = geneActivations.back();
            geneActivations.pop_back();
        }
        else
            a++;
    }

    unsigned int otherGenesExpressing = 0;
    for(unsigned int a = 0; a < geneActivations.size(); a++)
    {
        if(gene->getProtein() == geneActivations[a].gene->getProtein())
            if(geneActivations[a].activationTime == EXPRESSED_GENE)
                otherGenesExpressing++;
    }

    if(currentGeneExpressing && otherGenesExpressing == 0)
        proteinDegradations.push_back(ProteinDegradation(gene->getProtein()));
}

void Genome::expressGene(unsigned int index)
{
    proteins |= geneActivations[index].gene->getProtein()->getID();

    geneActivations[index].activationTime = EXPRESSED_GENE;
    for(unsigned int d = 0; d < proteinDegradations.size(); d++)
        if(geneActivations[index].gene->getProtein() == proteinDegradations[d].protein)
        {
            proteinDegradations[d] = proteinDegradations.back();
            proteinDegradations.pop_back();
            return;
        }
}

void Genome::degradateProtein(unsigned int *index)
{
    proteins -= proteinDegradations[*index].protein->getID();

    proteinDegradations[*index] = proteinDegradations.back();
    proteinDegradations.pop_back();

    (*index)--;
}

void Genome::computeMutations(float simulationTime)
{
    /* Check broken promoters */
    unsigned int s = 0;
    for(unsigned int p = 0; p < plasmids.size(); p++)
    {
        bool mutations = false;
        const std::vector<const Operon*>& operons = plasmids[p]->getOperons();
        for(unsigned int o = 0; o < operons.size(); o++)
        {
            PromoterState& promoterState = promoterStates[s + o];
            if(simulationTime >= promoterState.breakingTime)
            {
                Promoter::BreakingState breakingState = static_cast<Promoter::BreakingState>(promoterState.breakingValue);
                const Operon* brokenOperon = plasmidPool->mutatePromoter(promoterState.operon, breakingState);
                promoterState.operon = brokenOperon;
                promoterState.breakingTime = std::numeric_limits<float>::infinity();

                const std::vector<Gene>& previousGenes = operons[o]->getGenes();
                const std::vector<Gene>& currentGenes = brokenOperon->getGenes();
                for(unsigned int a = 0; a < geneActivations.size(); a++)
                    for(unsigned int g = 0; g < previousGenes.size(); g++)
                        if(geneActivations[a].gene == &previousGenes[g])
                            geneActivations[a].gene = &currentGenes[g];

                mutations = true;
            }
        }

        if(mutations)
        {
            std::vector<const Operon*> operonsCache;
            for(unsigned int o = 0; o < plasmids[p]->getOperons().size(); o++)
                operonsCache.push_back(promoterStates[s + o].operon);

            plasmids[p] = plasmidPool->createPlasmid(operonsCache, plasmids[p]->getName());
        }

        s += operons.size();
    }
}
