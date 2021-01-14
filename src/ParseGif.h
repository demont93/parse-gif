#pragma once

#include <cstdlib>
#include <array>
#include <string>
#include <algorithm>
#include <Block/ImageDescriptor.h>
#include <Block/LocalColorTable.h>
#include <Block/Header.h>
#include <Block/LogicalScreenDescriptor.h>
#include <Block/GlobalColorTable.h>
#include <Block/GraphicControlExtension.h>
#include "BinaryFileBuf.h"
#include "Pixel.h"

namespace parsegif
{


struct GraphicRenderingBlock
{
  GraphicControlExtensionBlock graphic_control_extension{};
  ImageDescriptorBlock image_descriptor;
  LocalColorTableBlock local_color_table{};
};

auto parse_graphic_rendering_block(BinaryFileBuf &buf) -> GraphicRenderingBlock;

struct DecoderSettings
{
  HeaderBlock header;
  LogicalScreenDescriptorBlock logical_screen_descriptor;
  GlobalColorTableBlock global_color_table{};

  DecoderSettings(HeaderBlock header,
                  LogicalScreenDescriptorBlock logical_screen_descriptor,
                  GlobalColorTableBlock global_color_table);
};

DecoderSettings parse_decoder_settings(BinaryFileBuf &buf);

struct GifImage
{
  int height;
  int width;
  int top;
  int left;
  std::vector<RGBA> data{};

  GifImage() = default;
  GifImage(int height, int width, int top, int left,
           const std::vector<RGBA> &data);
  GifImage(int height, int width, int top, int left, std::vector<RGBA> &&data);
  GifImage(const GifImage &rhs);
  GifImage(GifImage &&rhs) noexcept;
};

class Decoder;

auto process_gif_image(Decoder &decoder, BinaryFileBuf &buf)
-> GifImage;

struct Gif
{
  int height;
  int width;
  RGBA background_color;
  std::vector<GifImage> images{};
};

Gif parse(const std::string &s);
}
