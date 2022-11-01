#ifndef RTIAW_material_metal
#define RTIAW_material_metal

#include <optional>

#include "App/Renderer/HitRecord.h"
#include "App/Renderer/ScatteringRecord.h"

namespace RTIAW::Render::Materials {
class Metal {
  friend bool operator==(const Metal &lhs, const Metal &rhs);

public:
  explicit Metal(const color &albedo, const float fuzzyness)
      : m_albedo(albedo), m_fuzzyness{fuzzyness < 1.0f ? fuzzyness : 1.0f} {}

  [[nodiscard]] std::optional<ScatteringRecord> Scatter(const Ray &r_in, const HitRecord &rec) const;

private:
  color m_albedo;
  float m_fuzzyness{1.0f};
};

inline bool operator==(const Metal &lhs, const Metal &rhs) {
  return (lhs.m_albedo == rhs.m_albedo) && (lhs.m_fuzzyness && rhs.m_fuzzyness);
}
} // namespace RTIAW::Render::Materials

#endif
