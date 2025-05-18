#include "picture.h"

namespace renderer {

namespace {

constexpr Width kDefaultWidth{800};
constexpr Height kDefaultHeight{600};

}  // namespace

Picture::Picture() : width_(kDefaultWidth), height_(kDefaultHeight), pixels_(width_ * height_) {}

Picture::Picture(const Width width, const Height height) : width_(width), height_(height) {}

const Width Picture::GetWidth() const {
  return width_;
}

const Height Picture::GetHeight() const {
  return height_;
}

const Color* Picture::GetPixels() const {
  return pixels_.data();
}

void Picture::SetWidth(const Width width) {
  width_ = width;
  Resize();
}

void Picture::SetHeight(const Height height) {
  height_ = height;
  Resize();
}

void Picture::SetPixel(const size_t y, const size_t x, const Color& color) {
  pixels_[y * width_ + x] = color;
}

void Picture::Clear(const Color color) {
  for (int i = 0; i < width_ * height_; ++i) {
    pixels_[i] = color;
  }
}

void Picture::Resize() {
  pixels_.resize(width_ * height_);
}

}  // namespace renderer
