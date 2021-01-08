#include "../src/Decoder.h"
#include <gtest/gtest.h>
#include <algorithm>

TEST(Decoder, TestCodeParser) {
  using namespace parsegif;
  BinaryFileBuf buf("./data/test-image-stream");
  auto minimum_code_size{buf.get_byte()};
  auto block_size{buf.get_byte()};
  Decoder::CodeParser code_parser(buf,minimum_code_size,block_size);
  ASSERT_EQ(code_parser.next(), 4);
  ASSERT_EQ(code_parser.next(), 1);
  ASSERT_EQ(code_parser.next(), 6);
  ASSERT_EQ(code_parser.next(), 6);
  ASSERT_EQ(code_parser.next(), 2);
  code_parser.increase_code_size();
  ASSERT_EQ(code_parser.next(), 9);
  ASSERT_EQ(code_parser.next(), 9);
  ASSERT_EQ(code_parser.next(), 7);
}

TEST(Decoder, TestDecoder) {
  using namespace parsegif;
  BinaryFileBuf buf("./data/test-image-stream");
  Decoder decoder(
    Header{"", ""},
    LogicalScreenDescriptor(),
    ColorTable{std::vector<Color>{
      Color{255, 255, 255},
      Color{255, 0, 0},
      Color{0, 0, 255},
      Color{0, 255, 0}}}
  );
  auto ret = decoder.decode(GraphicRenderingBlock{}, buf);
  ASSERT_EQ(ret[0], (RGBA{255, 0, 0, 255}));
  ASSERT_EQ(ret[4], (RGBA{255, 0, 0, 255}));
  ASSERT_EQ(ret[5], (RGBA{0, 0, 255, 255}));
  ASSERT_EQ(buf.get_byte(), 0x3b);
}