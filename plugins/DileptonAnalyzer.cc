#include <cp3_llbb/Framework/interface/DileptonAnalyzer.h>
#include <cp3_llbb/Framework/interface/DileptonCategories.h>

#include <cp3_llbb/Framework/interface/LeptonsProducer.h>
#include <cp3_llbb/Framework/interface/ElectronsProducer.h>
#include <cp3_llbb/Framework/interface/MuonsProducer.h>

void DileptonAnalyzer::analyze(const edm::Event& event, const edm::EventSetup&, const ProducersManager& producers, const AnalyzersManager& analyzers) {

// ***** ***** *****
// Get all dilepton objects out of the event
// ***** ***** *****
    const MuonsProducer& muons = producers.get<MuonsProducer>("muons");
    const ElectronsProducer& electrons = producers.get<ElectronsProducer>("electrons");

    // Dimuons
    for( unsigned int imuon = 0 ; imuon < muons.p4.size() ; imuon++ )
    {
        if(muons.isLoose[imuon])
        {
            for( unsigned int jmuon = imuon+1 ; jmuon < muons.p4.size() ; jmuon++ )
            {
                if( muons.isLoose[jmuon] && muons.charge[imuon] * muons.charge[jmuon] < 0 )
                {
                    LorentzVector dimuon = muons.p4[imuon] + muons.p4[jmuon];
                    dimuons.push_back(dimuon);
                    dimuons_indices.push_back(std::make_pair(imuon, jmuon));
                }
            }
        }
    }

    // Dielectrons
    for( unsigned int ielectron = 0 ; ielectron < electrons.p4.size() ; ielectron++ )
    {
        if(electrons.ids[ielectron]["cutBasedElectronID-Spring15-50ns-V1-standalone-loose"])
        {
            for( unsigned int jelectron = ielectron+1 ; jelectron < electrons.p4.size() ; jelectron++ )
            {
                if( electrons.ids[jelectron]["cutBasedElectronID-Spring15-50ns-V1-standalone-loose"] && electrons.charge[ielectron] * electrons.charge[jelectron] < 0 )
                {
                    LorentzVector dielectron = electrons.p4[ielectron] + electrons.p4[jelectron];
                    dielectrons.push_back(dielectron);
                    dielectrons_indices.push_back(std::make_pair(ielectron, jelectron));
                }
            }
        }
    }

    //Dilepton electron-muon
    for( unsigned int ielectron = 0 ; ielectron < electrons.p4.size() ; ielectron++ )
    {
        if(electrons.ids[ielectron]["cutBasedElectronID-Spring15-50ns-V1-standalone-loose"])
        {
            for( unsigned int jmuon = 0 ; jmuon < muons.p4.size() ; jmuon++ )
            {
                if( muons.p4[jmuon].pt() < electrons.p4[ielectron].pt() &&
                    muons.isLoose[jmuon] &&
                    electrons.charge[ielectron] * muons.charge[jmuon] < 0 )
                {
                    LorentzVector dilepton = electrons.p4[ielectron] + muons.p4[jmuon];
                    dileptons_emu.push_back(dilepton);
                    dileptons_emu_indices.push_back(std::make_pair(ielectron, jmuon));
                }
            }
        }
    }

    //Dilepton muon-electron
    for( unsigned int imuon = 0 ; imuon < muons.p4.size() ; imuon++ )
    {
        if(muons.isLoose[imuon])
        {
            for( unsigned int jelectron = 0 ; jelectron < electrons.p4.size() ; jelectron++ )
            {
                if( electrons.p4[jelectron].pt() < muons.p4[imuon].pt() &&
                    electrons.ids[jelectron]["cutBasedElectronID-Spring15-50ns-V1-standalone-loose"] &&
                    muons.charge[imuon] * electrons.charge[jelectron] < 0 )
                {
                    LorentzVector dilepton = muons.p4[imuon] + electrons.p4[jelectron];
                    dileptons_mue.push_back(dilepton);
                    dileptons_mue_indices.push_back(std::make_pair(imuon, jelectron));
                }
            }
        }
    }

}

