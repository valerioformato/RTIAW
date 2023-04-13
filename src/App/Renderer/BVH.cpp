//
// Created by vformato on 12/25/22.
//

#include "BVH.h"
#include "Utils.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <algorithm>

namespace RTIAW::Render {

BVHNode::BVHNode(const std::vector<HittableObject> &src_objects, size_t start, size_t end, double time0, double time1) {
  // TODO: can we use std::span here?
  auto objects = src_objects; // Create a modifiable array of the source scene objects

  int axis = Random::uniformRand<int>(0, 2);
  auto box_compare = [axis](const HittableObject &a, const HittableObject &b) {
    auto maybe_box_a = a.BoundingBox(0, 0);
    auto maybe_box_b = b.BoundingBox(0, 0);

    if (!maybe_box_a || !maybe_box_b)
      spdlog::error("No bounding box in bvh_node constructor.");

    return maybe_box_a->A()[axis] < maybe_box_b->A()[axis];
  };

  size_t object_span = end - start;

  if (object_span == 1) {
    left = right = std::make_shared<TreeNode>(objects[start]);
  } else if (object_span == 2) {
    if (box_compare(objects[start], objects[start + 1])) {
      left = std::make_shared<TreeNode>(objects[start]);
      right = std::make_shared<TreeNode>(objects[start + 1]);
    } else {
      left = std::make_shared<TreeNode>(objects[start + 1]);
      right = std::make_shared<TreeNode>(objects[start]);
    }
  } else {
    std::sort(objects.begin() + start, objects.begin() + end, box_compare);

    auto mid = start + object_span / 2;
    left = std::make_shared<TreeNode>(BVHNode{objects, start, mid, time0, time1});
    right = std::make_shared<TreeNode>(BVHNode{objects, mid, end, time0, time1});
  }

  auto maybe_box_left = std::visit(overloaded{[&](auto &element) { return element.BoundingBox(time0, time1); }}, *left);
  auto maybe_box_right =
      std::visit(overloaded{[&](auto &element) { return element.BoundingBox(time0, time1); }}, *right);

  if (!maybe_box_left || !maybe_box_right)
    spdlog::error("No bounding box in bvh_node constructor.");

  boundingBox = Shapes::SurroundingBox(*maybe_box_left, *maybe_box_right);
}

std::optional<HitRecord> BVHNode::Hit(const Ray &r, const float t_min, float t_max) const {
  if (boundingBox.FastHit(r, t_min, t_max) < std::numeric_limits<float>::max()) {
    auto left_hit_record =
        std::visit(overloaded([&](const auto &element) { return element.Hit(r, t_min, t_max); }), *left);
    auto right_hit_record =
        std::visit(overloaded([&](const auto &element) { return element.Hit(r, t_min, t_max); }), *right);

    if (left_hit_record && right_hit_record) {
      return left_hit_record->t < right_hit_record->t ? left_hit_record : right_hit_record;
    } else if (left_hit_record) {
      return left_hit_record;
    } else if (right_hit_record) {
      return right_hit_record;
    }
  }

  return std::nullopt;
}

std::optional<Shapes::AABB> BVHNode::BoundingBox(double time0, double time1) const { return boundingBox; }
} // namespace RTIAW::Render
