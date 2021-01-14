#pragma once

#include <variant>
#include "ParseGif.h"

namespace parsegif
{

class Decoder
{
 public:
  class CodeParser;

 public:
  Decoder(const HeaderBlock &header,
          const LogicalScreenDescriptorBlock &logical_screen_descriptor,
          const GlobalColorTableBlock &global_color_table);

  std::vector<RGBA> decode(GraphicRenderingBlock data,
                           BinaryFileBuf &buf);

 private:
  HeaderBlock header;
  LogicalScreenDescriptorBlock logical_screen_descriptor;
  GlobalColorTableBlock global_color_table{};
  std::vector<std::vector<int>> dict{};

  void
  initialize_dict(int minimum_code_size, const ColorTableBlock &color_table);

  std::vector<int> get_image_data(BinaryFileBuf &buf,
                                  const ColorTableBlock &color_table,
                                  size_t data_size);

  std::vector<RGBA> interpret_codes(std::vector<int> &&color_codes,
                                    const ColorTableBlock &color_table,
                                    const GraphicRenderingBlock &settings);

  void process_code_in_dict(int code, int prev, std::vector<int> &ret_val);

  void process_code_not_in_dict(int prev, std::vector<int> &ret_val);

  [[nodiscard]] bool dict_contains_code(int code) const noexcept;

  [[nodiscard]] bool dict_is_bigger_than_code_size(int max_code) const noexcept;
};

struct ClearCode
{
};
struct StopCode
{
};

using Code = std::variant<StopCode, ClearCode, int>;

class Decoder::CodeParser
{
 public:
  CodeParser(BinaryFileBuf &buf, int minimum_code_size);

  auto next() -> Code;

  auto reset() -> void;

  auto increase_code_size() -> void;

  [[nodiscard]] auto code_size() const noexcept -> int;

  [[nodiscard]] auto max_code() const noexcept -> int;

  [[nodiscard]] int minimum_code_size() const noexcept;

  void dump();

 private:
  BinaryFileBuf &buf;
  int _minimum_code_size;
  int _clear_code{1 << minimum_code_size()};
  int _stop_code{_clear_code + 1};
  int block_size{};
  int _code_size{_minimum_code_size};
  int byte{};
  int mask{0x100};

  auto next_n_code() -> int;

  auto load_byte() -> void;
};
}