#if BUILD_WIN32
#define DEFAULT_FILENAME	   "Ripcord.exe"
#define DEFAULT_PATCH_FILENAME "RipcordPatched.exe"
#elif BUILD_LINUX
#define DEFAULT_FILENAME	   "Ripcord"
#define DEFAULT_PATCH_FILENAME "RipcordPatched"
#else
#error Not supported
#endif
