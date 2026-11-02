// Pure busy-release polling helper for the opt-in CMD25 Stop Tran path.
#pragma once
#include <stdint.h>
namespace sd_spi_stop_wait {
// SPI SD cards indicate release from programming busy with a full 0xFF byte.
// Arbitrary nonzero bytes are not enough. Clock handling is wrap-safe.
template <typename ReadByte, typename Clock>
inline bool waitReady(ReadByte readByte, Clock now, uint32_t timeoutMs) {
  const uint32_t start = now();
  do {
    if (readByte() == 0xFF) return true;
  } while ((uint32_t)(now() - start) < timeoutMs);
  return false;
}
} // namespace sd_spi_stop_wait
