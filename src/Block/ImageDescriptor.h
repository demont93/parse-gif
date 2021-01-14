#pragma once

#include "Block.h"
#include "BinaryFileBuf.h"

namespace parsegif
{
class ImageDescriptorBlock : public Block
{
 public:
  int left;
  int top;
  int width;
  int height;
  int size_of_local_color_table;
  bool interlace_flag;
  bool local_color_table_flag;

  ImageDescriptorBlock() = default;

  ImageDescriptorBlock(int left, int top, int width, int height,
                       int size_of_local_color_table,
                       bool interlace_flag,
                       bool local_color_table_flag);

  ImageDescriptorBlock(const ImageDescriptorBlock &rhs) = default;

  ImageDescriptorBlock &operator=(const ImageDescriptorBlock &rhs) = default;

  const char *get_name() override;

  int get_identifier() override;

 private:
  static constexpr const char *name{"Image Descriptor"};
  static constexpr int label{44};
};

ImageDescriptorBlock parse_image_descriptor(BinaryFileBuf &buf);
}