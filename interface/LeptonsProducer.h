#ifndef LEPTONS_PRODUCER
#define LEPTONS_PRODUCER

#include <cp3_llbb/Framework/interface/CandidatesProducer.h>
#include <RecoEgamma/EgammaTools/interface/EffectiveAreas.h>
#include <DataFormats/PatCandidates/interface/Muon.h>

#include <vector>

template <class T> class LeptonsProducer: public CandidatesProducer<T> {

    public:
        LeptonsProducer(const std::string& name, const ROOT::TreeGroup& tree, const edm::ParameterSet& config):
            CandidatesProducer<T>(name, tree, config),

            m_ea_R04(config.getUntrackedParameter<edm::FileInPath>("ea_R04").fullPath()),
            m_ea_R03(config.getUntrackedParameter<edm::FileInPath>("ea_R03").fullPath())
        {

        }

        virtual void doConsumes(const edm::ParameterSet& config, edm::ConsumesCollector&& collector) override {
            m_leptons_token = collector.consumes<std::vector<T>>(config.getUntrackedParameter<edm::InputTag>("src"));
            m_rho_token = collector.consumes<double>(config.getUntrackedParameter<edm::InputTag>("rho", edm::InputTag("fixedGridRhoFastjetAll")));
        }

        virtual ~LeptonsProducer() {}

    protected:
        void fill_candidate(const T& p) {
            CandidatesProducer<T>::fill_candidate(p);
            if( p.genParticle() != 0 )
            {
                this->matched.push_back(true);
                this->gen_p4.push_back(LorentzVector(p.genParticle()->pt(), p.genParticle()->eta(), p.genParticle()->phi(), p.genParticle()->energy()));
                this->gen_y.push_back(p.genParticle()->y());
                this->gen_charge.push_back(p.genParticle()->charge());
            } else {
                this->matched.push_back(false);
                this->gen_p4.push_back(LorentzVector(0., 0., 0., 0.));
                this->gen_y.push_back(0.);
                this->gen_charge.push_back(0.);
            }
         }
        void computeIsolations_R03(float chargedHadronIso, float neutralHadronIso, float photonIso, float puChargedHadronIso, float pt, float eta, float rho) {
            chargedHadronIsoR03.push_back(chargedHadronIso);
            neutralHadronIsoR03.push_back(neutralHadronIso);
            photonIsoR03.push_back(photonIso);
            puChargedHadronIsoR03.push_back(puChargedHadronIso);
            relativeIsoR03.push_back((chargedHadronIso + neutralHadronIso + photonIso) / pt);
            relativeIsoR03_deltaBeta.push_back((chargedHadronIso + std::max((neutralHadronIso + photonIso) - 0.5f * puChargedHadronIso, 0.0f)) / pt);

            float EA = m_ea_R03.getEffectiveArea(eta);
            relativeIsoR03_withEA.push_back((chargedHadronIso + std::max((neutralHadronIso + photonIso) - rho * EA, 0.0f)) / pt);
            EA_R03.push_back(EA);
        }

        void computeIsolations_R04(float chargedHadronIso, float neutralHadronIso, float photonIso, float puChargedHadronIso, float pt, float eta, float rho) {
            chargedHadronIsoR04.push_back(chargedHadronIso);
            neutralHadronIsoR04.push_back(neutralHadronIso);
            photonIsoR04.push_back(photonIso);
            puChargedHadronIsoR04.push_back(puChargedHadronIso);
            relativeIsoR04.push_back((chargedHadronIso + neutralHadronIso + photonIso) / pt);
            relativeIsoR04_deltaBeta.push_back((chargedHadronIso + std::max((neutralHadronIso + photonIso) - 0.5f * puChargedHadronIso, 0.0f)) / pt);

            float EA = m_ea_R04.getEffectiveArea(eta);
            relativeIsoR04_withEA.push_back((chargedHadronIso + std::max((neutralHadronIso + photonIso) - rho * EA, 0.0f)) / pt);
            EA_R04.push_back(EA);
        }

        // Tokens
        edm::EDGetTokenT<std::vector<T>> m_leptons_token;
        edm::EDGetTokenT<double> m_rho_token;

    private:

        // Effective areas
        EffectiveAreas m_ea_R04;
        EffectiveAreas m_ea_R03;

    public:
        // Tree members
        std::vector<float>& EA_R03 = this->tree["EAR03"].template write<std::vector<float>>();
        std::vector<float>& EA_R04 = this->tree["EAR04"].template write<std::vector<float>>();

        std::vector<float>& chargedHadronIsoR03 = this->tree["chargedHadronIsoR03"].template write<std::vector<float>>();
        std::vector<float>& neutralHadronIsoR03 = this->tree["neutralHadronIsoR03"].template write<std::vector<float>>();
        std::vector<float>& photonIsoR03 = this->tree["photonIsoR03"].template write<std::vector<float>>();
        std::vector<float>& puChargedHadronIsoR03 = this->tree["puChargedHadronIsoR03"].template write<std::vector<float>>();
        std::vector<float>& relativeIsoR03 = this->tree["relativeIsoR03"].template write<std::vector<float>>();
        std::vector<float>& relativeIsoR03_deltaBeta = this->tree["relativeIsoR03_deltaBeta"].template write<std::vector<float>>();
        std::vector<float>& relativeIsoR03_withEA = this->tree["relativeIsoR03_withEA"].template write<std::vector<float>>();

        std::vector<float>& chargedHadronIsoR04 = this->tree["chargedHadronIsoR04"].template write<std::vector<float>>();
        std::vector<float>& neutralHadronIsoR04 = this->tree["neutralHadronIsoR04"].template write<std::vector<float>>();
        std::vector<float>& photonIsoR04 = this->tree["photonIsoR04"].template write<std::vector<float>>();
        std::vector<float>& puChargedHadronIsoR04 = this->tree["puChargedHadronIsoR04"].template write<std::vector<float>>();
        std::vector<float>& relativeIsoR04 = this->tree["relativeIsoR04"].template write<std::vector<float>>();
        std::vector<float>& relativeIsoR04_deltaBeta = this->tree["relativeIsoR04_deltaBeta"].template write<std::vector<float>>();
        std::vector<float>& relativeIsoR04_withEA = this->tree["relativeIsoR04_withEA"].template write<std::vector<float>>();
};

#endif
