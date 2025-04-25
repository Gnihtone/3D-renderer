#include "except.h"
#include "common/exceptions.h"

#include <iostream>

namespace except {

void react() {
  try {
    throw;
  } catch(renderer::BaseException& e) {
    std::cout << "Got engine error: " << e.what() << std::endl;
  } catch(std::exception& e) {
    std::cout << "Got unexpected exception: " << e.what() << std::endl;
  }
}

}  // namespace except
