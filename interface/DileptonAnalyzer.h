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

        BRANCH(dimuons, std::vector<LorentzVector>);
        BRANCH(dielectrons, std::vector<LorentzVector>);
        BRANCH(dileptons_emu, std::vector<LorentzVector>);
        BRANCH(dileptons_mue, std::vector<LorentzVector>);
        BRANCH(dimuons_indices, std::vector<std::pair<unsigned int,unsigned int>>);
        BRANCH(dielectrons_indices, std::vector<std::pair<unsigned int,unsigned int>>);
        BRANCH(dileptons_emu_indices, std::vector<std::pair<unsigned int,unsigned int>>);
        BRANCH(dileptons_mue_indices, std::vector<std::pair<unsigned int,unsigned int>>);

    private:
        // empty
};


#endif
