#include <cp3_llbb/Framework/interface/Tools.h>
#include <DataFormats/PatCandidates/interface/Jet.h>

namespace Tools {
    namespace Jets {

        // Jet id: https://twiki.cern.ch/twiki/bin/view/CMS/JetID#Recommendations_for_13_TeV_data
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
        bool passLooseId(const pat::Jet& jet) {
            DECLARE_QUANTITIES(jet);

            if (fabs(eta) <= 3.0) {
                return (NHF < 0.99 && NEMF < 0.99 && NumConst > 1) && ((fabs(eta) <= 2.4 && CHF > 0 && CHM > 0 && CEMF < 0.99) || fabs(eta) > 2.4);
            } else {
                return (NEMF < 0.90 && NumNeutralParticles > 10);
            }
        }

        bool passTightId(const pat::Jet& jet) {
            DECLARE_QUANTITIES(jet);

            if (fabs(eta) <= 3.0) {
                return (NHF < 0.90 && NEMF < 0.90 && NumConst > 1) && ((fabs(eta) <= 2.4 && CHF > 0 && CHM > 0 && CEMF < 0.99) || fabs(eta) > 2.4);
            } else {
                return (NEMF < 0.90 && NumNeutralParticles > 10);
            }
        }

        bool passTightLeptonVetoId(const pat::Jet& jet) {
            DECLARE_QUANTITIES(jet);

            return (NHF < 0.90 && NEMF < 0.90 && NumConst > 1 && MUF < 0.8) && ((fabs(eta) <= 2.4 && CHF > 0 && CHM > 0 && CEMF < 0.90) || fabs(eta) > 2.4) && fabs(eta) <= 3.0;
        }
#pragma GCC diagnostic pop
    };
};
