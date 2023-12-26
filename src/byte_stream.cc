#include <stdexcept>

#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) {}

void Writer::push(string data )
{
  // Your code here.
  for (auto&& c : data) {
    if (available_capacity()) {
      buffer_.push(c);
      pushed_++;
    }
  }
}

void Writer::close()
{
  // Your code here.
  closed_ = true;
}

void Writer::set_error()
{
  // Your code here.
  error_ = true;
}

bool Writer::is_closed() const
{
  // Your code here.
  return closed_ ;
}

uint64_t Writer::available_capacity() const
{
  // Your code here.
  return capacity_ - buffer_.size();
}

uint64_t Writer::bytes_pushed() const
{
  // Your code here.
  return pushed_;
}

string_view Reader::peek() const
{
  // Your code here.
  if (buffer_.empty()) {
    return {};
  }
  return {&buffer_.front(), 1};
}

bool Reader::is_finished() const
{
  // Your code here.
  return buffer_.empty() && closed_ && !error_;
}

bool Reader::has_error() const
{
  // Your code here.
  return error_ ;
}

void Reader::pop( uint64_t len )
{
  // Your code here.
  while ( len-- > 0 && !buffer_.empty()) {
    buffer_.pop();
    popped_++;
  }
}

uint64_t Reader::bytes_buffered() const
{
  // Your code here
  return buffer_.size();
}

uint64_t Reader::bytes_popped() const
{
  // Your code here.
  return popped_;;
}
