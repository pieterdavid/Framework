#pragma once

#include <Math/Vector4D.h>

#define BRANCH(NAME, ...) __VA_ARGS__& NAME = tree[#NAME].write<__VA_ARGS__>()
#define TRANSIENT_BRANCH(NAME, ...) __VA_ARGS__& NAME = tree[#NAME].transient_write<__VA_ARGS__>()

typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<float>> LorentzVector;
