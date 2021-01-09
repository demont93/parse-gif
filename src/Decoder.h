#pragma once

#include <variant>
#include "ParseGif.h"

namespace parsegif {

class Decoder {
 public:
  class CodeParser;

 public:
  Decoder(const Header &header,
          const LogicalScreenDescriptor &logical_screen_descriptor,
          const ColorTable &global_color_table);
  std::vector<RGBA> decode(GraphicRenderingBlock data,
                           BinaryFileBuf &buf);

 private:
  Header header;
  LogicalScreenDescriptor logical_screen_descriptor;
  ColorTable global_color_table{};
  std::vector<std::vector<int>> dict{};

  void initialize_dict(int minimum_code_size, const ColorTable &color_table);
  auto get_image_data(BinaryFileBuf &buf,
                      const ColorTable &color_table,
                      size_t data_size) -> std::vector<int>;
  std::vector<RGBA> interpret_codes(std::vector<int> &&color_codes,
                                    const ColorTable &color_table,
                                    const GraphicRenderingBlock &settings);
  auto process_code_in_dict(int code, int prev, std::vector<int> &ret_val)
  -> void;
  auto process_code_not_in_dict(int prev, std::vector<int> &ret_val)
  -> void;
  [[nodiscard]] auto dict_contains_code(int code) const noexcept -> bool;
  [[nodiscard]] auto dict_is_bigger_than_code_size(int max_code) const noexcept
  -> bool;
};

struct ClearCode {
};
struct StopCode {
};

using Code = std::variant<StopCode, ClearCode, int>;

class Decoder::CodeParser {
 public:
  CodeParser(BinaryFileBuf &buf, int minimum_code_size);
  auto next() -> Code;
  auto reset() -> void;
  auto increase_code_size() -> void;
  [[nodiscard]] auto code_size() const noexcept -> int;
  [[nodiscard]] auto max_code() const noexcept -> int;
  [[nodiscard]] auto minimum_code_size() const noexcept -> int;

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