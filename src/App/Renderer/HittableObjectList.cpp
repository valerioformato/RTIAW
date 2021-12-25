#include <numeric>

#include "HittableObjectList.h"

template <class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

namespace RTIAW::Render {
void HittableObjectList::Add(const Shape &shape, const Material &material) {
  size_t materialIndex = std::numeric_limits<size_t>::max();

  if (const auto materialIt = std::find(begin(materials), end(materials), material); materialIt == end(materials)) {
    materials.push_back(material);
    materialIndex = materials.size() - 1;
  } else {
    materialIndex = std::distance(begin(materials), materialIt);
  }

  m_objects.emplace_back(shape, materialIndex);
}

HitResult HittableObjectList::Hit(const Ray &r, float t_min, float t_max) const {
  static constexpr HitResult empty_result{};

  float closest_t = t_max;
  const HittableObject *closest_obj = nullptr;

  for (const auto &object : m_objects) {
    if (const float temp_t = object.FastHit(r, t_min, closest_t); temp_t < std::numeric_limits<float>::max()) {
      closest_obj = &object;
      closest_t = temp_t;
    }
  }

  if (!closest_obj) {
    return empty_result;
  } else {
    const auto hitr = closest_obj->ComputeHitRecord(r, closest_t);
    return {hitr, std::visit(
                      overloaded{
                          [&](const auto &material) { return material.Scatter(r, hitr); },
                      },
                      materials[closest_obj->MaterialIndex()])};
  }
}
} // namespace RTIAW::Render
