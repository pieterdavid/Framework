#include <cp3_llbb/Framework/interface/VerticesProducer.h>

void VerticesProducer::produce(edm::Event& event, const edm::EventSetup& eventSetup) {

    edm::Handle<std::vector<reco::Vertex>> vertices;
    event.getByToken(m_vertices_token, vertices);

    for (const reco::Vertex& vertex: *vertices) {
        ndof.push_back(vertex.ndof());
        normalizedChi2.push_back(vertex.normalizedChi2());
        isFake.push_back(vertex.isFake());
        isValid.push_back(vertex.isValid());
        position.push_back(vertex.position());
        covariance.push_back(vertex.covariance());
    }

}
