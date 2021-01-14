#pragma once

#include "BinaryFileBuf.h"
#include <vector>

namespace parsegif
{
std::vector<int> parse_sub_block(BinaryFileBuf &buf);

void ignore_sub_block(BinaryFileBuf &buf);

namespace
{
int get_block_size(BinaryFileBuf& buf);
}
}