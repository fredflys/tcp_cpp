#include "reassembler.hh"
#include <algorithm> // for count

using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring, Writer& output )
{
  if (!buffer_sized_) {
    buffer_.resize(output.available_capacity(), 0);
    flags_.resize(output.available_capacity(), false);
    buffer_sized_ = true;
  }

  if (is_last_substring) {
    end_index_ = first_index + data.size();
  }
  
  // reassembled in ByteStream
  first_unassembled_index_ = output.bytes_pushed();
  first_unacceptable_index_ = first_unassembled_index_ + output.available_capacity();
  
  if (!data.empty()) {
      // duplicate: alreay assembled: not even reached the unassembled index
    if (first_index + data.size() < first_unassembled_index_ 
    // out of range: already beyond the unacceptable index and therefore cannot be fit in
        || first_index > first_unacceptable_index_) {
          data = "";
    } else {
      uint64_t start = first_index;
      uint64_t end = start + data.size();

      // partly overlapped: start from the unassembled index
      if (first_index < first_unassembled_index_) {
        start = first_unassembled_index_;
      }

      // going beyond: cut off the data out of capacity
      if (first_index + data.size() > first_unacceptable_index_) {
        end = first_unacceptable_index_;
      }

      // push data into buffer and set flags
      for (size_t i = start; i < end; i++) {
        buffer_[i - first_unassembled_index_] = data[i - first_index];
        flags_[i - first_unassembled_index_] = true;
      }
    }
  }

  string assembled_data;
  while (flags_.front()) {
    assembled_data += buffer_.front();
    buffer_.pop_front();
    flags_.pop_front();
    buffer_.push_back(0);
    flags_.push_back(false);
  }

  output.push(assembled_data);

  if (output.bytes_pushed() == end_index_) {
    output.close();
  }
}

uint64_t Reassembler::bytes_pending() const
{
  return static_cast<uint64_t>(
    count(flags_.begin(), flags_.end(), true)
  );
}
