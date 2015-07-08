#pragma once

#include <FWCore/ParameterSet/interface/ParameterSet.h>
#include <FWCore/Framework/interface/Event.h>
#include <FWCore/Framework/interface/EventSetup.h>
#include <FWCore/Framework/interface/ConsumesCollector.h>
#include <FWCore/Utilities/interface/InputTag.h>
#include <DataFormats/Common/interface/ValueMap.h>

#include <cp3_llbb/TreeWrapper/interface/TreeWrapper.h>

#include <iostream>

class Identifiable {

    public:
        Identifiable(ROOT::TreeGroup& tree):
            ids(tree["ids"].write<std::vector<std::map<std::string, bool>>>()) {
                // Empty
            }

        template <class T> void produce_id(const edm::Ref<T>& ref) {
            std::cout << "Entering Identifiable produce_id" << std::endl;
            std::cout << "\tref.get()= " << ref.get() << "\tids.size()= " << ids.size() << "\tm_id_maps.size()= " << m_id_maps.size() << std::endl;
            std::cout << "\tref.id()= " << ref.id() << "\tref.key()= " << ref.key() << std::endl;
            std::map<std::string, bool> ids_;
            for (auto& handle: m_id_maps) {
                std::cout << "handle.first= " << handle.first << std::endl;
                std::cout << "(*(handle.second))[ref]= " << (*(handle.second))[ref] << std::endl;
                ids_.emplace(handle.first, (*(handle.second))[ref]);
            }
            ids.push_back(ids_);
        }

        virtual void consumes_id_tokens(const edm::ParameterSet&, edm::ConsumesCollector&& collector) final;
        virtual void retrieves_id_tokens(const edm::Event& event, const edm::EventSetup& eventSetup) final;
        virtual void clean() final {
            m_id_maps.clear();
        }

    protected:
        std::vector<std::pair<std::string, edm::EDGetTokenT<edm::ValueMap<bool>>>> m_id_tokens;
        std::vector<std::pair<std::string, edm::Handle<edm::ValueMap<bool>>>> m_id_maps;
        std::vector<std::map<std::string, bool>>& ids;
};
