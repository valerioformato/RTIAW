#ifndef RTIAW_material_lambertian
#define RTIAW_material_lambertian

#include <optional>

#include "HitRecord.h"
#include "ScatteringRecord.h"

namespace RTIAW::Render::Materials {
class Lambertian {
  friend bool operator==(const Lambertian &lhs, const Lambertian &rhs);

public:
  explicit Lambertian(const color &albedo) : m_albedo(albedo) {}

  [[nodiscard]] std::optional<ScatteringRecord> Scatter(const Ray &r_in, const HitRecord &rec) const;

public:
  color m_albedo;
};

inline bool operator==(const Lambertian &lhs, const Lambertian &rhs) { return lhs.m_albedo == rhs.m_albedo; }
} // namespace RTIAW::Render::Materials

#endif
