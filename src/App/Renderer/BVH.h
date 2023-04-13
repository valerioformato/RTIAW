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

class BVHNode;
using TreeNode = std::variant<BVHNode, HittableObject>;

class BVHNode {
public:
  BVHNode() = default;
  BVHNode(const HittableObjectList &list, double time0, double time1)
      : BVHNode(list.Objects(), 0, list.Objects().size(), time0, time1) {}

  BVHNode(const std::vector<HittableObject> &src_objects, size_t start, size_t end, double time0, double time1);

  [[nodiscard]] std::optional<HitRecord> Hit(const Ray &r, const float t_min, const float t_max) const;

  [[nodiscard]] std::optional<Shapes::AABB> BoundingBox(double time0, double time1) const;

  std::shared_ptr<TreeNode> left;
  std::shared_ptr<TreeNode> right;

  Shapes::AABB boundingBox;
};

} // namespace RTIAW::Render
#endif // RTIAW_BVH_H
