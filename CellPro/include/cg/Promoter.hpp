#ifndef CG_PROMOTER_HPP
#define CG_PROMOTER_HPP

#include <cstdint>
#include <random>
#include <iostream>
#include <limits>

namespace cg
{
    class Promoter
    {
        public:
            enum Gate
            {
                OPEN, CLOSED, AND, NAND, OR, XOR
            };

            enum BreakingState
            {
                BREAK_TO_FALSE, BREAK_TO_TRUE, NOT_BROKEN
            };

            enum GateState
            {
                LOW, LOW_TO_HIGH, HIGH_TO_LOW, HIGH
            };

            Promoter(uint64_t gateMask, uint64_t proteinValues, Gate gate, BreakingState breakingState, float breakingToTrueRate = std::numeric_limits<float>::infinity(), float breakingToFalseRate = std::numeric_limits<float>::infinity());
            virtual ~Promoter();

            void setBreakingState(BreakingState breakingState);

            uint64_t getGateMask() const;
            uint64_t getProteinValues() const;
            Gate getGate() const;
            BreakingState getBreakingState() const;
            std::exponential_distribution<float> getBreakingToTrueTimeDistribution() const;
            std::exponential_distribution<float> getBreakingToFalseTimeDistribution() const;

            bool check(uint64_t proteins) const;
            GateState nextState(GateState gateState, uint64_t proteins) const;

        private:
            uint64_t gateMask;
            uint64_t proteinValues;
            Gate gate;
            BreakingState breakingState;
            std::exponential_distribution<float> breakingToTrueTimeDistribution;
            std::exponential_distribution<float> breakingToFalseTimeDistribution;
    };



    inline Promoter::Promoter(uint64_t gateMask, uint64_t proteinValues, Gate gate, BreakingState breakingState, float breakingToTrueRate, float breakingToFalseRate)
    : gateMask(gateMask)
    , proteinValues(proteinValues)
    , gate(gate)
    , breakingState(breakingState)
    , breakingToTrueTimeDistribution(1.0 / breakingToTrueRate)
    , breakingToFalseTimeDistribution(1.0 / breakingToFalseRate)
    {
    }

    inline Promoter::~Promoter()
    {
    }

    inline void Promoter::setBreakingState(BreakingState breakingState)
    {
        this->breakingState = breakingState;
    }

    inline uint64_t Promoter::getGateMask() const
    {
        return gateMask;
    }

    inline uint64_t Promoter::getProteinValues() const
    {
        return proteinValues;
    }

    inline Promoter::Gate Promoter::getGate() const
    {
        return gate;
    }

    inline Promoter::BreakingState Promoter::getBreakingState() const
    {
        return breakingState;
    }

    inline std::exponential_distribution<float> Promoter::getBreakingToTrueTimeDistribution() const
    {
        return breakingToTrueTimeDistribution;
    }

    inline std::exponential_distribution<float> Promoter::getBreakingToFalseTimeDistribution() const
    {
        return breakingToFalseTimeDistribution;
    }

    inline bool Promoter::check(uint64_t proteins) const
    {
        uint64_t input = proteinValues ^ ~proteins;
        uint64_t gateValue = (gateMask & input);

        switch(gate)
        {
            case OPEN:
                return true;
            case CLOSED:
                return false;
            case AND:
                return gateValue == gateMask;
            case NAND:
                return gateValue != gateMask;
            case OR:
                return gateValue != 0;
            case XOR:
                return gateValue != 0 && gateValue != gateMask;
            default:
                return false;
        }
    }

    inline Promoter::GateState Promoter::nextState(GateState gateState, uint64_t proteins) const
    {
        if(breakingState == Promoter::BreakingState::NOT_BROKEN)
            return GateState(((gateState & 1) << 1) | check(proteins));

        return GateState(((gateState & 1) << 1) | breakingState);
    }
}

#endif //CG_PROMOTER_HPP
