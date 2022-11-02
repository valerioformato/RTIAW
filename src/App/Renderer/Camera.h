#ifndef RTIAW_camera
#define RTIAW_camera

#include "Ray.h"
#include "Utils.h"

namespace RTIAW::Render {
class Camera {
public:
  struct CameraOrientation {
    point3 lookfrom;
    point3 lookat;
    vec3 vup;
  };

  Camera(CameraOrientation orientation, float verticalFov, float aspectRatio, float aperture, float focusDist);

  void SetShutterTime(float open, float close) {
    m_shutterOpen = open;
    m_shutterClose = close;
  }

  Ray NewRay(float u, float v) const;

private:
  // viewport properties
  float m_viewportHeight{2.0f};
  float m_viewportWidth{0.0f};
  float m_lensRadius{0.5f};

  point3 m_origin{0, 0, 0};
  vec3 m_horizontal;
  vec3 m_vertical;
  vec3 u, w, v;
  point3 m_lowerLeftCorner;
  float m_shutterOpen{0};
  float m_shutterClose{1};
};
} // namespace RTIAW::Render

#endif
