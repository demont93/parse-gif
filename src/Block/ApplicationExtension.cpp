#include "ApplicationExtension.h"
#include "ParseSubBlock.h"
#include "Error.h"
#include <cassert>
#include <vector>

namespace parsegif
{
ApplicationExtensionBlock::ApplicationExtensionBlock(
  std::string identifier,
  std::array<int, 3> authentication_code,
  std::vector<int> sub_data)
  : identifier(std::move(identifier)),
    authentication_code(std::move(authentication_code)),
    sub_data(std::move(sub_data))
{}

const char *ApplicationExtensionBlock::get_name()
{
  return name;
}

int ApplicationExtensionBlock::get_identifier()
{
  return label;
}

ApplicationExtensionBlock parse_application_extension(BinaryFileBuf &buf)
{
  // parse block size (must be 11)
  int block_size{buf.get_byte()};
  if (buf.is_done())
    throw UnexpectedEof();
  else if (block_size != 11)
    throw UnmatchedConstant("Application extension block size = 11");
  else if (buf.avail() < block_size && buf.fill() < block_size)
    throw UnexpectedEof();

  // parse identifier 8b
  std::string identifier{};
  identifier.reserve(8);
  buf.copy_n(8, std::back_inserter(identifier));

  // parse auth code 3b
  std::array<int, 3> auth;
  buf.copy_n(3, auth.begin());

  // parse sub data
  //   *
  //   block size (until 0)
  //   bytes
  std::vector<int> sub_data{parse_sub_block(buf)};
  return ApplicationExtensionBlock(std::move(identifier),
                                   auth,
                                   std::move(sub_data));
}
}