#ifndef RLE_COMPRESSOR_HPP
#define RLE_COMPRESSOR_HPP

#include <string>

namespace RLE {

class RLECompressor {
public:
  std::string compress(const std::string& input);
  std::string decompress(const std::string& input);
};

} // namespace RLE

#endif // RLE_COMPRESSOR_HPP
