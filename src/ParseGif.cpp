#include "ParseGif.h"
#include "BinaryFileBuf.h"
#include "Decoder.h"
#include "Error.h"
#include "Block/ApplicationExtension.h"
#include "Block/CommentExtension.h"
#include "Block/PlainTextExtension.h"
#include <array>

namespace parsegif
{

DecoderSettings::DecoderSettings(
  HeaderBlock header,
  LogicalScreenDescriptorBlock logical_screen_descriptor,
  GlobalColorTableBlock global_color_table
) : header(std::move(header)),
    logical_screen_descriptor(logical_screen_descriptor),
    global_color_table(std::move(global_color_table))
{}

DecoderSettings parse_decoder_settings(BinaryFileBuf &buf)
{
  auto header{parse_header(buf)};
  auto logical_screen_descriptor{parse_logical_screen_descriptor(buf)};
  GlobalColorTableBlock global_color_table;
  if (logical_screen_descriptor.global_table_flag)
  {
    global_color_table = parse_global_color_table(
      buf,
      logical_screen_descriptor.size_of_color_table
    );
  }
  return DecoderSettings(header, logical_screen_descriptor, global_color_table);
}

auto parse_graphic_rendering_block(BinaryFileBuf &buf)
-> GraphicRenderingBlock
{
  GraphicControlExtensionBlock graphic_control_extension;
  // Extension identifier and graphic control extension identifier.
  auto byte{buf.get_byte()};
  if (static_cast<ExtensionLabel>(byte) ==
      ExtensionLabel::GraphicControlExtension)
  {
    graphic_control_extension = parse_graphic_control_extension(buf);
    byte = buf.get_byte();
  }
  if (static_cast<BlockLabel>(byte) != BlockLabel::ImageDescriptor)
    throw UnexpectedBlock(
      get_block_name(static_cast<int>(BlockLabel::ImageDescriptor)));
  auto image_descriptor{parse_image_descriptor(buf)};
  LocalColorTableBlock local_color_table;
  if (image_descriptor.local_color_table_flag)
  {
    local_color_table =
      parse_local_color_table(buf, image_descriptor.size_of_local_color_table);
  }
  return GraphicRenderingBlock
    {
      .graphic_control_extension = graphic_control_extension,
      .image_descriptor = image_descriptor,
      .local_color_table = local_color_table
    };
}

Gif parse(const std::string &s)
{
  BinaryFileBuf buf(s);
  if (!buf.is_open()) throw FileOpenError(s);

  std::vector<GifImage> images{};
  auto global_decode_data{parse_decoder_settings(buf)};
  Decoder decoder(global_decode_data.header,
                  global_decode_data.logical_screen_descriptor,
                  global_decode_data.global_color_table);

  int counter{};
  bool done{false};
  while (!done)
  {
    auto peek{static_cast<BlockLabel>(buf.peek_byte())};
    if (buf.avail() < 1) throw UnexpectedEof();

    switch (peek)
    {
      case BlockLabel::Trailer:
        done = true;
        break;
      case BlockLabel::ImageDescriptor:
        images.emplace_back(process_gif_image(decoder, buf));
        break;
      case BlockLabel::Extension:
        buf.get_byte();
        switch (static_cast<ExtensionLabel>(buf.peek_byte()))
        {
          case ExtensionLabel::ApplicationExtension:
            // Ignore for now but check for errors.
            // TODO: Unimplemented
            buf.get_byte();
            parse_application_extension(buf);
            break;
          case ExtensionLabel::CommentExtension:
            ignore_comment_extension_block(buf);
            break;
          case ExtensionLabel::GraphicControlExtension:
            images.emplace_back(process_gif_image(decoder, buf));
            break;
          case ExtensionLabel::PlainTextExtension:
            // Ignore for now but check for errors.
            // TODO: Unimplemented
            parse_plain_text_extension(buf);
            break;
          default:
            throw UnexpectedByteSequence();
        }
        break;
      default:
        throw UnexpectedByteSequence();
    }
    ++counter;
  }

  auto &bg_index =
    global_decode_data.logical_screen_descriptor.background_color_index;
  // FIXME: FDUP, not symmetric
  auto bg_c = global_decode_data.logical_screen_descriptor.global_table_flag ?
              global_decode_data.global_color_table.color_table.data[bg_index] :
              RGBA{0, 0, 0, 255};
  return Gif{global_decode_data.logical_screen_descriptor.height,
             global_decode_data.logical_screen_descriptor.width,
             bg_c,
             std::move(images)};
}

GifImage::GifImage(
  int height, int width, int top, int left,
  const std::vector<RGBA> &data
) : height(height), width(width), top(top), left(left), data(data)
{}

GifImage::GifImage(
  int height, int width, int top, int left,
  std::vector<RGBA> &&data
) : height(height), width(width), top(top), left(left), data(std::move(data))
{}

GifImage::GifImage(const GifImage &rhs)
  : height(rhs.height), width(rhs.width), data(rhs.data)
{}

GifImage::GifImage(GifImage &&rhs) noexcept
  : height(rhs.height), width(rhs.width), top(rhs.top), left(rhs.left),
    data(std::move(rhs.data))
{}

GifImage process_gif_image(Decoder &decoder, BinaryFileBuf &buf)
{
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