#pragma once

#include "Block.h"
#include "BinaryFileBuf.h"
#include <string>

namespace parsegif
{

class HeaderBlock : public Block
{
 public:
  HeaderBlock() = default;

  HeaderBlock(std::string signature, std::string version);

  HeaderBlock(const HeaderBlock &header_block) = default;

  HeaderBlock(HeaderBlock &&header_block) noexcept = default;

  HeaderBlock &operator=(const HeaderBlock &header_block) = default;

  HeaderBlock &operator=(HeaderBlock &&header_block) noexcept = default;

  std::string signature;
  std::string version;

  const char *get_name() override;

  int get_identifier() override;

 private:
  constexpr static const char *name{"Header"};
};

HeaderBlock parse_header(BinaryFileBuf &buf);
}