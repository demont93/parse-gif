#target_link_libraries(timer CONAN_PKG::poco)
# Now simply link against gtest or gtest_main as needed. Eg

add_executable(tests ParseGif.cpp ../src/ParseGif.cpp ../src/BinaryFileBuf.cpp
        ../src/Decoder.cpp)
target_link_libraries(tests CONAN_PKG::gtest)
add_executable(test_decoder ../src/Decoder.cpp TestDecoder.cpp
        ../src/BinaryFileBuf.cpp ../src/ParseGif.cpp)
target_link_libraries(test_decoder CONAN_PKG::gtest)
