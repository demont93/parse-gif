
add_library(parsegif STATIC
        ParseGif.cpp
        BinaryFileBuf.cpp
        Decoder.cpp
        Block/ApplicationExtension.cpp
        Block/CommentExtension.cpp
        Block/ParseSubBlock.cpp
        Block/PlainTextExtension.cpp
        Block/Header.cpp
        Error.cpp
        Block/Block.cpp
        Block/LogicalScreenDescriptor.cpp
        Block/ColorTable.cpp
        Pixel.cpp
        Block/GlobalColorTable.cpp
        Block/GraphicControlExtension.cpp
        Block/ImageDescriptor.cpp
        Block/LocalColorTable.cpp
        )
