#ifndef CG_GENE_HPP
#define CG_GENE_HPP

#include <random>

namespace cg
{
    class Protein;
    class Gene
    {
        public:
            Gene(const Protein* protein, float activationTimeMean, float activationTimeDeviation);
            virtual ~Gene();

            void setProtein(const Protein* protein);

            const Protein* getProtein() const;
            std::normal_distribution<float> getActivationTimeDistribution() const;

        private:
            const Protein* protein;
            std::normal_distribution<float> activationTimeDistribution;
    };



    inline Gene::Gene(const Protein* protein, float activationTimeMean, float activationTimeDeviation)
    : protein(protein)
    , activationTimeDistribution(activationTimeMean, activationTimeDeviation)
    {
    }

    inline Gene::~Gene()
    {
    }

    inline void Gene::setProtein(const Protein* protein)
    {
        this->protein = protein;
    }

    inline const Protein* Gene::getProtein() const
    {
        return protein;
    }

    inline std::normal_distribution<float> Gene::getActivationTimeDistribution() const
    {
        return activationTimeDistribution;
    }
}

#endif //CG_GENE_HPP
