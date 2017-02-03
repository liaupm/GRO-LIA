#ifndef CG_PROTEIN_HPP
#define CG_PROTEIN_HPP

#include <random>
#include <cstdint>
#include <string>

namespace cg
{
    class Protein
    {
        public:
            Protein(unsigned int index, const std::string& name, float degradationTimeMean, float degradationTimeDeviation);
            virtual ~Protein();

            const std::string& getKey() const;
            uint64_t getID() const;
            const std::string& getName() const;
            std::normal_distribution<float> getDegradationTimeDistribution() const;

        private:
            std::string key;
            uint64_t id;
            std::string name;
            std::normal_distribution<float> degradationTimeDistribution;
    };



    inline Protein::Protein(unsigned int index, const std::string& name, float degradationTimeMean, float degradationTimeDeviation)
    : key("p" + std::to_string(index))
    , id(static_cast<uint64_t>(1) << index)
    , name(name)
    , degradationTimeDistribution(degradationTimeMean, degradationTimeDeviation)
    {
    }

    inline Protein::~Protein()
    {
    }

    inline const std::string& Protein::getKey() const
    {
        return key;
    }

    inline uint64_t Protein::getID() const
    {
        return id;
    }

    inline const std::string& Protein::getName() const
    {
        return name;
    }

    inline std::normal_distribution<float> Protein::getDegradationTimeDistribution() const
    {
        return degradationTimeDistribution;
    }
}

#endif //CG_PROTEIN_HPP
