#include "ParseSubBlock.h"
#include "Error.h"

namespace parsegif
{
std::vector<int> parse_sub_block(BinaryFileBuf &buf)
{
  std::vector<int> sub_data{};
  int block_size{get_block_size(buf)};
  while (block_size)
  {
    sub_data.reserve(sub_data.capacity() + block_size);
    buf.copy_n(block_size, std::back_inserter(sub_data));
    block_size = get_block_size(buf);
  }
  return sub_data;
}

void ignore_sub_block(BinaryFileBuf &buf)
{
  int block_size{get_block_size(buf)};
  while (block_size)
  {
    for (int i{}; i < block_size; ++i) buf.get_byte();
    block_size = get_block_size(buf);
  }
}

namespace
{
int get_block_size(BinaryFileBuf &buf)
{
  int block_size{buf.get_byte()};
  if (buf.is_done() || (buf.avail() < block_size && buf.fill() < block_size))
    throw UnexpectedEof();
  return block_size;
}
}
}