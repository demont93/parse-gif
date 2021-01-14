#include <sstream>
#include "Error.h"
#include "cstring"

namespace parsegif
{

IOError::IOError() : runtime_error("")
{}

const char *parsegif::FileOpenError::what() const noexcept
{
  return message.c_str();
}

FileOpenError::FileOpenError(std::string s) : message(base_message + s + ".")
{}

ParseError::ParseError() : runtime_error("")
{}

UnexpectedBlock::UnexpectedBlock(std::string expected)
  : expected(std::move(expected))
{
  message =
    std::string{"Expected a "} + expected + "block but data did not match";
}

UnexpectedBlock::UnexpectedBlock(std::string expected, int block_label)
  : expected(std::move(expected)), has_label(true), label(block_label)
{
  std::ostringstream ss{};
  ss << "Expected a " << expected << "block but found identifier "
     << block_label;
  try
  {
    found_name = get_block_name(label);
    ss << ", corresponding to block " << found_name;
  } catch (const std::runtime_error &e)
  {}
  ss << '.';
  message = ss.str();
}

const char *UnexpectedBlock::what() const noexcept
{
  return message.c_str();
}

UnsupportedFeature::UnsupportedFeature(std::string feature)
  : message(base)
{
  message += ":\n";
  message += feature;
}

const char *UnsupportedFeature::what() const noexcept
{
  return message.c_str();
}

const char *
UnsupportedVersion::what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW
{
  return "This decoder only supports version GIF89a. The version of the gif you"
         " are trying to decode is not supported.";
}

const char *UnexpectedEof::what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW
{
  return "Ecounter EOF while expecting more data.";
}

const char *
UnmatchedConstant::what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW
{
  return message.c_str();
}

UnmatchedConstant::UnmatchedConstant(std::string constant)
{
  message =
    std::string{"Expected constant value: "} + constant +
    ", which did not match.";
}

const char *
UnexpectedByteSequence::what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW
{
  return "Incoming bytes from image did not match the gif spec.";
}
}