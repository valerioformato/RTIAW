#include <glm/gtc/random.hpp>

#include "Materials/Dielectric.h"
#include "Materials/Lambertian.h"
#include "Materials/Metal.h"
#include "Renderer.h"
#include "Shapes/Sphere.h"

namespace RTIAW::Render {
void Renderer::SetScene(Scenes scene) {
  m_sceneType = scene;

  m_scene.Clear();

  switch (m_sceneType) {
  case Scenes::DefaultScene: {
    Camera::CameraOrientation orientation{point3(13, 2, 3), point3(0, 0, 0), vec3(0, 1, 0)};

    constexpr auto dist_to_focus = 10.0f;
    constexpr auto aperture = 0.1f;

    m_camera = std::make_unique<Camera>(orientation, 20.0f, AspectRatio(), aperture, dist_to_focus);

    m_scene.Add(Shapes::Sphere(point3(0, -1000, 0), 1000.0f), Materials::Lambertian(color(0.5, 0.5, 0.5)));

    for (int a = -11; a < 11; a++) {
      for (int b = -11; b < 11; b++) {
        const auto choose_mat = m_unifDistribution(m_rnGenerator);
        point3 center(a + 0.9 * m_unifDistribution(m_rnGenerator), 0.2, b + 0.9 * m_unifDistribution(m_rnGenerator));

        if ((center - point3(4, 0.2, 0)).length() > 0.9) {
          if (choose_mat < 0.8f) {
            // diffuse
            color randColor{m_unifDistribution(m_rnGenerator), m_unifDistribution(m_rnGenerator),
                            m_unifDistribution(m_rnGenerator)};
            auto albedo = randColor * randColor;
            m_scene.Add(Shapes::Sphere(center, 0.2f), Materials::Lambertian(albedo));
          } else if (choose_mat < 0.95f) {
            // metal
            color albedo{0.5f * (1.0f + m_unifDistribution(m_rnGenerator)),
                         0.5f * (1.0f + m_unifDistribution(m_rnGenerator)),
                         0.5f * (1.0f + m_unifDistribution(m_rnGenerator))};
            auto fuzz = 0.5f * m_unifDistribution(m_rnGenerator);
            m_scene.Add(Shapes::Sphere(center, 0.2f), Materials::Metal(albedo, fuzz));
          } else {
            // glass
            m_scene.Add(Shapes::Sphere(center, 0.2f), Materials::Dielectric(1.5f));
          }
        }
      }
    }
    m_scene.Add(Shapes::Sphere(point3(0, 1, 0), 1.0f), Materials::Dielectric(1.5f));
    m_scene.Add(Shapes::Sphere(point3(-4, 1, 0), 1.0f), Materials::Lambertian(color(0.4, 0.2, 0.1)));
    m_scene.Add(Shapes::Sphere(point3(4, 1, 0), 1.0f), Materials::Metal(color(0.7, 0.6, 0.5), 0.0f));
  } break;
  case Scenes::ThreeSpheres: {
    Camera::CameraOrientation orientation{point3(3, 3, 2), point3(0, 0, -1), vec3(0, 1, 0)};

    const auto lookDir = orientation.lookfrom - orientation.lookat;
    const auto dist_to_focus = std::sqrt(glm::dot(lookDir, lookDir));
    constexpr auto aperture = 1.0f;

    m_camera = std::make_unique<Camera>(orientation, 20.0f, AspectRatio(), aperture, dist_to_focus);

    auto R = std::cos(Utils::pi / 4);
    m_scene.Add(Shapes::Sphere(point3(0.0, -100.5, -1.0), 100.0f), Materials::Lambertian(color(0.8, 0.8, 0.0)));
    m_scene.Add(Shapes::Sphere(point3(0.0, 0.0, -1.0), 0.5f), Materials::Lambertian(color(0.1, 0.2, 0.5)));
    m_scene.Add(Shapes::Sphere(point3(-1.0, 0.0, -1.0), 0.5f), Materials::Dielectric(1.5f));
    m_scene.Add(Shapes::Sphere(point3(-1.0, 0.0, -1.0), -0.45f), Materials::Dielectric(1.5f));
    m_scene.Add(Shapes::Sphere(point3(1.0, 0.0, -1.0), 0.5f), Materials::Metal(color(0.8, 0.6, 0.2), 0.0f));
  } break;
  case Scenes::TestScene: {
    Camera::CameraOrientation orientation{point3(0, 2, 13), point3(0, 0, 0), vec3(0, 1, 0)};

    const auto lookDir = orientation.lookfrom - orientation.lookat;
    const auto dist_to_focus = std::sqrt(glm::dot(lookDir, lookDir));
    constexpr auto aperture = 0.1f;

    m_camera = std::make_unique<Camera>(orientation, 20.0f, AspectRatio(), aperture, dist_to_focus);

    auto material = Materials::Lambertian(color(0.4, 0.2, 0.1));
    m_scene.Add(Shapes::Sphere(point3(0, 0, -2), 1.0f), material);
    m_scene.Add(Shapes::Sphere(point3(-1, 0, 0), 1.0f), material);
    m_scene.Add(Shapes::Sphere(point3(1, 0, -4), 1.0f), material);
  } break;
  default:
    throw(std::runtime_error("Invalid scene selected"));
    break;
  }
}

} // namespace RTIAW::Render
