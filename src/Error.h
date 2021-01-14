#pragma once

#include "Block/Block.h"
#include <stdexcept>

// Errors:
// IO Error reading file
// buf met eof
// unexpected identifier or label
// Unmatched constant
// Unexpected byte sequence
// unsupported feature
// unsupported version

namespace parsegif
{

class IOError : public std::runtime_error
{
 public:
  IOError();

  virtual const char *
  what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override = 0;
};

class FileOpenError : public IOError
{
 public:
  explicit FileOpenError(std::string s);

  const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;

 private:
  std::string message;
  static constexpr const char *base_message{"Error opening file: "};
};

class ParseError : public std::runtime_error
{
 public:
  ParseError();

  virtual const char *
  what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override = 0;
};

class UnexpectedBlock : public ParseError
{
 public:
  explicit UnexpectedBlock(std::string expected);

  UnexpectedBlock(std::string expected, int block_label);

  const char *what() const noexcept override;

 private:
  bool has_label{false};
  std::string expected;
  int label{};
  std::string found_name{};
  std::string message{};
};

class UnsupportedFeature : public ParseError
{
 public:
  UnsupportedFeature(std::string feature);

  const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;

 private:
  std::string message;
  static constexpr const char *base{
    "This image contains data not supported by this decoder. "
    "Impossible to decode it for now"};
};

class UnsupportedVersion : public ParseError
{
 public:
  UnsupportedVersion() = default;

  const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;
};

class UnexpectedEof : public ParseError
{
 public:
  UnexpectedEof() = default;

  const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;
};

class UnmatchedConstant : public ParseError
{
 public:
  explicit UnmatchedConstant(std::string constant);

  const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;

 private:
  std::string message;
};

class UnexpectedByteSequence : public ParseError
{
 public:
  UnexpectedByteSequence() = default;

  const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;
};
}

