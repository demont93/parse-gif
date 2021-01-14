#pragma once

#include <BinaryFileBuf.h>
#include "Block.h"

namespace parsegif
{
enum class DisposalMethod
{
  Unspecified = 0,
  NoDispose = 1,
  RestoreBG = 2,
  RestorePrev = 3
};

class GraphicControlExtensionBlock : public Block
{
 public:
  int delay_time;
  int transparent_color_index;
  DisposalMethod disposal_method;
  bool user_input_flag;
  bool transparent_color_flag;

  GraphicControlExtensionBlock() = default;

  GraphicControlExtensionBlock(int delay_time, int transparent_color_index,
                               DisposalMethod disposal_method, bool user_input_flag,
                               bool transparent_color_flag);

  GraphicControlExtensionBlock(const GraphicControlExtensionBlock &rhs) = default;

  GraphicControlExtensionBlock &operator=(const GraphicControlExtensionBlock &rhs) =
  default;

  const char *get_name() override;

  int get_identifier() override;

 private:
  static constexpr const char *name{"Graphic Control Extension"};
  static constexpr const int label{249};
};

GraphicControlExtensionBlock parse_graphic_control_extension(BinaryFileBuf &buf);
}