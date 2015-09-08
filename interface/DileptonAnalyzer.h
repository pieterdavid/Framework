#ifndef DILEPTONANALYZER_H
#define DILEPTONANALYZER_H

#include <cp3_llbb/Framework/interface/Analyzer.h>
#include <cp3_llbb/Framework/interface/Category.h>

class DileptonAnalyzer: public Framework::Analyzer {
    public:
        DileptonAnalyzer(const std::string& name, const ROOT::TreeGroup& tree_, const edm::ParameterSet& config):
            Analyzer(name, tree_, config) {

        }

        virtual void analyze(const edm::Event&, const edm::EventSetup&, const ProducersManager&, const CategoryManager&) override;

        virtual void registerCategories(CategoryManager& manager) override;

        BRANCH(dileptons_mumu, std::vector<LorentzVector>);
        BRANCH(dileptons_elel, std::vector<LorentzVector>);
        BRANCH(dileptons_elmu, std::vector<LorentzVector>);
        BRANCH(dileptons_muel, std::vector<LorentzVector>);
        BRANCH(dileptons_mumu_indices, std::vector<std::pair<unsigned int,unsigned int>>);
        BRANCH(dileptons_elel_indices, std::vector<std::pair<unsigned int,unsigned int>>);
        BRANCH(dileptons_elmu_indices, std::vector<std::pair<unsigned int,unsigned int>>);
        BRANCH(dileptons_muel_indices, std::vector<std::pair<unsigned int,unsigned int>>);

    private:
        // empty
};


#endif
