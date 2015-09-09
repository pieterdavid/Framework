#include <cp3_llbb/Framework/interface/MuonsProducer.h>
#include <cp3_llbb/Framework/interface/ElectronsProducer.h>
#include <cp3_llbb/Framework/interface/JetsProducer.h>

#include <cp3_llbb/Framework/interface/DileptonCategories.h>
#include <cp3_llbb/Framework/interface/DileptonAnalyzer.h>

// ***** ***** *****
// Dilepton Mu-Mu category
// ***** ***** *****
bool MuMuCategory::event_in_category_pre_analyzers(const ProducersManager& producers) const {
    const MuonsProducer& muons = producers.get<MuonsProducer>("muons");
    return muons.p4.size() >= 2 ;
};

bool MuMuCategory::event_in_category_post_analyzers(const ProducersManager& producers, const AnalyzersManager& analyzers) const {
    const DileptonAnalyzer& dilepton_analyzer = analyzers.get<DileptonAnalyzer>("dilepton");
    return dilepton_analyzer.dileptons_mumu.size() > 0;
};

void MuMuCategory::register_cuts(CutManager& manager) {
    manager.new_cut("ll_mass", "mll > 20");
};

void MuMuCategory::evaluate_cuts_post_analyzers(CutManager& manager, const ProducersManager& producers, const AnalyzersManager& analyzers) const {
    const DileptonAnalyzer& dilepton_analyzer = analyzers.get<DileptonAnalyzer>("dilepton");
    for(unsigned int idilepton = 0; idilepton < dilepton_analyzer.dileptons_mumu.size(); idilepton++)
        if( dilepton_analyzer.dileptons_mumu[idilepton].M() > m_mll_cut)
        {
            manager.pass_cut("ll_mass");
            break;
        }
}

// ***** ***** *****
// Dilepton Mu-E category
// ***** ***** *****
bool MuElCategory::event_in_category_pre_analyzers(const ProducersManager& producers) const {
    const MuonsProducer& muons = producers.get<MuonsProducer>("muons");
    const ElectronsProducer& electrons = producers.get<ElectronsProducer>("electrons");
    return (muons.p4.size() >= 1) && (electrons.p4.size() >= 1);
};

bool MuElCategory::event_in_category_post_analyzers(const ProducersManager& producers, const AnalyzersManager& analyzers) const {
    const DileptonAnalyzer& dilepton_analyzer = analyzers.get<DileptonAnalyzer>("dilepton");
    return dilepton_analyzer.dileptons_muel.size() > 0;
};

void MuElCategory::register_cuts(CutManager& manager) {
    manager.new_cut("ll_mass", "mll > 20");
};

void MuElCategory::evaluate_cuts_post_analyzers(CutManager& manager, const ProducersManager& producers, const AnalyzersManager& analyzers) const {
    const DileptonAnalyzer& dilepton_analyzer = analyzers.get<DileptonAnalyzer>("dilepton");
    for(unsigned int idilepton = 0; idilepton < dilepton_analyzer.dileptons_muel.size(); idilepton++)
        if( dilepton_analyzer.dileptons_muel[idilepton].M() > m_mll_cut)
        {
            manager.pass_cut("ll_mass");
            break;
        }
}

// ***** ***** *****
// Dilepton E-Mu category
// ***** ***** *****
bool ElMuCategory::event_in_category_pre_analyzers(const ProducersManager& producers) const {
    const MuonsProducer& muons = producers.get<MuonsProducer>("muons");
    const ElectronsProducer& electrons = producers.get<ElectronsProducer>("electrons");
    return (muons.p4.size() >= 1) && (electrons.p4.size() >= 1);
};

bool ElMuCategory::event_in_category_post_analyzers(const ProducersManager& producers, const AnalyzersManager& analyzers) const {
    const DileptonAnalyzer& dilepton_analyzer = analyzers.get<DileptonAnalyzer>("dilepton");
    return dilepton_analyzer.dileptons_elmu.size() > 0;
};

void ElMuCategory::register_cuts(CutManager& manager) {
    manager.new_cut("ll_mass", "mll > 20");
};

void ElMuCategory::evaluate_cuts_post_analyzers(CutManager& manager, const ProducersManager& producers, const AnalyzersManager& analyzers) const {
    const DileptonAnalyzer& dilepton_analyzer = analyzers.get<DileptonAnalyzer>("dilepton");
    for(unsigned int idilepton = 0; idilepton < dilepton_analyzer.dileptons_elmu.size(); idilepton++)
        if( dilepton_analyzer.dileptons_elmu[idilepton].M() > m_mll_cut)
        {
            manager.pass_cut("ll_mass");
            break;
        }
}

// ***** ***** *****
// Dilepton El-El category
// ***** ***** *****
bool ElElCategory::event_in_category_pre_analyzers(const ProducersManager& producers) const {
    const ElectronsProducer& electrons = producers.get<ElectronsProducer>("electrons");
    return electrons.p4.size() >= 2;
};

bool ElElCategory::event_in_category_post_analyzers(const ProducersManager& producers, const AnalyzersManager& analyzers) const {
    const DileptonAnalyzer& dilepton_analyzer = analyzers.get<DileptonAnalyzer>("dilepton");
    return dilepton_analyzer.dileptons_elel.size() > 0;
};

void ElElCategory::register_cuts(CutManager& manager) {
    manager.new_cut("ll_mass", "mll > 20");
};

void ElElCategory::evaluate_cuts_post_analyzers(CutManager& manager, const ProducersManager& producers, const AnalyzersManager& analyzers) const {
    const DileptonAnalyzer& dilepton_analyzer = analyzers.get<DileptonAnalyzer>("dilepton");
    for(unsigned int idilepton = 0; idilepton < dilepton_analyzer.dileptons_elel.size(); idilepton++)
        if( dilepton_analyzer.dileptons_elel[idilepton].M() > m_mll_cut)
        {
            manager.pass_cut("ll_mass");
            break;
        }
}

