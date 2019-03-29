/* *to = from; rax = 8; jump *handler  */
static void *get_interp_shim (MIR_item_t from, MIR_item_t *to, void *handler) {
  static unsigned char pattern[] =
    {
     0x49, 0xba, 0, 0, 0, 0, 0, 0, 0, 0, /* 0: movabsq 0, r10 */
     0x49, 0xbb, 0, 0, 0, 0, 0, 0, 0, 0, /* 10: movabsq 0, r11 */
     0x4d, 0x89, 0x13, /* 20: mov r10, (r11) */
     0x49, 0xba, 0, 0, 0, 0, 0, 0, 0, 0, /* 23: movabsq 0, r10 */
     0x48, 0xc7, 0xc0, 0x08, 0x00, 0x00, 0x00, /* 33: mov $8, rax -- to save xmm varargs */
     0x41, 0xff, 0xe2, /* 40: jmpq   *%r10 */
    };
  uint8_t *addr = MIR_publish_code (pattern, sizeof (pattern));

  MIR_update_code (addr, 3, 2, from, 12, to, 25, handler);
  return addr;
}