#pragma once

#include "Block.h"
#include "ColorTable.h"

namespace parsegif
{
class LocalColorTableBlock : public Block
{
 public:
  ColorTableBlock color_table;

  LocalColorTableBlock() = default;

  LocalColorTableBlock(ColorTableBlock color_table);

  LocalColorTableBlock(const LocalColorTableBlock &rhs) = default;

  LocalColorTableBlock(LocalColorTableBlock &&rhs) noexcept = default;

  LocalColorTableBlock &operator=(const LocalColorTableBlock &rhs) = default;

  LocalColorTableBlock &operator=(LocalColorTableBlock &&rhs) noexcept
  = default;

  const char *get_name() override;

  int get_identifier() override;

 private:
  static constexpr const char *name{"Local Color Table"};
};

LocalColorTableBlock parse_local_color_table(BinaryFileBuf &buf,
                                             int size_of_local_color_table);
}