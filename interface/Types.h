#pragma once

#include <Math/Vector4D.h>

#define BRANCH(NAME, ...) __VA_ARGS__& NAME = tree[#NAME].write<__VA_ARGS__>()
#define TRANSIENT_BRANCH(NAME, ...) __VA_ARGS__& NAME = tree[#NAME].transient_write<__VA_ARGS__>()
#define ONLY_NOMINAL_BRANCH(NAME, ...) __VA_ARGS__& NAME = Framework::condition_branch<__VA_ARGS__>(tree, #NAME, !doingSystematics())

typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<float>> LorentzVector;

#include <cp3_llbb/TreeWrapper/interface/TreeWrapper.h>
namespace Framework {
    template <typename T>
    T& condition_branch(ROOT::TreeGroup& tree, const std::string& branch_name, bool condition) {
        if (condition) {
            return tree[branch_name].write<T>();
        } else {
            static T foo;
            return foo;
        }
    }
}
