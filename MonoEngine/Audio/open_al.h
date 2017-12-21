#if defined(__APPLE__)
    #include <OpenAL/al.h>
    #include <OpenAL/alc.h>
#elif defined(__linux__)
    #include <AL/al.h>
    #include <AL/alc.h>
#endif
