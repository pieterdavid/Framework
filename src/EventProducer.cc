#include <cp3_llbb/Framework/interface/EventProducer.h>

void EventProducer::produce(edm::Event& event_, const edm::EventSetup& eventSetup) {

    run = event_.id().run();
    lumi = event_.id().luminosityBlock();
    event = event_.id().event();

    edm::Handle<double> rho_handle;
    event_.getByToken(m_rho_token, rho_handle);
    
    rho = *rho_handle;

    pu_weight = 1;

    edm::Handle<std::vector<PileupSummaryInfo>> pu_infos;
    if (event_.getByToken(m_pu_info_token, pu_infos)) {
        for (const auto& pu_info: *pu_infos) {
            if (pu_info.getBunchCrossing() != 0)
                continue;

            npu = pu_info.getPU_NumInteractions();
            true_interactions = pu_info.getTrueNumInteractions();

            if (m_pu_reweighter.get())
                pu_weight = m_pu_reweighter->getWeight(true_interactions);
        }
    }

    weight = 1;

    edm::Handle<GenEventInfoProduct> gen_info;
    if (event_.getByToken(m_gen_info_token, gen_info)) {
        if (gen_info->hasBinningValues())
            pt_hat = gen_info->binningValues()[0];

        weight = gen_info->weight();

        n_ME_partons = gen_info->nMEPartons();
        n_ME_partons_filtered = gen_info->nMEPartonsFiltered();

        alpha_QCD = gen_info->alphaQCD();
        alpha_QED = gen_info->alphaQED();
        q_scale = gen_info->qScale();

        if (gen_info->hasPDF()) {
            pdf_id = gen_info->pdf()->id;
            pdf_x.first = gen_info->pdf()->x.first;
            pdf_x.second = gen_info->pdf()->x.second;
        }
    }

    m_event_weight_sum += weight;

    edm::Handle<LHEEventProduct> lhe_info;
    if (event_.getByToken(m_lhe_info_token, lhe_info)) {
        lhe_originalXWGTUP = lhe_info->originalXWGTUP();
        lhe_SCALUP = lhe_info->hepeup().SCALUP;
        for (auto& weight: lhe_info->weights()) {
            lhe_weights.push_back(std::make_pair(weight.id, weight.wgt));
        }
    }
}

void EventProducer::endJob(MetadataManager& metadata) {
    metadata.add("event_weight_sum", m_event_weight_sum);
    std::cout << "Sum of event weight: " << m_event_weight_sum << std::endl;
}
