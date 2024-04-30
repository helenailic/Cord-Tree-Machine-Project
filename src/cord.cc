#include "cord.hpp"

#include <stdexcept>

#include "cord_utilities.hpp"

Cord::Cord(std::string_view sv) {
  // constructs new leaf cord whose data_ is deep copy of sv.data()
  if (sv.empty()) {
    throw std::invalid_argument("invalid argument");
  }
  // allocating the space for the data char array
  data_ = new char[sv.size() + 1];
  data_[sv.size()] = '\0';
  for (unsigned int i = 0; i < sv.size(); ++i) {
    data_[i] = sv[i];
  }
  left_ = SharedPointer<Cord>();
  right_ = SharedPointer<Cord>();
  length_ = sv.size();
}

Cord::~Cord() {
  delete[] data_;
  data_ = nullptr;
  length_ = 0;
}

SharedPointer<char> Cord::ToString() const {
  // base case
  if (left_.Get() == nullptr && right_.Get() == nullptr && data_ != nullptr) {
    char* char_array = new char[length_ + 1];
    for (unsigned int i = 0; i <= length_; ++i) {
      char_array[i] = data_[i];
    }
    auto pointer = SharedPointer<char>(char_array);
    return pointer;
  }

  // recursive steps
  SharedPointer<char> left_subtree = left_.Get()->ToString();
  SharedPointer<char> right_subtree = right_.Get()->ToString();
  char* char_array = new char[left_->Length() + right_->Length() + 1];
  char_array[left_->Length() + right_->Length()] = '\0';

  // populate the array
  for (unsigned int i = 0; i < left_->Length(); ++i) {
    char_array[i] = left_subtree.Get()[i];
  }
  for (unsigned int i = 0; i <= right_->Length(); ++i) {
    // start from where the left population ended
    char_array[i + left_->Length()] = right_subtree.Get()[i];
  }

  auto pointer = SharedPointer<char>(char_array);
  return pointer;
}