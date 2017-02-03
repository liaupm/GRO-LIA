#lineWidth: 1

[PlasmidPool|
- RandomEngine randomEngine
- std::map<std::string, const Plasmid*> plasmids
- std::map<std::string, const Operon*> operons
- std::map<uint64_t, const Protein*> proteins
|
+ PlasmidPool(unsigned int randomSeed)
+ const Plasmid* createPlasmid(const std::vector<const Operon*>& operons)
+ const Operon* createOperon(const Promoter&, const std::vector<Gene>& genes)
+ const Protein* createProtein(float degradationTimeMean, float degradationTimeDeviation, const std::string& name)
+ const Operon* mutatePromoter(const Operon* operon, Promoter::BreakingState breakingState)
+ const Operon* mutateGenes(const Operon* operon, const std::vector<Gene>& genes)
+ const Plasmid* getPlasmid(const std::string& key) const
+ const Operon* getOperon(const std::string& key) const
+ const Protein* getProtein(uint64_t id) const
+ const Plasmid* getPlasmidByName(const std::string& name) const
+ const Operon* getOperonByName(const std::string& name) const
+ const Protein* getProteinByName(const std::string& name) const
+ const std::map<std::string, const Plasmid*>& getPlasmids() const
+ const std::map<std::string, const Operon*>& getOperons() const
+ const std::map<uint64_t, const Protein*>& getProteins() const
+ std::vector<const Protein*> getProteins(uint64_t proteinIDs) const
+ RandomEngine& getRandomEngine()
]

[Plasmid|
- std::string key
- std::string name
- std::vector<const Operon*> operons
|
+ Plasmid(const std::string& key, const std::vector<const Operon*>& operons)
+ const std::string& getKey() const
+ const std::string& getName() const
+ const std::vector<const Operon*>& getOperons() const
]

[Operon|
- std::string key
- std::string name
- Promoter promoter
- std::vector<Gene> genes
|
+ Operon(const std::string& key, const Promoter& promoter, const std::vector<Gene>& genes)
+ const std::string& getKey() const
+ const std::string& getName() const
+ const Promoter& getPromoter() const
+ const std::vector<Gene>& getGenes() const
]

[Promoter|
- uint64_t gateMask
- muint64_t proteinValues
- Gate gate
- BreakingState breakingState
- std::exponential_distribution<float> breakingToTrueTimeDistribution
- std::exponential_distribution<float> breakingToFalseTimeDistribution
|
+ Promoter(uint64_t gateMask, uint64_t proteinValues, Gate gate, BreakingState breakingState, float breakingToTrueRate, float breakingToFalseRate)
+ uint64_t getGateMask() const
+ uint64_t getProteinValues() const
+ Gate getGate() const
+ void setBreakingState(BreakingState breakingState)
+ BreakingState getBreakingState() const
+ std::exponential_distribution<float> getBreakingToTrueTimeDistribution() const
+ std::exponential_distribution<float> getBreakingToFalseTimeDistribution() const
+ bool check(uint64_t proteins) const
+ GateState nextState(GateState gateState, uint64_t proteins) const
]

[Gene|
- const Protein* protein
- std::normal_distribution<float> activationTimeDistribution
|
+ Gene(const Protein* protein, float activationTimeMean, float activationTimeDeviation)
+ const Protein* getProtein() const
+ std::normal_distribution<float> getActivationTimeDistribution() const
]

[Protein|
+ std::string key;
+ uint64_t id;
+ std::string name;
+ std::normal_distribution<float> degradationTimeDistribution
|
+ Protein(unsigned int index, const std::string& name, float degradationTimeMean, float degradationTimeDeviation)
+ virtual ~Protein()
+ const std::string& getKey() const;
+ uint64_t getID() const
+ const std::string& getName() const;
+ std::normal_distribution<float> getDegradationTimeDistribution() const
]

[Genome|
- PlasmidPool* plasmidPool;
- std::vector<const Plasmid*> plasmids
- std::vector<PromoterState> promoterStates
- std::vector<GeneActivations> geneActivations
- std::vector<ProteinDegradation> proteinDegradations
- uint64_t proteins
|
+ Genome(PlasmidPool* plasmidPool)
+ Genome(Genome& genome, bool mutate)
+ void add(const Plasmid* plasmid)
+ void remove(const Plasmid* plasmid)
+ void update(float simulationTime)
+ PlasmidPool* getPlasmidPool() const
+ const std::vector<const Plasmid*>& getPlasmids() const;
+ const std::vector<PromoterState>& getPromoterStates() const
+ const std::vector<GeneActivation>& getGeneActivations() const
+ const std::vector<ProteinDegradation>& getProteinDegradations() const
+ uint64_t getProteins() const
- void activateOperon(const Operon* operon)
- void deactivateOperon(const Operon* operon)
- void deactivateGene(const Gene* gene)
- void expressGene(unsigned int index)
- void degradateProtein(unsigned int* index)
- void computeMutations(float simulationTime)
]

[PromoterState|
- const Operon* operon
- GateState(enum) gateState
- float breakingTime
- bool breakingValue
|
PromoterState(const Operon* operon)
void computeBreakingEvent(float simulationTime, RandomEngine& randomEngine)
]

[GeneActivation|
- const Gene* gene
- float activationTime
|
GeneActivation(const Gene* gene, float time, RandomEngine& rng)
]

[ProteinDegradation|
- const Protein* protein
- float degradationTime
|
ProteinDegradation(const Protein* protein, float time, RandomEngine& rng)
]

[PlasmidPool]1+--0..*[Plasmid]
[PlasmidPool]1+--0..*[Operon]
[PlasmidPool]1+--0..*[Protein]

[Genome]+-0..*[PromoterState]
[Genome]+-0..*[GeneActivation]
[Genome]+-0..*[ProteinDegradation]
[Genome]-->0..*[Plasmid]
[Genome]->[PlasmidPool]

[Plasmid]-->0..*[Operon]
[Operon]+-[Promoter]
[Operon]+-0..*[Gene]
[Gene]-->[Protein]

[PromoterState]-->[Operon]
[GeneActivation]-->[Gene]
[ProteinDegradation]-->[Protein]
