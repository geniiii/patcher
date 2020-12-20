#include "checks.h"

#define CHECK(checkname, _address, ...)                \
	static byte check_sig_##checkname[] = __VA_ARGS__; \
	static const check checkname = {#checkname, _address, sizeof check_sig_##checkname / sizeof check_sig_##checkname[0], check_sig_##checkname}

#ifdef _WIN32
CHECK(version_check, 0x3B7150, "0.4.27.1");
#elif __linux__
CHECK(version_check, 0x4C92AD, "0.4.27");
#elif __APPLE__
CHECK(version_check, 0x47FFE2, "0.4.27");
#endif

const check* checks[] = {&version_check};
const byte num_checks = sizeof checks / sizeof checks[0];