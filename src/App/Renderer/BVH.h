//
// Created by vformato on 12/25/22.
//

#ifndef RTIAW_BVH_H
#define RTIAW_BVH_H

#include <memory>
#include <span>
#include <variant>

#include "App/Renderer/Shapes/AABB.h"
#include "App/Renderer/Shapes/Shapes.h"

#include "HittableObject.h"

namespace RTIAW::Render {

class BVHNode;
using TreeNode = std::variant<BVHNode, HittableObject>;

class BVHNode {
public:
  BVHNode() = default;
  BVHNode(const std::span<const HittableObject> list, double time0, double time1);

  [[nodiscard]] tl::optional<HitRecord> Hit(const Ray &r, const float t_min, const float t_max) const;

  [[nodiscard]] tl::optional<Shapes::AABB> BoundingBox(double time0, double time1) const;

  std::shared_ptr<TreeNode> left;
  std::shared_ptr<TreeNode> right;

  Shapes::AABB boundingBox{};
};

} // namespace RTIAW::Render
#endif // RTIAW_BVH_H
