#include "ParseGif.h"
#include "BinaryFileBuf.h"
#include "Decoder.h"
#include <array>
#include <ranges>
#include <cassert>

namespace parsegif {

[[maybe_unused]] Header::Header(
  const std::string &signature,
  const std::string &version
) : signature(signature), version(version) {}

Header::Header(std::string &&signature, std::string &&version)
  : signature(std::forward<std::string>(signature)),
    version(std::forward<std::string>(version)) {}

Header::Header(parsegif::Header &&rhs) noexcept
  : signature(std::move(rhs.signature)), version(std::move(rhs.version)) {}

Header parse_header(BinaryFileBuf &buf) {
  if (buf.avail() < 6 && buf.fill() < 6)
    throw std::runtime_error("File not long enough.");
  std::string signature{};
  signature.reserve(3);
  buf.copy_n_char(3, std::back_inserter(signature));
  if (signature != "GIF") throw std::runtime_error("Invalid signature.");
  std::string version{};
  version.reserve(3);
  buf.copy_n_char(3, std::back_inserter(version));
  if (version != "89a") throw std::runtime_error("Invalid version.\n");
  return Header(std::move(signature), std::move(version));
}

LogicalScreenDescriptor::LogicalScreenDescriptor(
  int width,
  int heigth,
  int backgroundColorIndex,
  int pixelAspectRatio,
  int sizeOfColorTable,
  int colorResolution,
  bool globalTableFlag)
  : width(width), height(heigth), background_color_index(backgroundColorIndex),
    pixel_aspect_ratio(pixelAspectRatio), size_of_color_table(sizeOfColorTable),
    color_resolution(colorResolution), global_table_flag(globalTableFlag) {}

auto parse_logical_screen_descriptor(BinaryFileBuf &buf)
-> LogicalScreenDescriptor {
  if (buf.avail() < 7 && buf.fill() < 7)
    throw std::runtime_error("Image data not long enough.\n");
  auto width{buf.get_word()};
  auto height{buf.get_word()};
  int packed{buf.get_byte()};
  bool global_table_flag{static_cast<bool>(packed & 0x80)};
  auto color_resolution{((packed & 0b0111'0000) >> 4) + 1};
  auto size_of_color_table{1 << ((packed & 7) + 1)};
  auto background_color_index{buf.get_byte()};
  auto pixel_aspect_ratio{buf.get_byte()};
  return LogicalScreenDescriptor(width, height, background_color_index,
                                 pixel_aspect_ratio, size_of_color_table,
                                 color_resolution, global_table_flag);
}

bool Color::operator==(const Color &rhs) const noexcept {
  return red == rhs.red &&
         green == rhs.green &&
         blue == rhs.blue;
}

auto parse_color_table(
  BinaryFileBuf &buf,
  int global_color_table_size
) -> parsegif::ColorTable {
  if (auto bytes_needed{global_color_table_size * 3};
    buf.avail() < bytes_needed &&
    buf.fill() < bytes_needed)
    throw std::runtime_error("Image data is not big enough.\n");

  std::vector<Color> v{};
  v.reserve(global_color_table_size);
  for (int i{}; i < global_color_table_size; ++i
    ) {
    auto red{buf.get_byte()};
    auto green{buf.get_byte()};
    auto blue{buf.get_byte()};
    v.push_back(Color{red, green, blue});
  }
  return ColorTable{std::move(v)};
}

auto parse_graphic_control_extension(BinaryFileBuf &buf)
-> parsegif::GraphicControlExtension {
  auto _block_size{buf.get_byte()};
  auto packed{buf.get_byte()};
  auto disposal_method{
    static_cast<DisposalMethod>((packed & 0b0001'1100) >> 2 )};
  bool user_input_flag{static_cast<bool>(packed & 2)};
  bool transparent_color_flag{static_cast<bool>(packed & 1)};
  auto delay_time{buf.get_word()};
  auto transparent_color_index{buf.get_byte()};
  auto end_block{buf.get_byte()};
  if (end_block != 0)
    throw std::runtime_error(
      "Missing end block for graphic control extension.\n");
  return GraphicControlExtension{
    delay_time,
    transparent_color_index,
    disposal_method,
    user_input_flag,
    transparent_color_flag
  };
}

GifDecodeData::GifDecodeData(
  Header header,
  LogicalScreenDescriptor logical_screen_descriptor,
  ColorTable global_color_table
) : header(std::move(header)),
    logical_screen_descriptor(logical_screen_descriptor),
    global_color_table(std::move(global_color_table)) {}

GifDecodeData parse_gif_decode_data(BinaryFileBuf &buf) {
  auto header{parse_header(buf)};
  auto logical_screen_descriptor{parse_logical_screen_descriptor(buf)};
  ColorTable global_color_table;
  if (logical_screen_descriptor.global_table_flag) {
    global_color_table = parse_color_table(
      buf,
      logical_screen_descriptor.size_of_color_table
    );
  }
  return GifDecodeData(header, logical_screen_descriptor, global_color_table);
}

auto parse_graphic_rendering_block(BinaryFileBuf &buf)
-> GraphicRenderingBlock {
  auto graphic_control_extension{GraphicControlExtension{}};
  // Extension identifier and graphic control extension identifier.
  auto byte{buf.get_byte()};
  if (byte == 0xf9) {
    graphic_control_extension = parse_graphic_control_extension(buf);
    byte = buf.get_byte();
  }
  if (byte != 0x2c)
    throw std::runtime_error("Missing image descriptor identifier.\n");
  auto image_descriptor{parse_image_descriptor(buf)};
  auto local_color_table{ColorTable{}};
  if (image_descriptor.local_color_table_flag) {
    local_color_table =
      parse_color_table(buf, image_descriptor.size_of_local_color_table);
  }
  return GraphicRenderingBlock{graphic_control_extension, image_descriptor,
                               local_color_table};
}

Gif parse(const std::string &s) {
  BinaryFileBuf buf(s);
  std::vector<GifImage> images{};
  if (!buf.is_open()) throw std::runtime_error("file error");
  auto global_decode_data{parse_gif_decode_data(buf)};
  Decoder decoder(global_decode_data.header,
                  global_decode_data.logical_screen_descriptor,
                  global_decode_data.global_color_table);
  while (1) {
    auto peek{buf.peek_byte()};
    if (peek == 0x3b) {
      break;
    } else if (peek == 0x2c ||
               (buf.get_byte() == 0x21 && buf.peek_byte() == 0xf9)) {
      images.emplace_back(process_gif_image(decoder, buf));
    } else {
      while (buf.get_byte() != 0);
    }
  }
  return Gif{global_decode_data.logical_screen_descriptor.height,
             global_decode_data.logical_screen_descriptor.width,
             std::move(images)};
}

auto parse_image_descriptor(BinaryFileBuf &buf) -> ImageDescriptor {
  auto left{buf.get_word()};
  auto top{buf.get_word()};
  auto width{buf.get_word()};
  auto height{buf.get_word()};
  auto packed{buf.get_byte()};
  bool local_color_table_flag{static_cast<bool>(packed & 0x80)};
  bool interlace_flag{static_cast<bool>(packed & 0x40)};
  if (interlace_flag) throw std::runtime_error("Interlaced not implemented.\n");
  // Ignore sort flag.
  int size_of_local_color_table{1 << ((packed & 0b0111) + 1)};
  return ImageDescriptor{left, top, width, height, size_of_local_color_table,
                         interlace_flag, local_color_table_flag};
}

RGBA::RGBA(int red, int green, int blue, int alpha)
  : red(red), green(green), blue(blue), alpha(alpha) {}

RGBA::RGBA(Color color, int alpha)
  : red(color.red), green(color.green), blue(color.blue), alpha(alpha) {}

auto RGBA::operator==(const RGBA &rhs) const noexcept -> bool {
  return red == rhs.red &&
         blue == rhs.blue &&
         green == rhs.green &&
         alpha == rhs.alpha;
}

GifImage::GifImage(
  int height, int width, int top, int left,
  const std::vector<RGBA> &data
) : height(height), width(width), top(top), left(left), data(data) {}

GifImage::GifImage(
  int height, int width, int top, int left,
  std::vector<RGBA> &&data
) : height(height), width(width), top(top), left(left), data(std::move(data)) {}

GifImage::GifImage(const GifImage &rhs)
  : height(rhs.height), width(rhs.width), data(rhs.data) {}

GifImage::GifImage(GifImage &&rhs) noexcept
  : height(rhs.height), width(rhs.width), top(rhs.top), left(rhs.left),
    data(std::move(rhs.data)) {}

auto process_gif_image(Decoder &decoder, BinaryFileBuf &buf)
-> GifImage {
  auto graphic_rendering_block{parse_graphic_rendering_block(buf)};
  auto data{decoder.decode(graphic_rendering_block, buf)};
  return GifImage(
    graphic_rendering_block.image_descriptor.height,
    graphic_rendering_block.image_descriptor.width,
    graphic_rendering_block.image_descriptor.top,
    graphic_rendering_block.image_descriptor.left,
    std::move(data)
  );
}

}