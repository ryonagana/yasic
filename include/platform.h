#ifndef PLATFORM_H
#define PLATFORM_H




//detect CPU
#if defined(_M_IX86) || defined(__i386__) || defined(__X86__) || defined(__INTEL__)
#define CPU_X86
#define CPU_STRING "Intel x86"

#elif defined(_M_AMD64) || defined(_M_X64) || defined(__x86_64__) || defined(__amd64__)
#define CPU_X64
#define CPU_STRING "Intel x64"

#elif defined(_IA64_) || defined(__ia64__) || defined(_M_IA64) || defined(__itanium__)
#define CPU_IA64
#define CPU_STRING "Intel Itanium x64"

#elif defined(_M_ARM) || defined(__arm__) || defined(__arm)
#define CPU_ARM32
#define CPU_STRING "ARM 32 Bits"

#elif defined(_M_ARM64) || defined(__arm64__) || defined(__aarch64__)
#define CPU_ARM64
#define CPU_STRING "ARM 64 Bits"
#else
#error "Unknown CPU"
#endif


//detect platform is running on
#if defined(__NT__) || defined(__WIN32__) || defined(_WIN32) || defined(__WINDOWS__) || defined(__WIN64)
#if defined(CPU_X64)
#define PLATFORM_WIN64
#define PLATFORM_NAME "Windows 64 Bits"
#elif defined(CPU_X86)
#define PLATFORM_WIN32
#define PLATFORM_NAME "Windows 32 Bits"
#endif


#elif defined(__linux__) || defined(__LINUX__) || defined(__gnu_linux__)
#define PLATFORM_LINUX
#define PLATFORM_NAME "GNU/Linux"

#elif defined(__APPLE__) && defined(__MACH__)
#define PLATFORM_MACOSX
#define PLATFORM_NAME "MacOSX"

#elif defined(__ANDROID__)
#define PLATFORM_ANDROID
#define PLATFORM_NAME "Android"

#else
#error "Unknown Platform"
#endif


/* detect what compiler is on
   because some has some unique changes for each compiler
   example inlines, declspecs __attributes__
*/


/* is ms visual studio ? */
#if defined(_MSC_VER) || (MSC_VER)
#define GAME_COMPILER_MSVC _MSC_FULL_VER
#define GAME_COMPILER "Microsoft Visual C++"
#define GAME_COMPILER_VER GAME_COMPILER_MSVC

/* is mingw (GNU Toochain for windows) */
#elif defined(__MINGW32__)

#if defined(CPU_X64)
#define GAME_COMPILER_MINGWX86_64
#define GAME_COMPILER "MingW x86_x64"
#define GAME_COMPILER_VER (__MINGW32_MAJOR_VERSION * 10000) + (__MINGW32_MINOR_VERSION * 100)
#else
#define GAME_COMPILER_MINGW32
#define GAME_COMPILER "MingW 32 bits"
#define GAME_COMPILER_VER (__MINGW32_MAJOR_VERSION * 10000) + (__MINGW32_MINOR_VERSION * 100)
#endif



#elif defined(__GNUC__) || defined(__linux__) || defined(__unix__)
#define GAME_COMPILER_GCC
#define GAME_COMPILER "GCC"
#define GAME_COMPILER_VER (__GNUC__ * 1000) +  (__GNUC_MINOR__ * 100) + __GNUC_PATCHLEVEL__)

#elif (__clang__)
#define GAME_COMPILER_LLVM
#define GAME_COMPILER "CLANG"
#define GAME_COMPILER_VER  (__clang__ * 1000) +  (__clang_minor__ * 100) + _clang_patchlevel__)

#else
#error "unknown compiler"
#endif


#if defined(PLATFORM_WIN64) || defined(PLATFORM_WIN32)
    #if defined(GAME_COMPILER_MINGWX86_64) || defined(GAME_COMPILER_MINGW32)
        #define GAME_INLINE inline
    #elif defined(GAME_COMPILER_MSVC)
        #define GAME_INLINE __inline
    #else
        #define GAME_INLINE
    #endif
#elif defined(GAME_COMPILER_GCC)
    #define GAME_INLINE inline //inline __attribute__((always_inline))
#elif defined(GAME_COMPILER_LLVM)
    #define GAME_INLINE inline
#endif


#endif // PLATFORM_H
