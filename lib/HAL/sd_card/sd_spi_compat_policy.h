// Pure SD SPI compatibility policy, testable without Arduino hardware.
#pragma once
#include <stdint.h>
namespace sd_spi_compat {
enum class PostOcrDecision : uint8_t { Reject, Standard, CheckSdscCsd };
// Only invoked when pre-init CMD58 returned R1=1/OCR=0 after valid CMD8,
// and ACMD41 reached ready. Subsequent nonzero OCR must be independently valid.
inline PostOcrDecision afterZeroPreInitOcr(uint32_t ocr) {
  if (ocr == 0) return PostOcrDecision::CheckSdscCsd;
  constexpr uint32_t required = (1UL << 20) | (1UL << 31);
  return (ocr & required) == required ? PostOcrDecision::Standard : PostOcrDecision::Reject;
}
// Call only with CRC-validated CMD9 CSD bytes; zero means reject.
inline uint32_t sdscSectorsFromCsd(const uint8_t c[16]) {
  const unsigned structure = c[0] >> 6;
  const unsigned readBlLen = c[5] & 0x0F;
  if (structure != 0 || readBlLen < 9 || readBlLen > 11) return 0;
  const uint32_t cSize = ((uint32_t)(c[6] & 3) << 10)
                       | ((uint32_t)c[7] << 2) | ((c[8] & 0xC0) >> 6);
  const unsigned cSizeMult = ((c[9] & 3) << 1) | ((c[10] & 0x80) >> 7);
  const uint64_t sectors = ((uint64_t)cSize + 1) << (cSizeMult + 2 + readBlLen - 9);
  return (sectors > 0 && sectors <= 4194304ULL) ? (uint32_t)sectors : 0;
}
} // namespace sd_spi_compat
