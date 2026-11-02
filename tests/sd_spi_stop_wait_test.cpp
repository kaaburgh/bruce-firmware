#include "../lib/HAL/sd_card/sd_spi_stop_wait.h"
#include <assert.h>
#include <stdint.h>
#include <vector>
using sd_spi_stop_wait::waitReady;

struct MockSpi {
  std::vector<uint8_t> rx;
  unsigned idx = 0;
  uint32_t tick = 0;
  uint8_t read() { return rx[idx < rx.size() ? idx++ : (unsigned)rx.size()-1]; }
  uint32_t now() { return tick++; }
};

int main() {
  MockSpi ok; ok.rx = {0x00,0x00,0xFF};
  assert(waitReady([&](){return ok.read();}, [&](){return ok.now();}, 500));
  assert(ok.idx == 3);
  MockSpi notReady; notReady.rx = {0x00,0x7F,0x80,0xFF};
  assert(waitReady([&](){return notReady.read();}, [&](){return notReady.now();}, 500));
  assert(notReady.idx == 4); // only 0xFF may indicate released busy
  MockSpi timeout; timeout.rx = {0x00};
  assert(!waitReady([&](){return timeout.read();}, [&](){return timeout.now();}, 3));
  MockSpi wrap; wrap.rx = {0x00,0x00,0xFF};
  wrap.tick = 0xFFFFFFFEu;
  assert(waitReady([&](){return wrap.read();}, [&](){return wrap.now();}, 3));
}
