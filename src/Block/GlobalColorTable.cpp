#include "GlobalColorTable.h"

namespace parsegif
{
inline const char *GlobalColorTableBlock::get_name()
{ return name; }

int GlobalColorTableBlock::get_identifier()
{ return 0; }

GlobalColorTableBlock::GlobalColorTableBlock
  (ColorTableBlock color_table)
  : color_table(std::move(color_table))
{}

GlobalColorTableBlock parse_global_color_table(parsegif::BinaryFileBuf &buf,
                                               int global_color_table_size)
{
  return GlobalColorTableBlock(parse_color_table(buf, global_color_table_size));
}
}