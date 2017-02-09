#include "GeneticTest.hpp"

#include <iostream>

int main(int argc, char** argv)
{
    GeneticTest geneticTest(sf::Vector2i(1000, 800), "Genetic Test", true);
    geneticTest.run();

    return 0;
}

void GeneticTest::load()
{
    plasmidPool = new cg::PlasmidPool(randomSeed);

    {
        //  TEST 0: AND
        /* Proteins */
        const cg::Protein* p0 = plasmidPool->createProtein(14.0f, 1.0f);
        const cg::Protein* p1 = plasmidPool->createProtein(12.0f, 2.0f);
        const cg::Protein* p2 = plasmidPool->createProtein(10.0f, 4.0f);

        /* Genes */
        cg::Gene geneP0(p0, 15.0f, 1.0f);
        cg::Gene geneP1(p1, 10.0f, 2.0f);
        cg::Gene geneP2(p2, 13.0f, 1.5f);

        /* Promoters: OPEN(), AND(p1, p2) */
        cg::Promoter pTrue(0, 0, cg::Promoter::Gate::OPEN, cg::Promoter::BreakingState::NOT_BROKEN);

        float gateMask = p1->getID() | p2->getID();
        float proteinValues = p1->getID() | p2->getID();
        cg::Promoter pAnd(gateMask, proteinValues, cg::Promoter::Gate::AND, cg::Promoter::BreakingState::NOT_BROKEN);

        /* Operons */
        const cg::Operon* o0 = plasmidPool->createOperon(pTrue, {geneP1});
        const cg::Operon* o1 = plasmidPool->createOperon(pTrue, {geneP2});
        const cg::Operon* o2 = plasmidPool->createOperon(pAnd, {geneP0});

        /* Plasmids */
        const cg::Plasmid* plasmid0 = plasmidPool->createPlasmid({o0, o1, o2});

        /* Genomes */
        cg::Genome* genome0 = new cg::Genome(plasmidPool);
        genome0->add(plasmid0);

        genomes.push_back(genome0);
    }

    {
        //  TEST 1: OSCILATOR
        /* Proteins */
        const cg::Protein* p0 = plasmidPool->createProtein(5.0f, 0.0f);
        const cg::Protein* p1 = plasmidPool->createProtein(5.0f, 0.0f);

        /* Genes */
        cg::Gene geneP0(p0, 10.0f, 0.0f);
        cg::Gene geneP1(p1, 10.0f, 0.0f);

        /* Promoters */
        cg::Promoter pYesP0(p0->getID(), p0->getID(), cg::Promoter::Gate::AND, cg::Promoter::BreakingState::NOT_BROKEN); // YES(P0)
        cg::Promoter pNotP1(p1->getID(), !p1->getID(), cg::Promoter::Gate::AND, cg::Promoter::BreakingState::NOT_BROKEN); // YES(¬P1)

        /* Operons */
        const cg::Operon* o0 = plasmidPool->createOperon(pYesP0, {geneP1}); //P1 = YES(¬P0)
        const cg::Operon* o1 = plasmidPool->createOperon(pNotP1, {geneP0}); //P2 = YES(¬P1)

        /* Plasmid */
        const cg::Plasmid* plasmid = plasmidPool->createPlasmid({o0, o1});

        /* Genome */
        cg::Genome* genome = new cg::Genome(plasmidPool);
        genome->add(plasmid);

        genomes.push_back(genome);
    }

    {
        //  TEST 2: REPRESSILATOR
        /* Proteins */
        const cg::Protein* p0 = plasmidPool->createProtein(5.0f, 1.5f);
        const cg::Protein* p1 = plasmidPool->createProtein(6.0f, 1.0f);
        const cg::Protein* p2 = plasmidPool->createProtein(5.5f, 1.0f);

        /* Genes */
        cg::Gene geneP0(p0, 12.0f, 1.0f);
        cg::Gene geneP1(p1, 10.0f, 2.0f);
        cg::Gene geneP2(p2, 10.2f, 1.5f);

        /* Promoters */
        cg::Promoter pNotP0(p0->getID(), !p0->getID(), cg::Promoter::Gate::AND, cg::Promoter::BreakingState::NOT_BROKEN); // YES(¬P0)
        cg::Promoter pNotP1(p1->getID(), !p1->getID(), cg::Promoter::Gate::AND, cg::Promoter::BreakingState::NOT_BROKEN); // YES(¬P1)
        cg::Promoter pNotP2(p2->getID(), !p2->getID(), cg::Promoter::Gate::AND, cg::Promoter::BreakingState::NOT_BROKEN); // YES(¬P2)

        /* Operons */
        const cg::Operon* o0 = plasmidPool->createOperon(pNotP0, {geneP1}); //P1 = YES(¬P0)
        const cg::Operon* o1 = plasmidPool->createOperon(pNotP1, {geneP2}); //P2 = YES(¬P1)
        const cg::Operon* o2 = plasmidPool->createOperon(pNotP2, {geneP0}); //P0 = YES(¬P2)

        /* Plasmid */
        const cg::Plasmid* plasmid = plasmidPool->createPlasmid({o0, o1, o2});

        /* Genome */
        cg::Genome* genome = new cg::Genome(plasmidPool);
        genome->add(plasmid);

        genomes.push_back(genome);
    }

    {
        //  TEST 3: BREAKING PROMOTER
        /* Proteins */
        const cg::Protein* p0 = plasmidPool->createProtein(10.0f, 1.0f);

        /* Genes */
        cg::Gene geneP0(p0, 8.0f, 0.0f);

        /* Promoters */
        cg::Promoter yesP0(p0->getID(), p0->getID(), cg::Promoter::Gate::AND, cg::Promoter::BreakingState::NOT_BROKEN, 2.0f, 2.0f); // YES(P0)

        /* Operons */
        const cg::Operon* o0 = plasmidPool->createOperon(yesP0, {geneP0}); //P0 = YES(P0)

        /* Plasmid */
        const cg::Plasmid* plasmid = plasmidPool->createPlasmid({o0});

        /* Genome */
        cg::Genome* genome = new cg::Genome(plasmidPool);
        genome->add(plasmid);

        genomes.push_back(genome);
    }

    {
        //  TEST 4: SEVERAL GENES BY OPERATOR
        /* Proteins */
        const cg::Protein* p0 = plasmidPool->createProtein(10.0f, 0.0f);
        const cg::Protein* p1 = plasmidPool->createProtein(10.0f, 0.0f);
        const cg::Protein* p2 = plasmidPool->createProtein(10.0f, 0.0f);

        /* Genes */
        cg::Gene geneP1(p1, 8.0f, 0.0f);
        cg::Gene geneP2(p2, 8.0f, 0.0f);

        /* Promoters */
        cg::Promoter pNotP0(p0->getID(), p0->getID(), cg::Promoter::Gate::NAND, cg::Promoter::BreakingState::NOT_BROKEN); // NOT(¬P0)

        /* Operons */
        const cg::Operon* o0 = plasmidPool->createOperon(pNotP0, {geneP1, geneP2}); //P1, P2 = NOT(¬P0)
        /* Plasmid */
        const cg::Plasmid* plasmid = plasmidPool->createPlasmid({o0});

        /* Genome */
        cg::Genome* genome = new cg::Genome(plasmidPool);
        genome->add(plasmid);

        genomes.push_back(genome);
    }

    {
        // TEST 5: (by Marcos)
        /* Proteins */
        const cg::Protein* rapx = plasmidPool->createProtein(31.1f, 0.0f, "rapx VERDE_OSCURO");
        const cg::Protein* tet = plasmidPool->createProtein(330.1f, 0.0f, "tet");
        const cg::Protein* lac = plasmidPool->createProtein(31.1f, 0.0f, "lac ROJO");
        const cg::Protein* ara = plasmidPool->createProtein(31.1f, 0.0f, "ara VERDE");

        /* Operons */
        cg::Promoter px1(ara->getID(), !ara->getID(), cg::Promoter::Gate::AND, cg::Promoter::BreakingState::NOT_BROKEN);
        const cg::Operon* opPx1 = plasmidPool->createOperon(px1, {cg::Gene(rapx, 5.1f, 0.0f)});

        cg::Promoter ps0(0, 0, cg::Promoter::Gate::OPEN, cg::Promoter::BreakingState::NOT_BROKEN);
        const cg::Operon* opPs0 = plasmidPool->createOperon(ps0, {cg::Gene(tet, 50.1f, 0.0f)});

        cg::Promoter px2(tet->getID(), !tet->getID(), cg::Promoter::Gate::AND, cg::Promoter::BreakingState::NOT_BROKEN);
        const cg::Operon* opPx2 = plasmidPool->createOperon(px2, {cg::Gene(lac, 5.0f, 0.0f)});

        cg::Promoter px3(rapx->getID() | lac->getID(), rapx->getID() | lac->getID(), cg::Promoter::Gate::OR, cg::Promoter::BreakingState::NOT_BROKEN);
        const cg::Operon* opPx3 = plasmidPool->createOperon(px3, {cg::Gene(ara, 10.0f, 0.0f)});

        const cg::Plasmid* plasmid0 = plasmidPool->createPlasmid({opPx1, opPs0, opPx2, opPx3});

        /* Genome */
        cg::Genome* genome = new cg::Genome(plasmidPool);
        genome->add(plasmid0);

        genomes.push_back(genome);
    }

    {
        //  TEST 6: XOR
        /* Proteins */
        const cg::Protein* p0 = plasmidPool->createProtein(14.0f, 1.0f);
        const cg::Protein* p1 = plasmidPool->createProtein(12.0f, 2.0f);
        const cg::Protein* p2 = plasmidPool->createProtein(10.0f, 4.0f);

        /* Genes */
        cg::Gene geneP0(p0, 15.0f, 1.0f);
        cg::Gene geneP1(p1, 10.0f, 2.0f);
        cg::Gene geneP2(p2, 13.0f, 1.5f);

        /* Promoters: TRUE(), XOR(p1, p2) */
        cg::Promoter pTrue(0, 0, cg::Promoter::Gate::OPEN, cg::Promoter::BreakingState::NOT_BROKEN);

        float gateMask = p1->getID() | p2->getID();
        float proteinValues = p1->getID() | p2->getID();
        cg::Promoter pXor(gateMask, proteinValues, cg::Promoter::Gate::XOR, cg::Promoter::BreakingState::NOT_BROKEN);

        /* Operons */
        const cg::Operon* o0 = plasmidPool->createOperon(pTrue, {geneP1});
        const cg::Operon* o1 = plasmidPool->createOperon(pTrue, {geneP2});
        const cg::Operon* o2 = plasmidPool->createOperon(pXor, {geneP0});

        /* Plasmids */
        const cg::Plasmid* plasmid0 = plasmidPool->createPlasmid({o0, o1, o2});

        /* Genomes */
        cg::Genome* genome0 = new cg::Genome(plasmidPool);
        genome0->add(plasmid0);

        genomes.push_back(genome0);
    }
}

