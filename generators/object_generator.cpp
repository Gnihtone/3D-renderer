#include "object_generator.h"

#include "../common/types.h"

namespace renderer::generators {

ObjectId GenerateObjectId() {
  static size_t id = 0;
  return ObjectId{id++};
}

}  // namespace renderer::generators