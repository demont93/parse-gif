#include "ColorTable.h"
#include "Error.h"

namespace parsegif
{
ColorTableBlock parse_color_table(BinaryFileBuf &buf,
                                  int global_color_table_size)
{
  if (auto bytes_needed{global_color_table_size * 3};
    buf.avail() < bytes_needed &&
    buf.fill() < bytes_needed)
  throw UnexpectedEof();

  std::vector<RGBA> v{};
  v.reserve(global_color_table_size);
  for (int i{}; i < global_color_table_size; ++i)
  {
    auto red{buf.get_byte()};
    auto green{buf.get_byte()};
    auto blue{buf.get_byte()};
    v.push_back(RGBA{red, green, blue, 255});
  }
  return ColorTableBlock{std::move(v)};
}
}
