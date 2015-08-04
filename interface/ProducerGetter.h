#pragma once

#include <string>

namespace Framework {
    class Producer;
};

class ProducerGetter {
    public:
        virtual const Framework::Producer& getProducer(const std::string& name) const = 0;
        virtual bool producerExists(const std::string& name) const = 0;
};
