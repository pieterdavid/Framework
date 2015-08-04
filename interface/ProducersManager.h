#ifndef PRODUCERS_MANAGER
#define PRODUCERS_MANAGER

#include "cp3_llbb/Framework/interface/ProducerGetter.h"
#include "cp3_llbb/Framework/interface/Producer.h"

#include <string>
#include <type_traits>

class ProducersManager {
    friend class ExTreeMaker;

    public:
    template <class T>
        const T& get(const std::string& name) const {
            static_assert(std::is_base_of<Framework::Producer, T>::value, "T must inherit from Framework::Producer");
            return dynamic_cast<const T&>(m_getter.getProducer(name));
        }

    bool exists(const std::string& name) const;

    private:
    ProducersManager(const ProducerGetter& getter);
    const ProducerGetter& m_getter;

};

#endif
