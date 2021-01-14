#include "Utility.h"
#include "ParseGif.h"
#include <gtest/gtest.h>
#include <lodepng.h>

// TEST(TestAgainstPng, TestSingleImage)
// {
//   auto gif{parsegif::parse("./data/homer.gif")};
//   //The image argument has width * height RGBA pixels or width * height * 4
//   // bytes
//   std::vector<u_char> data{};
//   data.reserve(gif.images[0].data.size() * 4);
//   std::ranges::for_each(gif.images[0].data, [&](parsegif::RGBA rgba)
//   {
//     data.push_back(rgba.red);
//     data.push_back(rgba.green);
//     data.push_back(rgba.blue);
//     data.push_back(rgba.alpha);
//   });
//   //Encode the image
//   unsigned error = lodepng::encode("./data/homer.png", data,
//                                    gif.images[0].width, gif.images[0].height);
//
//   //if there's an error, display it
//   if (error)
//   {
//     std::cout << "encoder error " << error << ": "
//               << lodepng_error_text(error) << std::endl;
//   }
//   ASSERT_FALSE(error);
// }

TEST(TestAgainstPng, TestAllImages)
{
  auto gif{parsegif::parse("./data/office.gif")};
  //The image argument has width * height RGBA pixels or width * height * 4
  // bytes
  std::vector<u_char> final_image{};
  final_image.reserve(gif.width * gif.height * 4);
  std::array<u_char, 4> color{(u_char) gif.background_color.red,
                              (u_char) gif.background_color.green,
                              (u_char) gif.background_color.blue,
                              0xff};

  for (int i{}, e{gif.width * gif.height}; i < e; ++i)
    std::copy_n(color.begin(), 4, std::back_inserter(final_image));

  std::string base{"./data/office/office"};
  std::ostringstream ss{};
  int counter{};
  for (const auto &image : gif.images)
  {
    auto twod{[=](int y, int x)
              { return y * gif.width + x; }};
    auto image_data{image.data.begin()};
    for (int y{}, y_e{image.height}; y < y_e; ++y)
    {
      for (int x{}, x_e{image.width}; x < x_e; ++x)
      {
        auto image_index{y * image.width + x};
        auto final_index{4 * ((y + image.top) * gif.width + (x + image.left))};
        if (image_data[image_index].alpha)
        {
          final_image[final_index] = image_data[image_index].red;
          final_image[final_index + 1] = image_data[image_index].green;
          final_image[final_index + 2] = image_data[image_index].blue;
          final_image[final_index + 3] = 0xff;
        }
      }
    }

    ss << base << ++counter << ".png";
    //Encode the image
    unsigned error = lodepng::encode(ss.str(), final_image,
                                     gif.width, gif.height);
    ss.str(""); // clear

    //if there's an error, display it
    if (error)
    {
      std::cout << "encoder error " << error << ": "
                << lodepng_error_text(error) << std::endl;
    }
    ASSERT_FALSE(error);
  }
}

// TEST(TestAgainstPng, TestAllImages)
// {
//   auto gif{parsegif::parse("./data/office.gif")};
//   //The image argument has width * height RGBA pixels or width * height * 4
//   // bytes
//   std::vector<u_char> final_image{};
//   final_image.reserve(gif.width * gif.height * 4);
//
//
//   std::string base{"./data/office/office"};
//   std::ostringstream ss{};
//   int counter{};
//   for (const auto &image : gif.images)
//   {
//
//     std::ranges::for_each(image.data, [&](parsegif::RGBA rgba)
//     {
//       final_image.push_back(rgba.red);
//       final_image.push_back(rgba.green);
//       final_image.push_back(rgba.blue);
//       final_image.push_back(rgba.alpha);
//     });
//
//     ss << base << ++counter << ".png";
//
//     //Encode the image
//     unsigned error = lodepng::encode(ss.str(), final_image,
//                                      image.width, image.height);
//
//     //if there's an error, display it
//     if (error)
//     {
//       std::cout << "encoder error " << error << ": "
//                 << lodepng_error_text(error) << std::endl;
//     }
//     ASSERT_FALSE(error);
//
//     final_image.clear();
//     ss.str(""); // clear
//   }
// }
