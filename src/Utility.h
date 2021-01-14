#pragma once

#include <vector>
#include <iterator>

template<typename T, typename C = std::vector<T>>
class Slice2dIter
{
 public:
  using iterator_concept = std::input_iterator_tag;
  using iterator_category = std::input_iterator_tag;
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using pointer = T *;
  using reference = T &;
 public:
  Slice2dIter() = default;
  Slice2dIter(int top, int left, int bot, int right,
              std::vector<C> *data);
  Slice2dIter(const Slice2dIter &rhs) = default;
  Slice2dIter(Slice2dIter &&rhs) noexcept = default;
  Slice2dIter &operator=(const Slice2dIter &rhs) = default;
  Slice2dIter &operator=(Slice2dIter &&rhs) noexcept = default;
  T &operator*();
  T &operator*() const;
  Slice2dIter &operator++();
  Slice2dIter operator++(int);
  bool operator==(const Slice2dIter &rhs) const;
  Slice2dIter begin();
  Slice2dIter end();

 private:
  int top{};
  int left{};
  int bot{};
  int right{};
  int x{left};
  int y{top};
  std::vector<C> *data{nullptr};

  Slice2dIter(int bot, std::vector<C> *data);
};

template<typename T, typename C>
T &Slice2dIter<T, C>::operator*()
{
  return (*data)[y][x];
}

template<typename T, typename C>
Slice2dIter<T, C> &Slice2dIter<T, C>::operator++()
{
  ++x;
  if (x == right)
  {
    x = left;
    ++y;
  }
  return *this;
}

template<typename T, typename C>
Slice2dIter<T, C>::Slice2dIter(
  int top,
  int left,
  int bot,
  int right,
  std::vector<C> *data
) : top(top), left(left), bot(bot), right(right), data(data)
{}

template<typename T, typename C>
bool Slice2dIter<T, C>::operator==(const Slice2dIter &rhs) const
{
  return y == rhs.y && data == data;
}

template<typename T, typename C>
Slice2dIter<T, C> Slice2dIter<T, C>::begin()
{
  return Slice2dIter(*this);
}

template<typename T, typename C>
Slice2dIter<T, C> Slice2dIter<T, C>::end()
{
  return Slice2dIter(bot, data);
}

template<typename T, typename C>
Slice2dIter<T, C>::Slice2dIter(int bot, std::vector<C> *data)
  : top{}, left{}, bot{}, right{}, x{right}, y{bot}, data(data)
{}

template<typename T, typename C>
Slice2dIter<T, C> Slice2dIter<T, C>::operator++(int)
{
  auto temp{*this};
  this->operator++();
  return temp;
}

template<typename T, typename C>
T &Slice2dIter<T, C>::operator*() const
{
  return (*data)[x][y];
}
