#include "gtest/gtest.h"
#include "../src/ParseGif.h"
#include <fstream>

//
//TEST(ParseGif, test_parse_gif) {
//  BinaryFileBuf buf("./data/sample_1.gif");
//  auto gif{parsegif::parse_gif_decode_data(buf)};
//  ASSERT_EQ(gif.header.signature, "GIF");
//  ASSERT_EQ(gif.header.version, "89a");
//  ASSERT_TRUE(gif.logical_screen_descriptor.global_table_flag);
//  ASSERT_EQ(gif.logical_screen_descriptor.size_of_color_table, 4);
//  ASSERT_EQ(gif.logical_screen_descriptor.color_resolution, 2);
//  ASSERT_EQ(gif.logical_screen_descriptor.background_color_index, 0);
//  ASSERT_EQ(gif.logical_screen_descriptor.height, 10);
//  ASSERT_EQ(gif.logical_screen_descriptor.width, 10);
//  ASSERT_EQ(gif.global_color_table.data.size(), 4);
//  ASSERT_EQ(gif.global_color_table.data[0], (parsegif::Color{255,255,255}));
//  ASSERT_EQ(gif.global_color_table.data[1], (parsegif::Color{255,0,0}));
//  ASSERT_EQ(gif.global_color_table.data[2], (parsegif::Color{0, 0,255}));
//  ASSERT_EQ(gif.global_color_table.data[3], (parsegif::Color{0,0,0}));
//}
//
TEST(ParseGif, TestParse) {
//  auto gif{parsegif::parse("./data/sample_1.gif")};
//  ASSERT_EQ(gif.width, 10);
//  ASSERT_EQ(gif.height, 10);
//  ASSERT_EQ(gif.images.size(), 1);
//  ASSERT_EQ(gif.images[0].data.size(), 100);
}

TEST(AnimatedGif, TestParse) {
//  auto gif{parsegif::parse("./data/sample_2_animation.gif")};
//  ASSERT_EQ(gif.width, 11);
//  ASSERT_EQ(gif.height, 29);
//  ASSERT_EQ(gif.images.size(), 3);
//  std::ranges::for_each(gif.images[0].data, [](auto rgba) {
//    std::cout << rgba.red << ' ' << rgba.green << ' ' << rgba.blue
//              << ' ' << rgba.alpha << '\n';
//  });
}

TEST(AnimatedGif, TestParse2) {
  auto gif{parsegif::parse("./data/animated2.gif")};
  ASSERT_EQ(gif.width, 256);
//  ASSERT_EQ(gif.height, 29);
//  ASSERT_EQ(gif.images.size(), 3);
//  std::ranges::for_each(gif.images[0].data, [](auto rgba) {
//    std::cout << rgba.red << ' ' << rgba.green << ' ' << rgba.blue
//              << ' ' << rgba.alpha << '\n';
//  });
}
