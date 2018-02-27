/* copied from googletest */

bool IsUTF8TrailByte(unsigned char t) { return 0x80 <= t && t<= 0xbf; }

bool IsValidUTF8(const char* str, size_t length) {
  const unsigned char *s = reinterpret_cast<const unsigned char *>(str);

  for (size_t i = 0; i < length;) {
    unsigned char lead = s[i++];

    if (lead <= 0x7f) {
      continue;  // single-byte character (ASCII) 0..7F
    }
    if (lead < 0xc2) {
      return false;  // trail byte or non-shortest form
    } else if (lead <= 0xdf && (i + 1) <= length && IsUTF8TrailByte(s[i])) {
      ++i;  // 2-byte character
    } else if (0xe0 <= lead && lead <= 0xef && (i + 2) <= length &&
               IsUTF8TrailByte(s[i]) &&
               IsUTF8TrailByte(s[i + 1]) &&
               // check for non-shortest form and surrogate
               (lead != 0xe0 || s[i] >= 0xa0) &&
               (lead != 0xed || s[i] < 0xa0)) {
      i += 2;  // 3-byte character
    } else if (0xf0 <= lead && lead <= 0xf4 && (i + 3) <= length &&
               IsUTF8TrailByte(s[i]) &&
               IsUTF8TrailByte(s[i + 1]) &&
               IsUTF8TrailByte(s[i + 2]) &&
               // check for non-shortest form
               (lead != 0xf0 || s[i] >= 0x90) &&
               (lead != 0xf4 || s[i] < 0x90)) {
      i += 3;  // 4-byte character
    } else {
      return false;
    }
  }
  return true;
}