#include "checks.h"

#define CHECK(checkname, _address, ...)                \
	static byte check_sig_##checkname[] = __VA_ARGS__; \
	static const check checkname = {#checkname, _address, sizeof check_sig_##checkname / sizeof check_sig_##checkname[0], check_sig_##checkname}

#ifdef _WIN32
CHECK(version_check, 0x3AEE50, "0.4.25");
#elif defined(__linux__)
CHECK(version_check, 0x4C2A7D, "0.4.25");
#endif

const check* checks[] = {&version_check};
const byte num_checks = sizeof checks / sizeof checks[0];