#include "ImageDescriptor.h"
#include "Error.h"

namespace parsegif
{
ImageDescriptorBlock::ImageDescriptorBlock(
  int left, int top, int width, int height,
  int size_of_local_color_table, bool interlace_flag,
  bool local_color_table_flag)
  : left(left), top(top), width(width), height(height),
    size_of_local_color_table(size_of_local_color_table),
    interlace_flag(interlace_flag),
    local_color_table_flag(local_color_table_flag)
{}

const char *ImageDescriptorBlock::get_name()
{
  return name;
}

int ImageDescriptorBlock::get_identifier()
{
  return label;
}

ImageDescriptorBlock parse_image_descriptor(BinaryFileBuf &buf)
{
  if (int block_size{9}; buf.avail() < block_size &&
                         buf.fill() < block_size)
    throw UnexpectedEof();
  auto left{buf.get_word()};
  auto top{buf.get_word()};
  auto width{buf.get_word()};
  auto height{buf.get_word()};
  auto packed{buf.get_byte()};
  bool local_color_table_flag{static_cast<bool>(packed & 0x80)};
  bool interlace_flag{static_cast<bool>(packed & 0x40)};
  if (interlace_flag)
    throw UnsupportedFeature("Interlaced image");
  // Ignore sort flag.
  // Ignore reserved.
  int size_of_local_color_table{1 << ((packed & 0b0111) + 1)};
  return ImageDescriptorBlock(left, top, width, height,
                              size_of_local_color_table,
                              interlace_flag, local_color_table_flag);
}
}
