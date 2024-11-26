// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Strings/Adapters/RamString.hpp>

#if ARDUINOJSON_ENABLE_STD_STREAM
#  include <ostream>
#endif

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

// A string.
// https://arduinojson.org/v7/api/jsonstring/
class JsonString {
  friend struct detail::StringAdapter<JsonString>;

 public:
  enum Ownership { Copied, Linked };

  JsonString() : str_(nullptr, 0, true) {}

  JsonString(const char* data, Ownership ownership = Copied)
      : str_(data, data ? ::strlen(data) : 0, ownership == Linked) {}

  JsonString(const char* data, size_t sz, Ownership ownership = Copied)
      : str_(data, sz, ownership == Linked) {}

  // Returns a pointer to the characters.
  const char* c_str() const {
    return str_.data();
  }

  // Returns true if the string is null.
  bool isNull() const {
    return str_.isNull();
  }

  // Returns true if the string is stored by address.
  // Returns false if the string is stored by copy.
  bool isLinked() const {
    return str_.isLinked();
  }

  // Returns length of the string.
  size_t size() const {
    return str_.size();
  }

  // Returns true if the string is non-null
  explicit operator bool() const {
    return str_.data() != 0;
  }

  // Returns true if strings are equal.
  friend bool operator==(JsonString lhs, JsonString rhs) {
    if (lhs.size() != rhs.size())
      return false;
    if (lhs.c_str() == rhs.c_str())
      return true;
    if (!lhs.c_str())
      return false;
    if (!rhs.c_str())
      return false;
    return memcmp(lhs.c_str(), rhs.c_str(), lhs.size()) == 0;
  }

  // Returns true if strings differs.
  friend bool operator!=(JsonString lhs, JsonString rhs) {
    return !(lhs == rhs);
  }

#if ARDUINOJSON_ENABLE_STD_STREAM
  friend std::ostream& operator<<(std::ostream& lhs, const JsonString& rhs) {
    lhs.write(rhs.c_str(), static_cast<std::streamsize>(rhs.size()));
    return lhs;
  }
#endif

 private:
  detail::RamString str_;
};

namespace detail {
template <>
struct StringAdapter<JsonString> {
  using AdaptedString = RamString;

  static const AdaptedString& adapt(const JsonString& s) {
    return s.str_;
  }
};
}  // namespace detail

ARDUINOJSON_END_PUBLIC_NAMESPACE
