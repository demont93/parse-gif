#include "PlainTextExtension.h"
#include "Error.h"

namespace parsegif
{

PlainTextExtensionBlock::PlainTextExtensionBlock(
  int left, int top, int width, int height,
  int character_width, int character_height,
  int foreground_color_index, int background_color_index,
  std::vector<int> sub_data)
  : left(left), top(top), width(width), height(height),
    character_width(character_width), character_height(character_height),
    foreground_color_index(foreground_color_index),
    background_color_index(background_color_index),
    sub_data(std::move(sub_data))
{}

const char *PlainTextExtensionBlock::get_name()
{ return name; }

int PlainTextExtensionBlock::get_identifier()
{ return label; }

PlainTextExtensionBlock parse_plain_text_extension(BinaryFileBuf &buf)
{
  int block_size{buf.get_byte()};
  if (buf.is_done() || (buf.avail() < block_size && buf.fill() < block_size))
    throw UnexpectedEof();
  else if (block_size != 12)
    throw UnmatchedConstant("Block size for plain extension block = 12");
  int left{buf.get_word()};
  int top{buf.get_word()};
  int width{buf.get_word()};
  int height{buf.get_word()};
  int character_width{buf.get_byte()};
  int character_height{buf.get_byte()};
  int foreground_color_index{buf.get_byte()};
  int background_color_index{buf.get_byte()};
  auto sub_data{parse_sub_block(buf)};
  return PlainTextExtensionBlock(left, top, width, height,
                                 character_width, character_height,
                                 foreground_color_index, background_color_index,
                                 std::move(sub_data));
}
}