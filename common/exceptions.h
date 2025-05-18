#pragma once

#include <exception>
#include <string>
#include <utility>

namespace renderer {

class BaseException : public std::exception {
 public:
  std::string message;

  explicit BaseException(std::string message) : message(std::move(message)) {}
};

class NotFoundException : public BaseException {
 public:
  explicit NotFoundException(std::string message) : BaseException(std::move(message)) {}
};

}  // namespace renderer
