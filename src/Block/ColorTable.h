#pragma once

#include "Pixel.h"
#include "BinaryFileBuf.h"
#include <vector>

namespace parsegif
{
struct ColorTableBlock
{
  std::vector<RGBA> data;
};

ColorTableBlock parse_color_table(BinaryFileBuf &buf,
                                  int global_color_table_size);
}