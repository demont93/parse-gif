#pragma once

#include "BinaryFileBuf.h"
#include "ParseSubBlock.h"
#include "Block.h"

namespace parsegif
{

class PlainTextExtensionBlock : public Block
{
 public:
  int left;
  int top;
  int width;
  int height;
  int character_width;
  int character_height;
  int foreground_color_index;
  int background_color_index;
  std::vector<int> sub_data;

  PlainTextExtensionBlock() = default;

  PlainTextExtensionBlock(int left, int top, int width, int height,
                          int character_width, int character_height,
                          int foreground_color_index,
                          int background_color_index,
                          std::vector<int> sub_data);

  PlainTextExtensionBlock(const PlainTextExtensionBlock &rhs) = default;

  PlainTextExtensionBlock(PlainTextExtensionBlock &&rhs) noexcept = default;

  PlainTextExtensionBlock &
  operator=(const PlainTextExtensionBlock &rhs) = default;

  PlainTextExtensionBlock &
  operator=(PlainTextExtensionBlock &&rhs) noexcept = default;

  const char *get_name() override;

  int get_identifier() override;

 private:
  static constexpr const char *name{"Plain Text Extension"};
  static constexpr int label{1};
};

PlainTextExtensionBlock parse_plain_text_extension(BinaryFileBuf &buf);
}