#include <iostream>
#include <memory>

#include <TFile.h>
#include <TH1F.h>

#include <cp3_llbb/Framework/interface/PUReweighter.h>

namespace Framework {

    PUReweighter::PUReweighter(const edm::ParameterSet& config, PUProfile mc_pu_profile) {

        std::string file = config.getUntrackedParameter<edm::FileInPath>("data_pu_profile").fullPath();

        // Retrieve PU histogram from file
        std::unique_ptr<TFile> f(TFile::Open(file.c_str()));
        TH1* data_pu_histogram = static_cast<TH1*>(f->Get("pileup"));

        size_t x_max = (size_t) data_pu_histogram->GetXaxis()->GetXmax();
        size_t n_bins = data_pu_histogram->GetNbinsX();

        std::shared_ptr<TH1F> mc_pu_histogram = std::make_shared<TH1F>("pu_mc", "", n_bins, 0, x_max);
        mc_pu_histogram->SetDirectory(nullptr);

        const std::vector<float>& coeffs = m_pu_profiles[mc_pu_profile];
        for (size_t i = 1; i <= (size_t) data_pu_histogram->GetNbinsX(); i++) {
            size_t coef_index = (size_t) data_pu_histogram->GetBinCenter(i);
            float coef = (coef_index) < coeffs.size() ? coeffs[coef_index] : 0.;

            mc_pu_histogram->SetBinContent(i, coef);
        }

        data_pu_histogram->Scale(1. / data_pu_histogram->Integral());
        mc_pu_histogram->Scale(1. / mc_pu_histogram->Integral());

        std::shared_ptr<TH1> pu_weights(static_cast<TH1*>(data_pu_histogram->Clone()));
        pu_weights->SetDirectory(nullptr);

        pu_weights->Divide(mc_pu_histogram.get());

        m_pu_weights = pu_weights;
    }

    float PUReweighter::getWeight(float n_interactions) {

        TH1* pu_weights = m_pu_weights.get();

        if (! pu_weights)
            return 1;

        return pu_weights->GetBinContent(pu_weights->GetXaxis()->FindBin(n_interactions));
    }
};
