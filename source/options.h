#if BUILD_WIN32
#define DEFAULT_FILENAME "Ripcord.exe"
#elif BUILD_LINUX
#define DEFAULT_FILENAME "Ripcord"
#else
#error Not supported
#endif
