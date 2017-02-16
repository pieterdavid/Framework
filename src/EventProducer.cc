#include <cp3_llbb/Framework/interface/EventProducer.h>

#include <regex>

void EventProducer::beginRun(const edm::Run& iRun, const edm::EventSetup& eventSetup) {

    edm::Handle<LHERunInfoProduct> lheRunProduct;
    // Do *not* use `getByToken` here as it won't work
    // See https://hypernews.cern.ch/HyperNews/CMS/get/physTools/3437.html
    if (!iRun.getByLabel("externalLHEProducer", lheRunProduct))
        return;

    int32_t pdf_set_ = lheRunProduct->heprup().PDFSUP.first;
    if (pdf_set_ < 0) {
#if defined DEBUG_PDF or defined DEBUG_PDF_LIGHT
        std::cout << "PDF set not stored inside LHERunInfoProduct. Parsing LHE headers." << std::endl;
#endif
        // Powheg sample. Find the PDF set in the headers
        // Line looks like 'lhans1 260000       ! pdf set for hadron 1 (LHA numbering)'
        bool got_it = false;
        static std::regex pdfset_regex(R"(lhans1\s+(\d+))");
        for (auto it = lheRunProduct->headers_begin(); it != lheRunProduct->headers_end(); ++it) {
            for (auto& line: it->lines()) {
#if defined DEBUG_PDF or defined DEBUG_PDF_LIGHT
                std::cout << line;
#endif
                std::smatch results;
                if (std::regex_search(line, results, pdfset_regex)) {
                    pdf_set_ = std::stoi(results[1].str());
                    got_it = true;
                    break;
                }
            }

            if (got_it) {
                break;
            }
        }
    }
    pdf_set = (uint32_t) pdf_set_;
    std::string pdf_set_str;

#if defined DEBUG_PDF or defined DEBUG_PDF_LIGHT
    std::cout << "PDF set: " << pdf_set_ << std::endl;
#endif

    isLO = (pdf_set == 263000) || (pdf_set == 263400);

    std::string scale_variation_group = "scale_variation";
    std::string hdamp_variation_group = "hdamp_variation";

    if (isLO) {
        pdf_set_str = m_lo_pdf_set_strs[pdf_set];
#ifdef USE_LHE_WEIGHTS_FOR_LO
        scale_variation_group = "Central scale variation";
        pdf_set_str += ".LHgrid";
#else
        std::cout << "Using LHAPDF to compute PDF uncertainties instead of weights stored in the LHE file" << std::endl;
        lhapdf_pdfs = LHAPDF::mkPDFs(pdf_set_str);
#endif
    }

    static std::regex weightgroup_regex("<weightgroup combine=\".*\" (?:type|name)=\"(.*)\">");
    // Find mapping between PDF set variations and weight ids
    // Lines looks like '<weight id="2001"> PDF set = 260001 </weight>'
    static std::regex nlo_weight_id_regex("<weight id=\"(\\d+)\">.*?(\\d+).*</weight>");
#ifdef USE_LHE_WEIGHTS_FOR_LO
    static std::regex lo_weight_id_regex("<weight id=\"(\\d+)\">.*</weight>");
#endif
    for (auto it = lheRunProduct->headers_begin(); it != lheRunProduct->headers_end(); ++it) {
        if (it->tag() != "initrwgt")
            continue;

        size_t global_index = 0;
        uint32_t pdf_set_to_match = pdf_set + 1;
        uint32_t id_to_match = 0;
        bool currently_matching = false;

        bool in_group = false;
        std::string current_group_type;

        size_t current_scale_variation_index = 0;

        for (auto& line: it->lines()) {
#if defined DEBUG_PDF or defined DEBUG_PDF_LIGHT
            std::cout << line;
#endif
            std::smatch results;

            if (! in_group) {
                if (std::regex_search(line, results, weightgroup_regex)) {
                    in_group = true;
                    current_group_type = results[1].str();
#if defined DEBUG_PDF or defined DEBUG_PDF_LIGHT
                    std::cout << "New group: " << current_group_type << std::endl;
#endif
                }

                continue;
            }

            if (line.find("</weightgroup>") != std::string::npos) {
                in_group = false;
                currently_matching = false;
                current_group_type = "";
#if defined DEBUG_PDF or defined DEBUG_PDF_LIGHT
                    std::cout << "End of current group" << std::endl;
#endif
                continue;
            }

            global_index++;

            bool regex_ok = false;
#ifdef USE_LHE_WEIGHTS_FOR_LO
            if (isLO) {
                regex_ok = std::regex_search(line, results, lo_weight_id_regex);
            } else {
#endif
                regex_ok = std::regex_search(line, results, nlo_weight_id_regex);
#ifdef USE_LHE_WEIGHTS_FOR_LO
            }
#endif

            if (! regex_ok) {
                std::cout << "Error: there's something wrong in PDF block parsing. The regex matching failed, and this should not happened. Expect a crash soon :)" << std::endl;
                continue;
            }

            uint32_t id = std::stoi(results[1].str());

#if defined DEBUG_PDF or defined DEBUG_PDF_LIGHT
            std::cout << "Extracted id: " << id << std::endl;
#endif

            // Scale variations?
            if (current_group_type == scale_variation_group) {
                // Ignore scale variation index 0, 5 and 7
                if (current_scale_variation_index == 0 || current_scale_variation_index == 5 || current_scale_variation_index == 7) {
                    current_scale_variation_index++;
                    continue;
                }

                m_scale_variations_matching.push_back(std::make_pair(id, global_index - 1));
                current_scale_variation_index++;

                continue;
            } else if (current_group_type == hdamp_variation_group) {
                has_hdamp_variation = true;

                // Only valid index are 5019 and 5010
                // See https://twiki.cern.ch/twiki/bin/view/CMS/TopSystematics#Matrix_element_Parton_Shower_ME
                if (id == 5010) {
                    hdamp_down_index = global_index - 1;
                } else if (id == 5019) {
                    hdamp_up_index = global_index - 1;
                }
            }

            // PDF variations
#ifdef USE_LHE_WEIGHTS_FOR_LO
            if (isLO) {
                // Check that we are currently parsing the group corresponding to our PDF
                if (current_group_type != pdf_set_str) {
                    if (currently_matching)
                        break;

                    continue;
                }

                if (! currently_matching) {
                    currently_matching = true;
                    // Skip the first weight as it's the nominal one
                    continue;
                }

                m_pdf_weights_matching.push_back(std::make_pair(id, global_index - 1));

            } else {
#endif
                uint32_t local_pdf_set = std::stoi(results[2].str());

                if (local_pdf_set == pdf_set_to_match) {
                    currently_matching = true;
                    id_to_match = id;
                }

                if (id_to_match == id) {
                    m_pdf_weights_matching.push_back(std::make_pair(id, global_index - 1));
                    id_to_match++;
                }
#ifdef USE_LHE_WEIGHTS_FOR_LO
            }
#endif
        }

#if defined DEBUG_PDF or defined DEBUG_PDF_LIGHT
        std::cout << "Relation table between CMSSW IDs and index" << std::endl;
        std::cout << "CMSSW ID -> index" << std::endl;
        for (const auto& m: m_pdf_weights_matching) {
            std::cout << m.first << " -> " << m.second << std::endl;
        }

        std::cout << "Number of replicas: " << m_pdf_weights_matching.size() << std::endl;

        for (const auto& m: m_scale_variations_matching) {
            std::cout << m.first << " -> " << m.second << std::endl;
        }
        std::cout << "Number of scales variation weights: " << m_scale_variations_matching.size() << std::endl;

        if (has_hdamp_variation) {
            std::cout << "This sample has hdamp scale variations" << std::endl;
            std::cout << "  -> Up: " << hdamp_up_index << std::endl;
            std::cout << "  -> Down: " << hdamp_down_index << std::endl;
        }
#endif

        if (!m_scale_variations_matching.empty() && m_scale_variations_matching.size() != 6) {
            std::cout << "\033[0;31m" << "Warning: invalid number of scale variation (6 was expected, only " << m_scale_variations_matching.size() << " found)." << "\033[0m" << std::endl;
        }

        break;
    }

    has_alphas_uncertainty = false;
    // These PDF set do not have alpha_s variation
    if ((pdf_set != 263000) && (pdf_set != 263400)) {
        has_alphas_uncertainty = true;
#if defined DEBUG_PDF or defined DEBUG_PDF_LIGHT
        std::cout << "This PDF set has alphaS variation" << std::endl;
#endif
    }

    // If PDF is not NNPDF30, warn the user because the PDF weights won't be valid
    if ((pdf_set != 263000) && (pdf_set != 263400) && (pdf_set != 260000) && (pdf_set != 260400) && (pdf_set != 292200) && (pdf_set != 292000)) {
        std::cout << "\033[0;31m" << "Warning: this sample was not generated using NNPDF30 PDF. The uncertainty on the PDF computed by this module are probably not correct for this PDF. Use this caution." << "\033[0m" << std::endl;
    }
}

