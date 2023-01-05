//
// Created by vformato on 12/25/22.
//

#include "BVH.h"
#include "Utils.h"

#include <algorithm>

namespace RTIAW::Render {
BVHNode::BVHNode(const std::vector<HittableObject> &src_objects, size_t start, size_t end, double time0, double time1) {
  auto objects = src_objects; // Create a modifiable array of the source scene objects

  static std::mt19937 generator{std::random_device{}()};
  unsigned int axis = std::uniform_int_distribution<unsigned int>{0, 2}(generator);
  auto comparator = (axis == 0) ? box_x_compare : (axis == 1) ? box_y_compare : box_z_compare;

  size_t object_span = end - start;

  if (object_span == 1) {
    m_left = m_right = std::make_shared<NodeType>(objects[start]);
  } else if (object_span == 2) {
    if (comparator(objects[start], objects[start + 1])) {
      m_left = std::make_shared<NodeType>(objects[start]);
      m_right = std::make_shared<NodeType>(objects[start + 1]);
    } else {
      m_left = std::make_shared<NodeType>(objects[start + 1]);
      m_right = std::make_shared<NodeType>(objects[start]);
    }
  } else {
    std::sort(std::next(objects.begin(), start), std::next(objects.begin(), end), comparator);

    size_t mid = start + object_span / 2;
    m_left = std::make_shared<NodeType>(BVHNode{objects, start, mid, time0, time1});
    m_right = std::make_shared<NodeType>(BVHNode{objects, mid, end, time0, time1});
  }

  Shapes::AABB box_left, box_right;

  auto get_bounding_box = [&](const NodeType &node) {
    return std::visit(
        overloaded{
            [&](const auto &inner_node) { return inner_node.BoundingBox(); },
        },
        node);
  };
  
  if (!m_left->bounding_box(time0, time1, box_left) || !right->bounding_box(time0, time1, box_right))
    std::cerr << "No bounding box in bvh_node constructor.\n";

  box = surrounding_box(box_left, box_right);
}

float BVHNode::FastHit(const Ray &r, const float t_min, const float t_max) const {
  if (m_box.FastHit(r, t_min, t_max) == std::numeric_limits<float>::max())
    return {};

  auto get_fast_hit = [&](const NodeType &node) {
    return std::visit(
        overloaded{
            [&](const auto &inner_node) { return inner_node.FastHit(r, t_min, t_max); },
        },
        node);
  };

  float left_hit = get_fast_hit(*m_left);
  if (left_hit != std::numeric_limits<float>::max())
    return left_hit;

  auto right_hit = get_fast_hit(*m_right);
  if (right_hit != std::numeric_limits<float>::max())
    return right_hit;

  return {};
}

std::optional<HitRecord> BVHNode::Hit(const Ray &r, const float t_min, const float t_max) const {
  if (m_box.FastHit(r, t_min, t_max) == std::numeric_limits<float>::max())
    return {};

  auto get_hit = [&](const NodeType &node) {
    return std::visit(
        overloaded{
            [&](const auto &inner_node) { return inner_node.Hit(r, t_min, t_max); },
        },
        node);
  };

  auto left_hit = get_hit(*m_left);
  if (left_hit)
    return left_hit;

  auto right_hit = get_hit(*m_right);
  if (right_hit)
    return right_hit;

  return {};
}
} // namespace RTIAW::Render
