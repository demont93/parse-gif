#include "LogicalScreenDescriptor.h"
#include "Error.h"

namespace parsegif
{

const char *LogicalScreenDescriptorBlock::get_name()
{ return name; }

int LogicalScreenDescriptorBlock::get_identifier()
{ return 0; }

LogicalScreenDescriptorBlock::LogicalScreenDescriptorBlock(
  int width,
  int heigth,
  int backgroundColorIndex,
  int pixelAspectRatio,
  int sizeOfColorTable,
  int colorResolution,
  bool globalTableFlag)
  : width(width), height(heigth), background_color_index(backgroundColorIndex),
    pixel_aspect_ratio(pixelAspectRatio), size_of_color_table(sizeOfColorTable),
    color_resolution(colorResolution), global_table_flag(globalTableFlag)
{}

LogicalScreenDescriptorBlock parse_logical_screen_descriptor(BinaryFileBuf &buf)
{
  if (auto block_size{7}; buf.avail() < block_size && buf.fill() < block_size)
    throw UnexpectedEof();
  auto width{buf.get_word()};
  auto height{buf.get_word()};
  int packed{buf.get_byte()};
  bool global_table_flag{static_cast<bool>(packed & 0x80)};
  auto color_resolution{((packed & 0b0111'0000) >> 4) + 1};
  auto size_of_color_table{1 << ((packed & 7) + 1)};
  auto background_color_index{buf.get_byte()};
  auto pixel_aspect_ratio{buf.get_byte()};
  return LogicalScreenDescriptorBlock(width, height, background_color_index,
                                      pixel_aspect_ratio, size_of_color_table,
                                      color_resolution, global_table_flag);
}
}
