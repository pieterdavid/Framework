#include <cp3_llbb/Framework/interface/ElectronsProducer.h>
#include <iostream>

void ElectronsProducer::produce(edm::Event& event, const edm::EventSetup& eventSetup) {

    Identifiable::retrieves_id_tokens(event, eventSetup);

    edm::Handle<std::vector<pat::Electron>> electrons;
    event.getByToken(m_leptons_token, electrons);

    edm::Handle<double> rho_handle;
    event.getByToken(m_rho_token, rho_handle);

    double rho = *rho_handle;

    size_t index = 0;
    for (const auto& electron: *electrons) {
        std::cout << "HELLO WORLD, I AM A NEW ELECTRON!" << std::endl;
        if (! pass_cut(electron))
            continue;

        fill_candidate(electron);

        reco::GsfElectron::PflowIsolationVariables pfIso = electron.pfIsolationVariables();
        computeIsolations_R03(pfIso.sumChargedHadronPt, pfIso.sumNeutralHadronEt, pfIso.sumPhotonEt, pfIso.sumPUPt, electron.pt(), electron.superCluster()->eta(), rho);
        computeIsolations_R04(electron.chargedHadronIso(), electron.neutralHadronIso(), electron.photonIso(), electron.puChargedHadronIso(), electron.pt(), electron.superCluster()->eta(), rho);

        const pat::ElectronRef electronRef(electrons, index++);
        std::cout << "Will now produce the electron IDs" << std::endl;
        Identifiable::produce_id(electronRef);
        std::cout << "Finished producing the electron IDs" << std::endl;
    }
    Identifiable::clean();
}
