#include "Decoder.h"
#include "Error.h"
#include <cassert>
#include <fstream>
#include <iostream>

namespace parsegif
{

Decoder::Decoder(
  const HeaderBlock &header,
  const LogicalScreenDescriptorBlock &logical_screen_descriptor,
  const GlobalColorTableBlock &global_color_table
) : header(header),
    logical_screen_descriptor(logical_screen_descriptor),
    global_color_table(global_color_table)
{}

auto Decoder::initialize_dict(
  int minimum_code_size,
  const ColorTableBlock &color_table
) -> void
{
  dict.clear();
  auto size{2 + (1 << minimum_code_size)};
  for (int i{}; i < size; ++i)
    dict.push_back({i});
}

std::vector<RGBA> Decoder::decode(GraphicRenderingBlock data,
                                  BinaryFileBuf &buf)
{
  auto &color_table{data.image_descriptor.local_color_table_flag ?
                    data.local_color_table.color_table :
                    global_color_table.color_table};
  auto color_codes{get_image_data(buf, color_table,
                                  data.image_descriptor.width *
                                  data.image_descriptor.height)};
  if (buf.get_byte() != 0)
    throw UnmatchedConstant("Image data stream terminator = 0");
  return interpret_codes(std::move(color_codes), color_table, data);
}

std::vector<int> Decoder::get_image_data(BinaryFileBuf &buf,
                                         const ColorTableBlock &color_table,
                                         size_t data_size)
{
  std::vector<int> ret_value{};
  ret_value.reserve(data_size);
  CodeParser code_parser(buf, buf.get_byte());

  Code code{};
  int prev{-1};
  while (1)
  {
    code = code_parser.next();
    if (std::holds_alternative<ClearCode>(code))
    {
      code_parser.reset();
      initialize_dict(code_parser.minimum_code_size(), color_table);
      prev = -1;
      continue;
    } else if (std::holds_alternative<StopCode>(code))
    {
      break;
    } else
    {
      auto n_code{std::get<int>(code)};
      assert(n_code <= dict.size());
      if (prev != -1)
      {

        if (dict_contains_code(n_code))
          process_code_in_dict(n_code, prev, ret_value);
        else
          process_code_not_in_dict(prev, ret_value);

        if (dict_is_bigger_than_code_size(code_parser.max_code()))
          code_parser.increase_code_size();
      } else
      {
        std::ranges::copy(dict[n_code], std::back_inserter(ret_value));
      }
      prev = n_code;
    }
  }
  code_parser.dump();
  return ret_value;
}

std::vector<RGBA> Decoder::interpret_codes(
  std::vector<int> &&color_codes,
  const ColorTableBlock &color_table,
  const GraphicRenderingBlock &settings)
{
  std::vector<RGBA> ret_value{};
  ret_value.reserve(color_codes.size());

  if (settings.graphic_control_extension.transparent_color_flag)
  {
    std::ranges::transform(
      color_codes,
      std::back_inserter(ret_value),
      [&](auto code)
      {
        if (code == settings.graphic_control_extension.transparent_color_index)
          return RGBA{0, 0, 0, 0};
        else
          return color_table.data[code];
      });
  } else
  {
    std::ranges::transform(
      color_codes,
      std::back_inserter(ret_value),
      [&](auto code)
      {
        return color_table.data[code];
      });
  }

  return ret_value;
}

[[nodiscard]] auto Decoder::dict_is_bigger_than_code_size(
  int max_code
) const noexcept -> bool
{
  return (dict.size() - 1) >= max_code;
}

auto Decoder::dict_contains_code(int code) const noexcept -> bool
{
  return code < dict.size();
}

void Decoder::process_code_in_dict(int code, int prev,
                                   std::vector<int> &ret_val)
{
  if (dict.size() < 0x1000)
  {
    dict.emplace_back(dict[prev]);
    dict.back().push_back(dict[code].front());
  }
  std::ranges::copy(dict[code], std::back_inserter(ret_val));
}

void Decoder::process_code_not_in_dict(int prev, std::vector<int> &ret_val)
{
  dict.emplace_back(dict[prev]);
  dict.back().push_back(dict.back().front());
  std::ranges::copy(dict.back(), std::back_inserter(ret_val));
}

Decoder::CodeParser::CodeParser(
  BinaryFileBuf &buf,
  int minimum_code_size
) : buf(buf),
    _minimum_code_size(minimum_code_size)
{}

int Decoder::CodeParser::next_n_code()
{
  int code{};
  for (int i{}; i <= code_size(); ++i)
  {
    if (mask == 0x100) load_byte();
    code |= (static_cast<bool>(mask & byte) << i);
    mask <<= 1;
  }
  return code;
}

void Decoder::CodeParser::load_byte()
{
  if (!this->block_size)
  {
    block_size = buf.get_byte();
  }
  mask = 1;
  byte = buf.get_byte();
  --block_size;
}

void Decoder::CodeParser::reset()
{
  _code_size = _minimum_code_size;
}

void Decoder::CodeParser::increase_code_size()
{
  if (_code_size < 11) ++_code_size;
}

int Decoder::CodeParser::code_size() const noexcept
{
  return _code_size;
}

auto Decoder::CodeParser::max_code() const noexcept -> int
{
  return (1 << (code_size() + 1)) - 1;
}

auto Decoder::CodeParser::minimum_code_size() const noexcept -> int
{
  return _minimum_code_size;
}

auto Decoder::CodeParser::next() -> Code
{
  auto next{next_n_code()};
  if (next == _clear_code) return {ClearCode{}};
  else if (next == _stop_code) return {StopCode{}};
  else return {next};
}

void Decoder::CodeParser::dump()
{
  while (block_size--) buf.get_byte();
}
}