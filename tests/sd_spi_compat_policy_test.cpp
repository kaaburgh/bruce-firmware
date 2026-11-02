#include "../lib/HAL/sd_card/sd_spi_compat_policy.h"
#include <assert.h>
#include <stdint.h>
using sd_spi_compat::PostOcrDecision;
using sd_spi_compat::afterZeroPreInitOcr;
using sd_spi_compat::sdscSectorsFromCsd;
int main() {
  assert(afterZeroPreInitOcr(0) == PostOcrDecision::CheckSdscCsd);
  assert(afterZeroPreInitOcr(0x40000000) == PostOcrDecision::Reject);
  assert(afterZeroPreInitOcr(0x80000000) == PostOcrDecision::Reject);
  assert(afterZeroPreInitOcr(0x00100000) == PostOcrDecision::Reject);
  assert(afterZeroPreInitOcr(0x40100000) == PostOcrDecision::Reject);
  assert(afterZeroPreInitOcr(0x80100000) == PostOcrDecision::Standard);
  assert(afterZeroPreInitOcr(0xC0FF8080) == PostOcrDecision::Standard);
  uint8_t csd[16] = {0x00,0x26,0x01,0x32,0x5B,0x5A,0x83,0xC7,
                      0xF6,0xDB,0xFF,0x9F,0x16,0x80,0x40,0x19};
  assert(sdscSectorsFromCsd(csd) == 3964928UL);
  csd[0] = 0x40; assert(sdscSectorsFromCsd(csd) == 0); csd[0] = 0;
  csd[5] = 0x58; assert(sdscSectorsFromCsd(csd) == 0);
  csd[5] = 0x5C; assert(sdscSectorsFromCsd(csd) == 0);
  csd[5] = 0x5B; assert(sdscSectorsFromCsd(csd) == 0);
  csd[5] = 0x5A; assert(sdscSectorsFromCsd(csd) == 3964928UL);
}
