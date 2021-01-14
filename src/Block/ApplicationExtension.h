#pragma once

#include "BinaryFileBuf.h"
#include "Block.h"
#include <array>
#include <vector>
#include <string>

namespace parsegif
{

class ApplicationExtensionBlock : public Block
{
 public:
  std::string identifier;
  std::array<int, 3> authentication_code;
  std::vector<int> sub_data;

  ApplicationExtensionBlock() = default;

  ApplicationExtensionBlock(std::string identifier,
                            std::array<int, 3> authentication_code,
                            std::vector<int> sub_data);

  ApplicationExtensionBlock(const ApplicationExtensionBlock &rhs) = default;

  ApplicationExtensionBlock(ApplicationExtensionBlock &&rhs) noexcept = default;

  ApplicationExtensionBlock &operator=(const ApplicationExtensionBlock &rhs)
  = default;

  ApplicationExtensionBlock &operator=(ApplicationExtensionBlock &&rhs) noexcept
  = default;

  const char *get_name() override;

  int get_identifier() override;

 private:
  static constexpr const char *name{"Application Extension"};
  static constexpr int label{255};
};

ApplicationExtensionBlock parse_application_extension(BinaryFileBuf &buf);
}