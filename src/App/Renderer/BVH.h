//
// Created by vformato on 12/25/22.
//

#ifndef RTIAW_BVH_H
#define RTIAW_BVH_H

#include <memory>
#include <variant>

#include "App/Renderer/Shapes/AABB.h"
#include "App/Renderer/Shapes/Shapes.h"
#include "HittableObjectList.h"

namespace RTIAW::Render {

class BVHNode {
public:
  using NodeType = std::variant<BVHNode, HittableObject>;

  BVHNode() = default;

  BVHNode(const std::vector<HittableObject> &src_objects, size_t start, size_t end, double time0, double time1);

  BVHNode(const HittableObjectList &list, double time0, double time1)
      : BVHNode(list.Objects(), 0, list.Objects().size(), time0, time1) {}

  [[nodiscard]] float FastHit(const Ray &r, const float t_min, const float t_max) const;
  [[nodiscard]] std::optional<HitRecord> Hit(const Ray &r, const float t_min, const float t_max) const;

  Shapes::AABB BoundingBox() const;

public:
  std::shared_ptr<NodeType> m_left;
  std::shared_ptr<NodeType> m_right;
  Shapes::AABB m_box;
};

Shapes::AABB BVHNode::BoundingBox() const { return m_box; }

} // namespace RTIAW::Render
#endif // RTIAW_BVH_H
