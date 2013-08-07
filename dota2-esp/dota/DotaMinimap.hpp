class CMinimap {
public:
  static void CreateLocationPing(Vector vector, int unknown) {
    if (create_location_ping_address_ == 0) {
      unsigned long pattern_address = (unsigned long)utils::FindPattern(
        "client.dll",
        reinterpret_cast<unsigned char*>("\xE8\x00\x00\x00\x00\x83\xC4\x10\xB0\x01\x8B\xE5"),
        "x????xxxxxxx");

      create_location_ping_address_ = utils::GetAbsoluteAddress(pattern_address);
    }


    float x = vector.x;
    float y = vector.y;
    float z = vector.z;

    __asm {
      push unknown
        push z
        push y
        push x
        call create_location_ping_address_
    }
  }
private:
  static unsigned long create_location_ping_address_;
};