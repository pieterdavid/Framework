#ifndef PRODUCERS_MANAGER
#define PRODUCERS_MANAGER

#include <string>

namespace Framework {
    class Producer;
};

class ExTreeMaker;

class ProducersManager {
    friend class ExTreeMaker;

    public:
        const Framework::Producer& get(const std::string& name) const;

    private:
        ProducersManager(ExTreeMaker* framework);
        ExTreeMaker* m_framework;

};

#endif
