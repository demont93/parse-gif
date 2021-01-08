#pragma once

#include <cstdlib>
#include <array>
#include <string>
#include <algorithm>
#include "BinaryFileBuf.h"

namespace parsegif {

struct Header {
  std::string signature;
  std::string version;

  [[maybe_unused]] Header(const std::string &signature, const std::string &version);
  Header(std::string &&signature, std::string &&version);
  Header(const Header &rhs) = default;
  Header(Header &&rhs) noexcept;
};

Header parse_header(BinaryFileBuf &buf2);

struct LogicalScreenDescriptor {
  int width;
  int height;
  int size_of_color_table;
  int background_color_index;
  int pixel_aspect_ratio;
  int color_resolution; // this + 1 = number of bits per primary color
  bool global_table_flag;

  LogicalScreenDescriptor(int width, int heigth, int backgroundColorIndex,
                          int pixelAspectRatio, int sizeOfColorTable,
                          int colorResolution, bool globalTableFlag);
  LogicalScreenDescriptor() = default;
};

auto parse_logical_screen_descriptor(BinaryFileBuf &buf)
-> LogicalScreenDescriptor;

struct Color {
  int red;
  int green;
  int blue;
  bool operator==(const Color& rhs) const noexcept;
};

struct ColorTable {
  std::vector<Color> data;
};

auto parse_color_table(BinaryFileBuf &buf,
                       int global_color_table_size) -> ColorTable;

struct ImageDescriptor {
  int left;
  int top;
  int width;
  int height;
  int size_of_local_color_table;
  bool interlace_flag;
  bool local_color_table_flag;
};

auto parse_image_descriptor(BinaryFileBuf &buf) -> ImageDescriptor;

enum class DisposalMethod {
  Unspecified = 0,
  NoDispose = 1,
  RestoreBG = 2,
  RestorePrev = 3
};

struct GraphicControlExtension {
  int delay_time;
  int transparent_color_index;
  DisposalMethod disposal_method;
  bool user_input_flag;
  bool transparent_color_flag;
};

auto parse_graphic_control_extension(int *&f, size_t remaining)
-> GraphicControlExtension;

struct GraphicRenderingBlock {
  GraphicControlExtension graphic_control_extension{};
  ImageDescriptor image_descriptor;
  ColorTable local_color_table{};
};

auto parse_graphic_rendering_block(BinaryFileBuf &buf) -> GraphicRenderingBlock;

struct GifDecodeData {
  Header header;
  LogicalScreenDescriptor logical_screen_descriptor;
  ColorTable global_color_table{};

  GifDecodeData(Header header, LogicalScreenDescriptor logical_screen_descriptor,
      ColorTable global_color_table);
};

GifDecodeData parse_gif_decode_data(BinaryFileBuf &buf);

struct RGBA {
  int red;
  int green;
  int blue;
  int alpha;

  RGBA() = default;
  RGBA(int red, int green, int blue, int alpha);
  RGBA(Color color, int alpha);

  auto operator==(const RGBA &rhs) const noexcept -> bool;
};


struct GifImage {
  int height;
  int width;
  std::vector<RGBA> data{};

  GifImage() = default;
  GifImage(int height, int width, const std::vector<RGBA>& data);
  GifImage(int height, int width, std::vector<RGBA>&& data);
  GifImage(const GifImage& rhs);
  GifImage(GifImage&& rhs) noexcept;
};

class Decoder;

auto process_gif_image(Decoder &decoder, BinaryFileBuf &buf)
-> GifImage;

struct Gif {
  int height;
  int width;
  std::vector<GifImage> images{};
};

Gif parse(const std::string& s);

}
