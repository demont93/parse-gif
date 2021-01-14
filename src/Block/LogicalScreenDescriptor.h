#pragma once

#include <BinaryFileBuf.h>
#include "Block.h"

namespace parsegif
{
class LogicalScreenDescriptorBlock : public Block
{
 public:
  int width;
  int height;
  int size_of_color_table;
  int background_color_index;
  int pixel_aspect_ratio;
  int color_resolution; // this + 1 = number of bits per primary color
  bool global_table_flag;

  LogicalScreenDescriptorBlock() = default;

  LogicalScreenDescriptorBlock(int width, int heigth, int backgroundColorIndex,
                               int pixelAspectRatio, int sizeOfColorTable,
                               int colorResolution, bool globalTableFlag);

  const char *get_name() override;

  int get_identifier() override;

 private:
  static constexpr const char *name{"Logical Screen Descriptor"};
};

LogicalScreenDescriptorBlock
parse_logical_screen_descriptor(BinaryFileBuf &buf);
}