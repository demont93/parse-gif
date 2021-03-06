#pragma once

#include <cstdlib>
#include <array>
#include <cstdio>
#include <string>

namespace parsegif
{

class BinaryFileBuf
{
 public:
  BinaryFileBuf() = default;

  BinaryFileBuf(std::string s);

  ~BinaryFileBuf();

  int get_byte();

  int peek_byte();

  int get_word();

  int avail();

  auto fill() -> int;

  template<typename I>
  requires std::output_iterator<I, int>
  I copy_n(size_t n, I out)
  {
    for (int i{}; i < n; ++i)
    {
      *(out++) = static_cast<int>(get_byte());
    }
    return out;
  }

  [[nodiscard]] bool is_open() const noexcept;

  [[nodiscard]] bool file_eof() const noexcept;

  [[nodiscard]] bool is_done() const noexcept;

 private:
  std::array<u_char, 8192> buf;
  FILE *file{nullptr};
  int begin{};
  int end{};
  bool _file_eof{false};
  bool is_empty{true};

  [[nodiscard]] static auto wrap_index(int i) noexcept -> int;
};
}