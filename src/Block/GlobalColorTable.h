#pragma once

#include "Block.h"
#include "ColorTable.h"

namespace parsegif
{

class GlobalColorTableBlock : public Block
{
 public:
  ColorTableBlock color_table;

  GlobalColorTableBlock() = default;

  GlobalColorTableBlock(ColorTableBlock color_table);

  GlobalColorTableBlock(const GlobalColorTableBlock &rhs) = default;

  GlobalColorTableBlock(GlobalColorTableBlock &&rhs) noexcept = default;

  GlobalColorTableBlock &operator=(const GlobalColorTableBlock &rhs) = default;

  GlobalColorTableBlock &operator=(GlobalColorTableBlock &&rhs)
  noexcept = default;

  const char *get_name() override;

  int get_identifier() override;

 private:
  constexpr static const char *name{"Global Color Table"};
};

GlobalColorTableBlock parse_global_color_table(BinaryFileBuf &buf,
                                               int size_of_global_color_table);
}