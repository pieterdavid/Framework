#include <cp3_llbb/Framework/interface/MetadataManager.h>

#include <TParameter.h>

template<>
void MetadataManager::add(const std::string& name, const double& value) {
    std::shared_ptr<TObject> v(new TParameter<double>(name.c_str(), value));
    m_file->WriteTObject(v.get());
    m_trash.push_back(v);
}

template<>
void MetadataManager::add(const std::string& name, const float& value) {
    std::shared_ptr<TObject> v(new TParameter<float>(name.c_str(), value));
    m_file->WriteTObject(v.get());
    m_trash.push_back(v);
}