void GeneticTest::update()
{
    simulationTime += 0.1f;

    for(unsigned int i = 0; i < genomes.size(); i++)
        genomes[i]->update(simulationTime, 0.1f);
}

void GeneticTest::updateGUI()
{
    ImGui::SetNextWindowPos(ImVec2(5.0f, 5.0f));
    ImGui::Begin("Plasmid Pool", nullptr, ImVec2(350.0f, window.getSize().y - 5), 0.4f, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize);

    if(ImGui::CollapsingHeader("Proteins"))
    {
        for(std::map<uint64_t, const cg::Protein*>::const_iterator it = plasmidPool->getProteins().begin(); it != plasmidPool->getProteins().end(); ++it)
            if(ImGui::TreeNode((void*)it->second, "%s", it->second->getKey().c_str()))
            {
                ImGui::BulletText("Name: %s", it->second->getName().c_str());
                ImGui::BulletText("ID: %lu", it->second->getID());
                ImGui::BulletText("Degradation time: (%.3f, %.3f)", it->second->getDegradationTimeDistribution().mean(), it->second->getDegradationTimeDistribution().stddev());
                ImGui::TreePop();
            }
        ImGui::TextColored(ImVec4(0.0f, 0.35f, 1.0f, 1.0f), " [%lu]", plasmidPool->getProteins().size());
    }

    if(ImGui::CollapsingHeader("Operons"))
    {
        for(std::map<std::string, const cg::Operon*>::const_iterator it = plasmidPool->getOperons().begin(); it != plasmidPool->getOperons().end(); ++it)
            if(ImGui::TreeNode((void*)it->second, "%s", it->second->getKey().c_str()))
            {
                ImGui::BulletText("Name: %s", it->second->getName().c_str());
                if(ImGui::TreeNodeEx((void*)it->second, ImGuiTreeNodeFlags_DefaultOpen, "Promoter"))
                {
                    ImGui::BulletText("Gate: %u", it->second->getPromoter().getGate());
                    ImGui::BulletText("Gate mask: %lo", it->second->getPromoter().getGateMask());
                    ImGui::BulletText("Protein values: %lo", it->second->getPromoter().getProteinValues());
                    ImGui::BulletText("Breaking state: %u", it->second->getPromoter().getBreakingState());
                    ImGui::BulletText("Breaking to true (lambda): %.4f", it->second->getPromoter().getBreakingToTrueTimeDistribution().lambda());
                    ImGui::BulletText("Breaking to false (lambda): %.4f", it->second->getPromoter().getBreakingToFalseTimeDistribution().lambda());
                    ImGui::TreePop();
                }

                const std::vector<cg::Gene>& genes = it->second->getGenes();
                for(unsigned int g = 0; g < genes.size(); g++)
                {
                    if(ImGui::TreeNodeEx((void*)&genes[g], ImGuiTreeNodeFlags_DefaultOpen, "%s", genes[g].getProtein()->getKey().c_str()))
                    {
                        ImGui::BulletText("Activation time: (%.3f, %.3f)", genes[g].getActivationTimeDistribution().mean(), genes[g].getActivationTimeDistribution().stddev());
                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }
        ImGui::TextColored(ImVec4(0.0f, 0.35f, 1.0f, 1.0f), " [%lu]", plasmidPool->getOperons().size());
    }

    if(ImGui::CollapsingHeader("Plasmids"))
    {
        for(std::map<std::string, const cg::Plasmid*>::const_iterator it = plasmidPool->getPlasmids().begin(); it != plasmidPool->getPlasmids().end(); ++it)
            if(ImGui::TreeNode((void*)it->second, "%s", it->second->getKey().c_str()))
            {
                ImGui::BulletText("Name: %s", it->second->getName().c_str());

                ImGui::TreePop();
            }
        ImGui::TextColored(ImVec4(0.0f, 0.35f, 1.0f, 1.0f), " [%lu]", plasmidPool->getPlasmids().size());
    }

    ImGui::End();


    ImGui::SetNextWindowPos(ImVec2(365.0f, 5.0f));
    ImGui::Begin("Genomes", nullptr, ImVec2(350.0f, window.getSize().y - 5), 0.4f, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize);
    for(unsigned int i = 0; i < genomes.size(); i++)
        if(ImGui::TreeNode((void*)&genomes[i], "Genome: %u", i))
        {
            if(ImGui::TreeNodeEx((void*)&genomes[i]->getPlasmids(), ImGuiTreeNodeFlags_DefaultOpen, "Plasmids"))
            {
                const std::vector<const cg::Plasmid*>& plasmids = genomes[i]->getPlasmids();
                for(unsigned int p = 0; p < plasmids.size(); p++)
                {
                    std::string name = !plasmids[p]->getName().empty() ? "(" + plasmids[p]->getName() + ")" : "";
                    ImGui::BulletText("%s %s", plasmids[p]->getKey().c_str(), name.c_str());
                }
                ImGui::TreePop();
            }

            if(ImGui::TreeNodeEx((void*)&genomes[i]->getPromoterStates(), ImGuiTreeNodeFlags_DefaultOpen, "Promoter states"))
            {
                const std::vector<cg::Genome::PromoterState>& promoterStates = genomes[i]->getPromoterStates();
                for(unsigned int p = 0; p < promoterStates.size(); p++)
                {
                    std::string name = !promoterStates[p].operon->getName().empty() ? "(" + promoterStates[p].operon->getName() + ")" : "";
                    if(ImGui::TreeNode((void*)&promoterStates[p],"%s %s", promoterStates[p].operon->getKey().c_str(), name.c_str()))
                    {
                        ImGui::BulletText("Gate state: %u", promoterStates[p].gateState);
                        ImGui::BulletText("Breaking time: %.2f", promoterStates[p].breakingTime);
                        ImGui::BulletText("Breaking value: %u", promoterStates[p].breakingValue);
                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }

            if(ImGui::TreeNodeEx((void*)&genomes[i]->getGeneActivations(), ImGuiTreeNodeFlags_DefaultOpen, "Gene activations"))
            {
                const std::vector<cg::Genome::GeneActivation>& geneActivations = genomes[i]->getGeneActivations();
                for(unsigned int p = 0; p < geneActivations.size(); p++)
                {
                    std::string name = !geneActivations[p].gene->getProtein()->getName().empty() ? "(" + geneActivations[p].gene->getProtein()->getName() + ")" : "";
                    if(ImGui::TreeNode((void*)&geneActivations[p],"%s %s", geneActivations[p].gene->getProtein()->getKey().c_str(), name.c_str()))
                    {
                        ImGui::BulletText("Activation time: %.2f", geneActivations[p].activationTime);
                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }

            if(ImGui::TreeNodeEx((void*)&genomes[i]->getProteinDegradations(), ImGuiTreeNodeFlags_DefaultOpen, "Protein degradations"))
            {
                const std::vector<cg::Genome::ProteinDegradation>& proteinDegradations = genomes[i]->getProteinDegradations();
                for(unsigned int p = 0; p < proteinDegradations.size(); p++)
                {
                    std::string name = !proteinDegradations[p].protein->getName().empty() ? "(" + proteinDegradations[p].protein->getName() + ")" : "";
                    if(ImGui::TreeNode((void*)&proteinDegradations[p],"%s %s", proteinDegradations[p].protein->getKey().c_str(), name.c_str()))
                    {
                        ImGui::BulletText("Degradation time: %.2f", proteinDegradations[p].degradationTime);
                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }

            std::vector<const cg::Protein*> proteins = plasmidPool->getProteins(genomes[i]->getProteins());
            if(ImGui::TreeNodeEx((void*)&proteins, ImGuiTreeNodeFlags_DefaultOpen, "Proteins"))
            {
                for(unsigned int p = 0; p < proteins.size(); p++)
                {
                    std::string name = !proteins[p]->getName().empty() ? "(" + proteins[p]->getName() + ")" : "";
                    ImGui::BulletText("%s %s", proteins[p]->getKey().c_str(), name.c_str());
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }

    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(740, 5));
    ImGui::Begin("Simulation time", nullptr, ImVec2(100.0f, 30), 0.4f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Simulation time: %.2f s", simulationTime);
    ImGui::Separator();

    if(ImGui::Button("1 x 0.1s"))
        update();
    ImGui::SameLine();
    if(ImGui::Button("10 x 0.1s"))
        for(unsigned int i = 0; i < 10; i++)
            update();
    ImGui::SameLine();
    if(ImGui::Button("50 x 0.1s"))
        for(unsigned int i = 0; i < 50; i++)
            update();

    if(ImGui::Button("Reset"))
        reset = true;
    ImGui::SameLine();
    if(ImGui::Button(("Reset with seed: (" + std::to_string(randomSeed) + ")").c_str()))
    {
        reset = true;
        randomSeed++;
    }

    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(725, 100));
    ImGui::Begin("Plasmid Manager", nullptr, ImVec2(260.0f, window.getSize().y - 100), 0.4f, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize);


    if(ImGui::CollapsingHeader("Create proteins"))
    {
        static char name[64] = "";
        static float mean = 0.0;
        static float deviance = 0.0;
        ImGui::InputText("Name", name, 64);
        ImGui::Text("DegradationTime: ");
        ImGui::InputFloat("Mean", &mean);
        ImGui::InputFloat("Deviance", &deviance);
        if(ImGui::Button("    Create    "))
        {
            plasmidPool->createProtein(mean, deviance, name);
            name[0] = '\0';
            mean = 0.0f;
            deviance = 0.0f;
        }
    }

    if(ImGui::CollapsingHeader("Create operons"))
    {

    }

    if(ImGui::CollapsingHeader("Create plasmids"))
    {

    }

    if(ImGui::CollapsingHeader("Add/Remove plasmids to genomes", ImGuiTreeNodeFlags_DefaultOpen))
    {
        static int genomeIndex = -1;
        static char plasmidKey[128] = "";
        ImGui::Text("Genome index:");
        ImGui::InputInt(" ", &genomeIndex);
        ImGui::Text("Plasmid key:");
        ImGui::InputText("  ", plasmidKey, 128);
        if(ImGui::Button("    Add    "))
        {
            if(genomeIndex < static_cast<int>(genomes.size()))
            {
                const cg::Plasmid* plasmid = plasmidPool->getPlasmid(plasmidKey);
                if(plasmid)
                    genomes[genomeIndex]->add(plasmid);
            }
        }
        ImGui::SameLine();
        if(ImGui::Button("    Remove    "))
        {
            if(genomeIndex < static_cast<int>(genomes.size()))
            {
                const cg::Plasmid* plasmid = plasmidPool->getPlasmid(plasmidKey);
                if(plasmid)
                    genomes[genomeIndex]->remove(plasmid);
            }
        }
    }

    ImGui::End();

    //ImGui::ShowTestWindow(nullptr);
}

bool GeneticTest::proccessEvent(const sf::Event& event)
{
    return true;
}

void GeneticTest::render() const
{

}

GeneticTest::GeneticTest(sf::Vector2i size, const std::string& name, bool verticalSync)
: plasmidPool(nullptr)
, reset(false)
, simulationTime(0.0f)
, randomSeed(0)
{
    //Window
    window.create(sf::VideoMode(size.x, size.y), name, sf::Style::Close, sf::ContextSettings(24));
    window.setVerticalSyncEnabled(verticalSync);

    //OpenGL
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

void GeneticTest::run()
{
    //Main loop
    ImGui::SFML::Init(window);
    ImGui::GetIO().IniFilename = NULL;

    ImGui::PushStyleColor(ImGuiCol_Button, ImColor(0.2f, 0.2f, 0.5f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(0.2f, 0.2f, 0.65f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor(0.2f, 0.2f, 0.35f));

    load();

    sf::Clock elapsed;
    bool running = true;
    while(running)
    {
        float dt = elapsed.restart().asSeconds();
        if(reset)
            resetScene();

        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                running = false;
            else if(event.type == sf::Event::Resized)
            {
                glViewport(0.0f, 0.0f, event.size.width, event.size.height);
            }

            ImGui::SFML::ProcessEvent(event);
            running &= proccessEvent(event);
        }

        ImGui::SFML::Update(window, dt);
        updateGUI();

        glClear(GL_COLOR_BUFFER_BIT);

        render();
        ImGui::Render();

        window.display();
    }

    ImGui::SFML::Shutdown();
}

void GeneticTest::destroy()
{
    if(plasmidPool != nullptr)
        delete plasmidPool;

    for(unsigned int i = 0; i < genomes.size(); i++)
        delete genomes[i];
}

void GeneticTest::resetScene()
{
    reset = false;
    simulationTime = 0.0f;
    genomes.clear();
    destroy();
    load();
}

