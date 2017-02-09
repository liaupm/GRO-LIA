#ifndef CG_PLASMID_HPP
#define CG_PLASMID_HPP

#include <vector>
#include <string>

namespace cg
{
    class Operon;
    class Plasmid
    {
        public:
            Plasmid(const std::string& key, const std::string& name, const std::vector<const Operon*>& operons);
            virtual ~Plasmid();

            const std::string& getKey() const;
            const std::string& getName() const;
            const std::vector<const Operon*>& getOperons() const;

        private:
            std::string key;
            std::string name;
            std::vector<const Operon*> operons;
    };



    inline Plasmid::Plasmid(const std::string& key, const std::string& name, const std::vector<const Operon*>& operons)
    : key(key)
    , name(name)
    , operons(operons)
    {
    }

    inline Plasmid::~Plasmid()
    {
    }

    inline const std::string& Plasmid::getKey() const
    {
        return key;
    }

    inline const std::string& Plasmid::getName() const
    {
        return name;
    }

    inline const std::vector<const Operon*>& Plasmid::getOperons() const
    {
        return operons;
    }
}

#endif //CG_PLASMID_HPP
