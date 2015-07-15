// -*- C++ -*-
//
// Package:    cp3_llbb/Framework
// Class:      ExTreeMaker
// 
//
// Original Author:  Sébastien Brochet
//         Created:  Fri, 26 Jun 2015 13:40:32 GMT
//
//


// system include files
#include <vector>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <chrono>

// user include files
#include <cp3_llbb/Framework/interface/Framework.h>
#include <cp3_llbb/TreeWrapper/interface/TreeWrapper.h>

#include <TFile.h>
#include <TTree.h>

ExTreeMaker::ExTreeMaker(const edm::ParameterSet& iConfig):
    m_output_filename(iConfig.getParameter<std::string>("output")) {

        m_output.reset(TFile::Open(m_output_filename.c_str(), "recreate"));

        TTree* tree = new TTree("t", "t");
        m_wrapper.reset(new ROOT::TreeWrapper(tree));

        m_categories.reset(new CategoryManager(*m_wrapper));
        m_producers_manager.reset(new ProducersManager(this));

        // Load plugins
        if (!iConfig.existsAs<edm::ParameterSet>("producers")) {
            throw new std::logic_error("No producers specified");
        }

        std::cout << std::endl << "producers: " << std::endl;
        const edm::ParameterSet& producers = iConfig.getParameterSet("producers");
        std::vector<std::string> producersName = producers.getParameterNames();
        for (std::string& producerName: producersName) {
            edm::ParameterSet producerData = producers.getParameterSet(producerName);
            bool enable = producerData.getParameter<bool>("enable");
            if (! enable)
                continue;

            const std::string type = producerData.getParameter<std::string>("type");
            const std::string tree_prefix = producerData.getParameter<std::string>("prefix");
            edm::ParameterSet producerParameters;
            if (producerData.existsAs<edm::ParameterSet>("parameters"))
                producerParameters = producerData.getParameterSet("parameters");

            std::cout << " -> Adding producer '" << producerName << "' of type '" << type << "'" << std::endl;
            auto producer = std::shared_ptr<Framework::Producer>(ExTreeMakerProducerFactory::get()->create(type, producerName, m_wrapper->group(tree_prefix), producerParameters));
            producer->doConsumes(producerParameters, consumesCollector());

            m_producers.emplace(producerName, producer);
        }

        if (!iConfig.existsAs<edm::ParameterSet>("analyzers")) {
            return;
        }

        std::cout << std::endl << "analyzers: " << std::endl;
        const edm::ParameterSet& analyzers = iConfig.getParameterSet("analyzers");
        std::vector<std::string> analyzersName = analyzers.getParameterNames();
        for (std::string& analyzerName: analyzersName) {
            edm::ParameterSet analyzerData = analyzers.getParameterSet(analyzerName);
            bool enable = analyzerData.getParameter<bool>("enable");
            if (! enable)
                continue;

            const std::string type = analyzerData.getParameter<std::string>("type");
            const std::string tree_prefix = analyzerData.getParameter<std::string>("prefix");
            edm::ParameterSet analyzerParameters;
            if (analyzerData.existsAs<edm::ParameterSet>("parameters"))
                analyzerParameters = analyzerData.getParameterSet("parameters");

            std::cout << " -> Adding analyzer '" << analyzerName << "' of type '" << type << "'" << std::endl;
            auto analyzer = std::shared_ptr<Framework::Analyzer>(ExTreeMakerAnalyzerFactory::get()->create(type, analyzerName, m_wrapper->group(tree_prefix), analyzerParameters));
            analyzer->doConsumes(analyzerParameters, consumesCollector());
            analyzer->registerCategories(*m_categories);

            m_analyzers.push_back(analyzer);
        }

}


ExTreeMaker::~ExTreeMaker() {

}


void ExTreeMaker::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
    for (auto& producer: m_producers)
        producer.second->produce(iEvent, iSetup);

    for (auto& analyzer: m_analyzers)
        analyzer->analyze(iEvent, iSetup, *m_producers_manager);

    if (m_categories->evaluate(*m_producers_manager))
        m_wrapper->fill();
    else
        m_wrapper->reset();
}


// ------------ method called once each job just before starting event loop  ------------
void ExTreeMaker::beginJob() {
    m_start_time = clock::now();

    for (auto& producer: m_producers)
        producer.second->beginJob();

    for (auto& analyzer: m_analyzers)
        analyzer->beginJob();
}

// ------------ method called once each job just after ending the event loop  ------------
void ExTreeMaker::endJob() {
    for (auto& producer: m_producers)
        producer.second->endJob();

    for (auto& analyzer: m_analyzers)
        analyzer->endJob();

    auto end_time = clock::now();

    std::cout << std::endl << "Job done in " << std::chrono::duration_cast<ms>(end_time - m_start_time).count() / 1000. << "s" << std::endl;
    m_output->Write();

    m_categories->print_summary();
}

void ExTreeMaker::beginRun(const edm::Run& run, const edm::EventSetup& eventSetup) {
    for (auto& producer: m_producers)
        producer.second->beginRun(run, eventSetup);

    for (auto& analyzer: m_analyzers)
        analyzer->beginRun(run, eventSetup);

}

void ExTreeMaker::endRun(const edm::Run& run, const edm::EventSetup& eventSetup) {
    for (auto& producer: m_producers)
        producer.second->endRun(run, eventSetup);

    for (auto& analyzer: m_analyzers)
        analyzer->endRun(run, eventSetup);
}

void ExTreeMaker::beginLuminosityBlock(const edm::LuminosityBlock& lumi, const edm::EventSetup& eventSetup) {
    for (auto& producer: m_producers)
        producer.second->beginLuminosityBlock(lumi, eventSetup);

    for (auto& analyzer: m_analyzers)
        analyzer->beginLuminosityBlock(lumi, eventSetup);

}

void ExTreeMaker::endLuminosityBlock(const edm::LuminosityBlock& lumi, const edm::EventSetup& eventSetup) {
    for (auto& producer: m_producers)
        producer.second->endLuminosityBlock(lumi, eventSetup);

    for (auto& analyzer: m_analyzers)
        analyzer->endLuminosityBlock(lumi, eventSetup);
}

Framework::Producer& ExTreeMaker::getProducer(const std::string& name) {
    const auto& producer = m_producers.find(name);
    if (producer == m_producers.end()) {
        // FIXME: Throw
    }

    return *producer->second;
}

//define this as a plug-in
DEFINE_FWK_MODULE(ExTreeMaker);
