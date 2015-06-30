#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include <FWCore/Framework/interface/ConsumesCollector.h>
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "cp3_llbb/Framework/interface/Analyzer.h"
#include "cp3_llbb/Framework/interface/Producer.h"
#include "cp3_llbb/Framework/interface/Category.h"
#include "cp3_llbb/Framework/interface/ProducersManager.h"

class ExTreeMaker: public edm::EDProducer {
    friend class ProducersManager;

    public:
        explicit ExTreeMaker(const edm::ParameterSet&);
        ~ExTreeMaker();

    private:
        virtual void beginJob() override;
        virtual void produce(edm::Event&, const edm::EventSetup&) override;
        virtual void endJob() override;

        virtual void beginRun(const edm::Run&, const edm::EventSetup&) override;
        virtual void endRun(const edm::Run&, const edm::EventSetup&) override;

        virtual void beginLuminosityBlock(const edm::LuminosityBlock&, const edm::EventSetup&) override;
        virtual void endLuminosityBlock(const edm::LuminosityBlock&, const edm::EventSetup&) override;

        // For ProducersManager
        Framework::Producer& getProducer(const std::string& name);
        std::unique_ptr<ProducersManager> m_producers_manager;

        std::string m_output_filename;
        std::unique_ptr<TFile> m_output;
        std::unique_ptr<ROOT::TreeWrapper> m_wrapper;
        std::unordered_map<std::string, std::shared_ptr<Framework::Producer>> m_producers;
        std::vector<std::shared_ptr<Framework::Analyzer>> m_analyzers;

        // Categories
        std::unique_ptr<CategoryManager> m_categories;

        // Timing
        typedef std::chrono::system_clock clock;
        typedef std::chrono::milliseconds ms;
        typedef std::chrono::seconds seconds;

        clock::time_point m_start_time;
};


#endif
