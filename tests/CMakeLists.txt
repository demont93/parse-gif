include_directories(../src)
find_package(Threads REQUIRED)


add_executable(tests
        ParseGif.cpp
        ../src/ParseGif.cpp
        ../src/BinaryFileBuf.cpp
        ../src/Decoder.cpp)
target_link_libraries(tests CONAN_PKG::gtest)

add_executable(test_decoder
        TestDecoder.cpp
        ../src/Decoder.cpp
        ../src/BinaryFileBuf.cpp
        ../src/ParseGif.cpp)
target_link_libraries(test_decoder CONAN_PKG::gtest)

add_executable(test_against_png
        TestAgainstPng.cpp
        ../src/ParseGif.cpp
        ../src/BinaryFileBuf.cpp
        ../src/Decoder.cpp
        ../src/Block/ApplicationExtension.cpp
        ../src/Block/CommentExtension.cpp
        ../src/Block/ParseSubBlock.cpp
        ../src/Block/PlainTextExtension.cpp
        ../src/Block/Header.cpp
        ../src/Error.cpp
        ../src/Block/Block.cpp
        ../src/Block/LogicalScreenDescriptor.cpp
        ../src/Block/ColorTable.cpp
        ../src/Pixel.cpp
        ../src/Block/GlobalColorTable.cpp
        ../src/Block/GraphicControlExtension.cpp
        ../src/Block/ImageDescriptor.cpp
        ../src/Block/LocalColorTable.cpp
        )
target_link_libraries(test_against_png
        CONAN_PKG::gtest
        CONAN_PKG::lodepng
        Threads::Threads
        )
