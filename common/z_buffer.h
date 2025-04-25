#pragma once

#include <vector>

namespace renderer {

class ZBuffer {
 public:
  ZBuffer();

 private:
  std::vector<float> buffer_;
};

}  // namespace renderer
