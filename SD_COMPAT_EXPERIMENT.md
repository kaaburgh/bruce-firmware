# Experimental Transcend SDSC compatibility test (NOT upstream firmware)

This branch combines two independently reviewable changes:

- [PR #3](https://github.com/kaaburgh/bruce-firmware/pull/3): zero CMD58 OCR fallback, guarded by CMD8/ACMD41 and a CRC-checked SDSC CSD.
- [PR #4](https://github.com/kaaburgh/bruce-firmware/pull/4): CMD25 stop-token extra clock + busy wait.

Both flags are enabled **only on this experimental branch**, in the `lilygo-t-embed-cc1101` board configuration. Neither PR enables flags by default. Both flags apply to every SD card with the respective condition met; the CMD25 fix changes writes for *all* cards in this test image, including working SanDisk. Back up cards before testing.

## Testing

Try the failing Transcend 2 GB and a known-good SanDisk, recording baseline Bruce behavior and results with the experimental image separately.

Check mount success, multi-block writes (larger than 1 KiB), read-back file hashes, errors after power cycle, seven+ settings saves, and latency. Report card model/capacity, CMD8/CMD58/ACMD41/CSD logs, CMD13 errors, unexpected reboot, and any regressions. Don't infer that other users' "SD card not found" errors share the same cause (CMD0 errors are different).

## Flashing

The Actions artifact includes `app-0x10000.bin` (application partition only) and `merged-0x0.bin` (full firmware). For the same 16 MB ESP32-S3 partition layout, use application-only flashing at offset `0x10000` to avoid overwriting NVS/partitions. A different partition table requires special care. **Never flash the full merged image at offset 0x10000.**

These images are intended for controlled testing, not general distribution. Neither change is validated on a wide range of cards or yet merged into Bruce.
