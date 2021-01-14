#include "GraphicControlExtension.h"
#include "Error.h"

namespace parsegif
{
const char *GraphicControlExtensionBlock::get_name()
{ return name; }

int GraphicControlExtensionBlock::get_identifier()
{ return label; }

GraphicControlExtensionBlock::GraphicControlExtensionBlock(int delay_time,
                                                           int transparent_color_index,
                                                           DisposalMethod disposal_method,
                                                           bool user_input_flag,
                                                           bool transparent_color_flag)
  : delay_time(delay_time), transparent_color_index(transparent_color_index),
    disposal_method(disposal_method), user_input_flag(user_input_flag),
    transparent_color_flag(transparent_color_flag)
{}

GraphicControlExtensionBlock parse_graphic_control_extension(BinaryFileBuf &buf)
{
  if (auto block_size{5}; buf.avail() < block_size && buf.fill() < block_size)
    throw UnexpectedEof();
  if (buf.get_byte() != 4)
    throw UnmatchedConstant("Graphic Control Extension block size = 4");
  auto packed{buf.get_byte()};
  auto disposal_method{
    static_cast<DisposalMethod>((packed & 0b0001'1100) >> 2 )};
  bool user_input_flag{static_cast<bool>(packed & 2)};
  bool transparent_color_flag{static_cast<bool>(packed & 1)};
  auto delay_time{buf.get_word()};
  auto transparent_color_index{buf.get_byte()};
  if (buf.get_byte() != 0)
    throw UnmatchedConstant("Block terminator for graphic control extension");
  return GraphicControlExtensionBlock(delay_time,
                                      transparent_color_index,
                                      disposal_method,
                                      user_input_flag,
                                      transparent_color_flag);
}
}