#pragma once

#include <vector>

#include "geometry.h"
#include "types.h"

namespace renderer {
class Picture {
 public:
  Picture();
  Picture(const Width width, const Height height);

  ~Picture() = default;

  const Width GetWidth() const;
  const Height GetHeight() const;
  const Color* GetPixels() const;

  void SetWidth(const Width width);
  void SetHeight(const Height height);

  void SetPixel(const size_t y, const size_t x, const Color& color);
  void Clear(const Color color);

 private:
  void Resize();

  Width width_;
  Height height_;

  std::vector<Color> pixels_;
};

}  // namespace renderer
