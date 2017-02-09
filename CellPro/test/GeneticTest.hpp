#ifndef GENETIC_TEST_HPP
#define GENETIC_TEST_HPP

#include "IMGUI_SFML.h"
#include <IMGUI/imgui.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

#include "cg/PlasmidPool.hpp"
#include "cg/Genome.hpp"

class GeneticTest
{
    public:
        GeneticTest(sf::Vector2i size, const std::string& name, bool verticalSync = true);
        virtual ~GeneticTest();

        void run();

    private:
        void load();
        void update();
        void updateGUI();
        bool proccessEvent(const sf::Event& event);
        void render() const;
        void destroy();
        void resetScene();

        sf::Window window;
        cg::PlasmidPool* plasmidPool;
        std::vector<cg::Genome*> genomes;
        bool reset;
        double simulationTime;
        unsigned int randomSeed;
};

inline GeneticTest::~GeneticTest()
{
    destroy();
}

#endif //GENETIC_TEST_HPP