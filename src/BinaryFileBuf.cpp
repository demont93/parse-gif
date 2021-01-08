#include <stdexcept>
#include "BinaryFileBuf.h"

BinaryFileBuf::BinaryFileBuf(std::string s)
  : file(fopen(s.c_str(), "rb")) {}

bool BinaryFileBuf::is_open() const noexcept {
  return static_cast<bool>(file);
}

auto BinaryFileBuf::fill() -> int {
  auto to_read = 8192 - avail();
  int read;

  while (to_read && !file_eof()) {
    if (begin == end && is_empty) {
      begin = 0;
      read = fread(&buf, 1, 8192, file);
      to_read -= read;
      end = wrap_index(read);
    } else if (begin < end) {
      read = fread(&buf[end], 1, 8192 - end, file);
      to_read -= read;
      end = wrap_index(end + read);
    } else {
      read = fread(&buf[end], 1, end - begin, file);
      to_read -= read;
      end += read;
    }
    if (is_empty && read) is_empty = false;
    _file_eof = feof(file);
    if (ferror(file)) {
      perror("Error reading file.");
      throw std::runtime_error("Error reading file.");
    }
  }
  return 8192 - to_read;
}

int BinaryFileBuf::avail() {
  auto to{end < begin ? end + 8192 : end};
  return to - begin;
}

int BinaryFileBuf::get_byte() {
  auto ret{peek_byte()};
  if (++begin == 8192) begin = 0;
  if (begin == end) is_empty = true;
  return ret;
}

int BinaryFileBuf::get_word() {
  return get_byte() + get_byte() * 0x100;
}

BinaryFileBuf::~BinaryFileBuf() {
  fclose(file);
}

bool BinaryFileBuf::file_eof() const noexcept {
  return _file_eof;
}

bool BinaryFileBuf::is_done() const noexcept {
  return file_eof() && is_empty;
}

int BinaryFileBuf::peek_byte() {
  if (is_empty) fill();
  return static_cast<int>(buf[begin]);
}

auto BinaryFileBuf::wrap_index(int i) noexcept -> int {
  return i % 8192;
}