void EventProducer::produce(edm::Event& event_, const edm::EventSetup& eventSetup) {

    run = event_.id().run();
    lumi = event_.id().luminosityBlock();
    event = event_.id().event();
    is_data = event_.isRealData();

    edm::Handle<double> rho_handle;
    event_.getByToken(m_rho_token, rho_handle);
    
    rho = *rho_handle;

    pu_weight = 1;

    // Moriond17 reminiaod validation flags
    if (event_.isRealData()) {
        edm::Handle<bool> dupECALClusters_handle;
        edm::Handle<DetIdCollection> hitsNotReplaced_handle;

        if (event_.getByToken(m_dupECALClusters_token, dupECALClusters_handle)) {
            event_.getByToken(m_hitsNotReplaced_token, hitsNotReplaced_handle);

            dupECALClusters = *dupECALClusters_handle;
            hitsNotReplacedEmpty = hitsNotReplaced_handle->empty();
        }
    }

    edm::Handle<std::vector<PileupSummaryInfo>> pu_infos;
    if (event_.getByToken(m_pu_info_token, pu_infos)) {
        for (const auto& pu_info: *pu_infos) {
            if (pu_info.getBunchCrossing() != 0)
                continue;

            npu = pu_info.getPU_NumInteractions();
            true_interactions = pu_info.getTrueNumInteractions();

            if (m_pu_reweighter.get())
                pu_weight = m_pu_reweighter->getWeight(true_interactions);
                pu_weight_up = m_pu_reweighter_up->getWeight(true_interactions);
                pu_weight_down = m_pu_reweighter_down->getWeight(true_interactions);
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

    pdf_weight = 1;
    pdf_weight_up = 1;
    pdf_weight_down = 1;

    pdf_weight_gg = 1.;
    pdf_weight_gg_up = 1.;
    pdf_weight_gg_down = 1.;

    pdf_weight_qq = 1.;
    pdf_weight_qq_up = 1.;
    pdf_weight_qq_down = 1.;

    pdf_weight_qg = 1.;
    pdf_weight_qg_up = 1.;
    pdf_weight_qg_down = 1.;

    hdamp_weight_up = 1.;
    hdamp_weight_down = 1.;

    if (event_.isRealData() || m_scale_variations_matching.empty()) {
        // Push six 1 to the scale variation array
        // for compatibility between MC and data
        scale_weights.resize(6, 1);
    }

    edm::Handle<LHEEventProduct> lhe_info;
    if (event_.getByToken(m_lhe_info_token, lhe_info)) {
        lhe_originalXWGTUP = lhe_info->originalXWGTUP();
        lhe_SCALUP = lhe_info->hepeup().SCALUP;

        // Compute HT of the event
        const lhef::HEPEUP& lhe_hepeup = lhe_info->hepeup();
        std::vector<lhef::HEPEUP::FiveVector> lhe_particles = lhe_hepeup.PUP;
        ht = 0;
        for ( size_t iparticle = 0; iparticle < lhe_particles.size(); iparticle++ ) {
            int pdgid_ = lhe_hepeup.IDUP[iparticle];
            int status_ = lhe_hepeup.ISTUP[iparticle];
            if ( status_ == 1 && ((std::abs(pdgid_) >= 1 && std::abs(pdgid_) <= 6 ) || (std::abs(pdgid_) == 21)) )
                ht += std::sqrt(lhe_particles[iparticle][0]*lhe_particles[iparticle][0] + lhe_particles[iparticle][1]*lhe_particles[iparticle][1]);
        }

        // Handle PDF & scale weights

        // No weight on some events...
        if (lhe_info->weights().empty()) {
            std::cout << "Warning: no weight for this event." << std::endl;
            // Set scale weights to 1. PDF weights are already set to 1 by default
            scale_weights.resize(6, 1);
            goto end;
        }

        auto weight_is_valid = [](double weight) {
            if (std::isnan(weight) || std::isinf(weight))
                return false;

            // On some samples, weights coming from the LHE are crazy (either small or 0, or very large) like ZZTo2L2Nu
            // Example:
            // Computed weight #76 = 0 (raw LHE weight: 1.99712e-62)
            // Computed weight #77 = inf (raw LHE weight: 3.63487e+228)
            // Protect against those pathological weight by fixing a hard limit on how big or small the weight can be
            if ((weight < 0.01) || (weight > 100))
                return false;

            return true;
        };

        if (isLO && !scalup_decision_taken) {
            scalup_decision_taken = true;

            // Compute all PDF weights. If all of them are crazy, then switch to workaround mode
            size_t n_invalid = 0;
            if (!m_pdf_weights_matching.empty()) {
                for (auto& w: m_pdf_weights_matching) {
                    float weight = lhe_info->weights()[w.second].wgt / lhe_originalXWGTUP;
                    if (!weight_is_valid(weight)) {
                        n_invalid++;
                    }
                }
            }

            if (n_invalid == m_pdf_weights_matching.size())
                use_scalup_for_lo_weights = true;

#ifdef DEBUG_PDF
            std::cout << "Workaround for LO LHE weights:";
            if (use_scalup_for_lo_weights)
                std::cout << " use SCALUP";
            else
                std::cout << " use originalXWGTUP";
            std::cout << std::endl;
#endif
        }

        const float lhe_weight_nominal_weight = (isLO && use_scalup_for_lo_weights) ? lhe_SCALUP : lhe_originalXWGTUP;

        // Scale variations
        for (auto& m: m_scale_variations_matching) {
            float weight = lhe_info->weights()[m.second].wgt / lhe_weight_nominal_weight;
            if (!weight_is_valid(weight)) {
#ifdef DEBUG_PDF
                std::cout << "Corrupted scale weight #" << scale_weights.size() << std::endl;
#endif
                weight = 1.;
            }
            scale_weights.push_back(weight);
        }

        // hdamp variation
        if (has_hdamp_variation) {
            hdamp_weight_up = lhe_info->weights()[hdamp_up_index].wgt / lhe_weight_nominal_weight;
            hdamp_weight_down = lhe_info->weights()[hdamp_down_index].wgt / lhe_weight_nominal_weight;

            if (! weight_is_valid(hdamp_weight_up))
                hdamp_weight_up = 1.;

            if (! weight_is_valid(hdamp_weight_down))
                hdamp_weight_down = 1.;
        }

        // PDF variations
        for (auto& weight: lhe_info->weights()) {
            lhe_weights.push_back(std::make_pair(weight.id, weight.wgt));
        }

        std::vector<float> pdf_weights;
        pdf_weights.reserve(110);
        float mean = 0.;
        size_t n = 0;

        if (m_pdf_weights_matching.empty()) {
#ifndef USE_LHE_WEIGHTS_FOR_LO
            if (! isLO)
                goto end;

            n = lhapdf_pdfs.size() - 1;

            // Evaluate each member of the PDF for each parton

            double xpdf1_nominal = lhapdf_pdfs[0]->xfxQ(pdf_id.first, pdf_x.first, q_scale);
            double xpdf2_nominal = lhapdf_pdfs[0]->xfxQ(pdf_id.second, pdf_x.second, q_scale);
            double w0 = xpdf1_nominal * xpdf2_nominal;

            for (size_t i = 1; i < lhapdf_pdfs.size(); i++) {
                double xpdf1 = lhapdf_pdfs[i]->xfxQ(pdf_id.first, pdf_x.first, q_scale);
                double xpdf2 = lhapdf_pdfs[i]->xfxQ(pdf_id.second, pdf_x.second, q_scale);

                pdf_weights.push_back((xpdf1 * xpdf2) / w0);
#ifdef DEBUG_PDF
                std::cout << "Computed weight #" << i << " = " << pdf_weights.back() << std::endl;
#endif

                mean += pdf_weights.back();
            }
#else
            goto end;
#endif
        } else {
            size_t from = 0;
            size_t to = m_pdf_weights_matching.size();

            if (has_alphas_uncertainty)
                to -= 2;

            n = to - from;

            for (size_t i = from; i < to; i++) {
                float weight = lhe_info->weights()[m_pdf_weights_matching[i].second].wgt / lhe_weight_nominal_weight;
#ifdef DEBUG_PDF
                std::cout << "Computed weight #" << i + 1 << " = " << weight << " (raw LHE weight: " << lhe_info->weights()[m_pdf_weights_matching[i].second].wgt << ")" << std::endl;
#endif
                if (!weight_is_valid(weight)) {
#if defined DEBUG_PDF or defined DEBUG_PDF_LIGHT
                    std::cout << "PDF weight #" << i + 1 << " is not valid, skipping." << std::endl;
#endif
                    continue;
                }

                mean += weight;
                pdf_weights.push_back(weight);
            }
        }

        float rms = 0.;
        if (pdf_weights.size() < (n * 0.8)) {
            // Not enough weights to compute a meaningfull RMS
            mean = 1;
            rms = 0;
            std::cout << "Error: not enough weights to compute PDF uncertainty." << std::endl;
        } else {

            mean /= pdf_weights.size();

            // Compute standard deviation
            for (auto weight: pdf_weights) {
                rms += (weight - mean) * (weight - mean);
            }
            rms = std::sqrt(1 / (pdf_weights.size() - 1.) * rms);

#ifdef DEBUG_PDF
            std::cout << "PDF uncertainty: " << rms << " (mean value: " << mean << ")" << std::endl;
#endif

            // Add alphaS uncertainty
            if (has_alphas_uncertainty) {
                float weight_alphas_up = lhe_info->weights()[m_pdf_weights_matching[n].second].wgt / lhe_weight_nominal_weight;
                float weight_alphas_down = lhe_info->weights()[m_pdf_weights_matching[n + 1].second].wgt / lhe_weight_nominal_weight;

                float alphas_uncertainty = 0;
                if (weight_is_valid(weight_alphas_up) && weight_is_valid(weight_alphas_down)) {
                    // 1.5 factor is needed because NNPDF30 alphaS uncertainties are
                    // +- 0.001 whereas recommendations for Run2 are +- 0.0015
                    alphas_uncertainty = 1.5 * (weight_alphas_up - weight_alphas_down) / 2.;
                }
#ifdef DEBUG_PDF
                std::cout << "alphaS uncertainty: " << alphas_uncertainty << std::endl;
#endif
                rms = std::sqrt(rms * rms + alphas_uncertainty * alphas_uncertainty);
            }
        }

        INITIAL_STATE initial_state_;
        if ((pdf_id.first == 0 || std::abs(pdf_id.first) == 21) && (pdf_id.second == 0 || pdf_id.second == 21))
            initial_state_ = INITIAL_STATE::GG;
        else if ((pdf_id.first == 0 || std::abs(pdf_id.first) == 21) || (pdf_id.second == 0 || pdf_id.second == 21))
            initial_state_ = INITIAL_STATE::QG;
        else
            initial_state_ = INITIAL_STATE::QQ;

        initial_state = static_cast<uint8_t>(initial_state_);

        pdf_weight = mean;
        pdf_weight_up = mean + rms;
        pdf_weight_down = std::max(0.f, mean - rms); // Prevent negative weight

        if (initial_state_ == INITIAL_STATE::GG) {
            pdf_weight_gg = mean;
            pdf_weight_gg_up = mean + rms;
            pdf_weight_gg_down = std::max(0.f, mean - rms); // Prevent negative weight
        } else if (initial_state_ == INITIAL_STATE::QQ) {
            pdf_weight_qq = mean;
            pdf_weight_qq_up = mean + rms;
            pdf_weight_qq_down = std::max(0.f, mean - rms); // Prevent negative weight
        } else {
            pdf_weight_qg = mean;
            pdf_weight_qg_up = mean + rms;
            pdf_weight_qg_down = std::max(0.f, mean - rms); // Prevent negative weight
        }

#ifdef DEBUG_PDF
        std::cout << "Total PDF uncertainty: " << rms << std::endl;
#endif
    }

end:

    if (std::isnan(weight) || std::isinf(weight)) {
#if defined DEBUG_PDF or defined DEBUG_PDF_LIGHT
        std::cout << "ERROR: event weight is NaN or inf" << std::endl;
#endif
        weight = 1.;
    }

    if (std::isnan(pdf_weight) || std::isinf(pdf_weight)) {
#if defined DEBUG_PDF or defined DEBUG_PDF_LIGHT
        std::cout << "ERROR: event pdf weight is NaN or inf" << std::endl;
#endif
        pdf_weight = 1.;
    }

    if (std::isnan(pdf_weight_up) || std::isinf(pdf_weight_up)) {
#if defined DEBUG_PDF or defined DEBUG_PDF_LIGHT
        std::cout << "ERROR: event pdf weight up is NaN or inf" << std::endl;
#endif

        pdf_weight_up = 1.;
    }

    if (std::isnan(pdf_weight_down) || std::isinf(pdf_weight_down)) {
#if defined DEBUG_PDF or defined DEBUG_PDF_LIGHT
        std::cout << "ERROR: event pdf weight down is NaN or inf" << std::endl;
#endif
        pdf_weight_down = 1.;
    }


    m_event_weight_sum += weight;
    m_event_weight_sum_pdf_nominal += weight * pdf_weight;
    m_event_weight_sum_pdf_up += weight * pdf_weight_up;
    m_event_weight_sum_pdf_down += weight * pdf_weight_down;

    m_event_weight_sum_pdf_qq_nominal += weight * pdf_weight_qq;
    m_event_weight_sum_pdf_qq_up += weight * pdf_weight_qq_up;
    m_event_weight_sum_pdf_qq_down += weight * pdf_weight_qq_down;

    m_event_weight_sum_pdf_gg_nominal += weight * pdf_weight_gg;
    m_event_weight_sum_pdf_gg_up += weight * pdf_weight_gg_up;
    m_event_weight_sum_pdf_gg_down += weight * pdf_weight_gg_down;

    m_event_weight_sum_pdf_qg_nominal += weight * pdf_weight_qg;
    m_event_weight_sum_pdf_qg_up += weight * pdf_weight_qg_up;
    m_event_weight_sum_pdf_qg_down += weight * pdf_weight_qg_down;

    m_event_weight_sum_hdamp_up += weight * hdamp_weight_up;
    m_event_weight_sum_hdamp_down += weight * hdamp_weight_down;

    if (! event_.isRealData()) {

        if (m_event_weight_sum_scales.size() != scale_weights.size()) {
            m_event_weight_sum_scales.resize(scale_weights.size());
        }

        for (size_t i = 0; i < scale_weights.size(); i++) {

            if (std::isnan(scale_weights[i]) || std::isinf(scale_weights[i])) {
#if defined DEBUG_PDF or defined DEBUG_PDF_LIGHT
                std::cout << "ERROR: event scale weight " << i << " is NaN or inf" << std::endl;
#endif
                scale_weights[i] = 1.;
            }

            m_event_weight_sum_scales[i] += weight * scale_weights[i];
        }
    }
}

void EventProducer::endJob(MetadataManager& metadata) {
    metadata.add("event_weight_sum", m_event_weight_sum);
    metadata.add("event_weight_sum_pdf_nominal", m_event_weight_sum_pdf_nominal);
    metadata.add("event_weight_sum_pdf_up", m_event_weight_sum_pdf_up);
    metadata.add("event_weight_sum_pdf_down", m_event_weight_sum_pdf_down);

    metadata.add("event_weight_sum_pdf_qq_nominal", m_event_weight_sum_pdf_qq_nominal);
    metadata.add("event_weight_sum_pdf_qq_up", m_event_weight_sum_pdf_qq_up);
    metadata.add("event_weight_sum_pdf_qq_down", m_event_weight_sum_pdf_qq_down);

    metadata.add("event_weight_sum_pdf_gg_nominal", m_event_weight_sum_pdf_gg_nominal);
    metadata.add("event_weight_sum_pdf_gg_up", m_event_weight_sum_pdf_gg_up);
    metadata.add("event_weight_sum_pdf_gg_down", m_event_weight_sum_pdf_gg_down);

    metadata.add("event_weight_sum_pdf_qg_nominal", m_event_weight_sum_pdf_qg_nominal);
    metadata.add("event_weight_sum_pdf_qg_up", m_event_weight_sum_pdf_qg_up);
    metadata.add("event_weight_sum_pdf_qg_down", m_event_weight_sum_pdf_qg_down);

    metadata.add("event_weight_sum_hdamp_up", m_event_weight_sum_hdamp_up);
    metadata.add("event_weight_sum_hdamp_down", m_event_weight_sum_hdamp_down);

    for (size_t i = 0; i < m_event_weight_sum_scales.size(); i++) {
        std::string name = "event_weight_sum_scale_" + std::to_string(i);
        metadata.add(name, m_event_weight_sum_scales[i]);
    }

    std::cout << "Sum of event weight: " << m_event_weight_sum << std::endl;

#ifndef USE_LHE_WEIGHTS_FOR_LO
    for (auto& pdf: lhapdf_pdfs) {
        delete pdf;
    }
#endif
}
