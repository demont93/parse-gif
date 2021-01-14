#include "Block.h"

namespace parsegif
{
int get_block_label(const std::string &s)
{
  auto pair{std::ranges::find_if(block_names, [=](const auto &pair)
  {
    return pair.first == s;
  })};
  if (pair == block_names.end())
    throw std::runtime_error(
      "Request the label of a block with an unrecognized name");
  else if (pair->second == 0)
    throw std::runtime_error(
      "Requested block label of a block that has no label definition");
  return pair->second;
}

const char *get_block_name(int label)
{
  auto pair{std::ranges::find_if(block_names, [=](const auto &pair)
  {
    return pair.second == label;
  })};
  if (pair == block_names.end() || label == 0)
    throw std::runtime_error("No block with that label.");
  return pair->first;
}
}