#pragma once

#include <string>
#include <memory>
#include <algorithm>

namespace parsegif
{

class Block
{
 public:
  virtual const char *get_name() = 0;

  virtual int get_identifier() = 0;
};

enum class BlockLabel {
  ImageDescriptor = 44,
  Trailer = 59,
  Extension = 33
};

enum class ExtensionLabel {
  ApplicationExtension = 255,
  CommentExtension = 254,
  GraphicControlExtension = 249,
  PlainTextExtension = 1,
};

static constexpr const std::array<std::pair<const char *, int>, 11> block_names
  {
    std::make_pair("Application Extension", 255),
    std::make_pair("Comment Extension", 254),
    std::make_pair("Global Color Table", 0),
    std::make_pair("Graphic Control Extension", 249),
    std::make_pair("Header", 0),
    std::make_pair("Image Descriptor", 44),
    std::make_pair("Local Color Table", 0),
    std::make_pair("Logical Screen Descriptor", 0),
    std::make_pair("Plain Text Extension", 1),
    std::make_pair("Trailer", 59),
    std::make_pair("Extension", 0x21)
  };

const char *get_block_name(int label);

int get_block_label(const std::string &s);
}