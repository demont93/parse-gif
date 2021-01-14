#include "Header.h"
#include "Error.h"
#include <stdexcept>

namespace parsegif
{

const char *HeaderBlock::get_name()
{
  return name;
}

int HeaderBlock::get_identifier()
{
  return 0;
}

HeaderBlock::HeaderBlock(std::string signature, std::string version)
  : signature(std::move(signature)), version(std::move(version))
{}

HeaderBlock parse_header(BinaryFileBuf &buf)
{
  std::string signature{};
  signature.reserve(3);
  buf.copy_n(3, std::back_inserter(signature));
  if (signature != "GIF")
    throw UnmatchedConstant("GIF");
  std::string version{};
  version.reserve(3);
  buf.copy_n(3, std::back_inserter(version));
  if (version != "89a") throw UnsupportedVersion();
  return HeaderBlock(std::move(signature), std::move(version));
}
}
