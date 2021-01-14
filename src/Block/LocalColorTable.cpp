#include "LocalColorTable.h"

namespace parsegif
{
LocalColorTableBlock::LocalColorTableBlock(ColorTableBlock color_table)
  : color_table(std::move(color_table))
{}

const char *LocalColorTableBlock::get_name()
{ return name; }

int LocalColorTableBlock::get_identifier()
{ return 0; }

LocalColorTableBlock parse_local_color_table(BinaryFileBuf &buf,
                                             int size_of_local_color_table)
{
  return parse_color_table(buf, size_of_local_color_table);
}
}
