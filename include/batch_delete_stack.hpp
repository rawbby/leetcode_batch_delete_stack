#pragma once

#include <algorithm>
#include <charconv>
#include <iostream>
#include <limits>
#include <string>
#include <string_view>
#include <vector>

namespace impl {

constexpr static auto gInvalidValue = std::numeric_limits<int>::max();

class BatchDeleteStack
{
  enum class Marker : int // NOLINT(*-enum-size)
  {
    LOWER = -1,
    VALUE = 0,
    UPPER = 1,
  };

  std::vector<std::pair<Marker, int>> mStack;

public:
  void push(
    int value)
  {
    mStack.emplace_back(Marker::VALUE, value);
  }

  void removeUpper(
    int upperBound)
  {
    mStack.emplace_back(Marker::UPPER, upperBound);
  }

  void removeLower(
    int lowerBound)
  {
    mStack.emplace_back(Marker::LOWER, lowerBound);
  }

  auto popBack() -> int
  {
    auto lowerBound = std::numeric_limits<int>::min();
    auto upperBound = std::numeric_limits<int>::max();

    while (!mStack.empty()) {
      auto const [marker, value] = mStack.back();
      mStack.pop_back();

      switch (marker) {
        case Marker::LOWER:
          lowerBound = std::max(lowerBound, value);
          break;
        case Marker::UPPER:
          upperBound = std::min(upperBound, value);
          break;
        case Marker::VALUE:
          if (value >= lowerBound && value <= upperBound) {
            if (lowerBound != std::numeric_limits<int>::min()) removeLower(lowerBound);
            if (upperBound != std::numeric_limits<int>::max()) removeUpper(upperBound);
            return value;
          }
      }
    }

    return gInvalidValue;
  }

  void pop()
  {
    popBack();
  }

  auto peak() -> int
  {
    auto const value = popBack();
    push(value);
    return value;
  }
};
}

inline void
solve()
{
  auto line = std::string{};

  auto const toInt = [&line](std::size_t offset) {
    auto const intString = std::string_view{ line }.substr(offset);

    auto value = int{};
    // auto const [ptr, ec] =
    std::from_chars(intString.data(), intString.data() + intString.size(), value);
    return value;
  };

  auto stack = impl::BatchDeleteStack{};
  while (std::getline(std::cin, line)) {
    // NOLINTBEGIN(*-magic-numbers)
    if (line.at(1) == 'u') stack.push(toInt(5));
    else if (line[1] == 'o') stack.pop();
    else if (line.at(7) == 'u') stack.removeUpper(toInt(13));
    else stack.removeLower(toInt(13));
    // NOLINTEND(*-magic-numbers)

    if (auto const top = stack.peak(); top == impl::gInvalidValue) std::cout << "EMPTY\n";
    else std::cout << top << '\n';
  }
}
