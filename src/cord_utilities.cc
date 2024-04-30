#include "cord_utilities.hpp"

#include <stdexcept>

SharedPointer<Cord> ConcatCords(SharedPointer<Cord> left_cord,
                                SharedPointer<Cord> right_cord) {
  // have access to private members of cord
  if (left_cord.Get() == nullptr || right_cord.Get() == nullptr) {
    throw std::invalid_argument("invalid argument");
  }
  Cord* concat_cord = new Cord();
  SharedPointer<Cord> concat_cord_pointer = SharedPointer<Cord>(concat_cord);

  concat_cord_pointer->left_ = left_cord;
  concat_cord_pointer->right_ = right_cord;
  concat_cord_pointer->length_ = left_cord->length_ + right_cord->length_;
  return concat_cord_pointer;
}

SharedPointer<Cord> SubString(SharedPointer<Cord> curr_cord,
                              unsigned int lower_idx,
                              unsigned int upper_idx) {
  // substrings cord pointed to by curr_cord with lower_idx and upper_idx
  // lower inclusive, upper exclusive
  // is this the correct out of bounds calculation?
  if (lower_idx >= upper_idx || lower_idx < 0 ||
      upper_idx > curr_cord.Get()->Length()) {
    throw std::invalid_argument("invalid argument");
  }

  return Traverse(curr_cord, lower_idx, upper_idx);
}

SharedPointer<Cord> Traverse(SharedPointer<Cord> curr_cord,
                             unsigned int lower_idx,
                             unsigned int upper_idx) {
  if (lower_idx == 0 && upper_idx == curr_cord.Get()->Length()) {
    return curr_cord;
  }
  if (curr_cord->Left().Get() == nullptr &&
      curr_cord->Right().Get() == nullptr) {
    if (lower_idx < curr_cord.Get()->Length() &&
        upper_idx <= curr_cord.Get()->Length()) {
      char* char_array = new char[upper_idx - lower_idx + 1];
      char_array[upper_idx - lower_idx] = '\0';
      unsigned int i = 0;
      for (unsigned int j = lower_idx; j < upper_idx; ++j) {
        char_array[i] = curr_cord->Data().at(j);
        ++i;
      }
      Cord* new_substring_cord = new Cord(char_array);
      SharedPointer<Cord> pointer = SharedPointer<Cord>(new_substring_cord);
      return pointer;
    }
  }
  if (lower_idx < curr_cord->Left()->Length() &&
      upper_idx > curr_cord->Left()->Length()) {
    SharedPointer<Cord> sub1 =
        Traverse(curr_cord->Left(), lower_idx, curr_cord->Left()->Length());
    SharedPointer<Cord> sub2 = Traverse(
        curr_cord->Right(), 0, upper_idx - curr_cord->Left()->Length());
    return ConcatCords(sub1, sub2);
  }
  if (lower_idx < curr_cord->Left()->Length() &&
      upper_idx <= curr_cord->Left()->Length()) {
    return Traverse(curr_cord->Left(), lower_idx, upper_idx);
  }
  unsigned int new_lower_idx = lower_idx - curr_cord->Left()->Length();
  unsigned int new_upper_idx = upper_idx - curr_cord->Left()->Length();
  if (new_lower_idx < curr_cord->Right()->Length() &&
      new_upper_idx <= curr_cord->Right()->Length()) {
    return Traverse(curr_cord->Right(), new_lower_idx, new_upper_idx);
  }
  return curr_cord;
}

// SharedPointer<Cord> cord_substring =
//     Traverse(curr_cord->Left(), lower_idx, upper_idx);
// return cord_substring;

// SharedPointer<Cord> cord_substring =
// Traverse(curr_cord->Right(), new_lower_idx, new_upper_idx);
// return cord_substring;