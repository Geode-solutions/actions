
#ifndef geode_common_core_api_H
#define geode_common_core_api_H

#ifdef GEODE_COMMON_CORE_STATIC_DEFINE
#  define geode_common_core_api
#  define GEODE_COMMON_CORE_NO_EXPORT
#else
#  ifndef geode_common_core_api
#    ifdef geode_common_core_EXPORTS
        /* We are building this library */
#      define geode_common_core_api __declspec(dllexport)
#    else
        /* We are using this library */
#      define geode_common_core_api __declspec(dllimport)
#    endif
#  endif

#  ifndef GEODE_COMMON_CORE_NO_EXPORT
#    define GEODE_COMMON_CORE_NO_EXPORT 
#  endif
#endif

#ifndef GEODE_COMMON_CORE_DEPRECATED
#  define GEODE_COMMON_CORE_DEPRECATED __declspec(deprecated)
#endif

#ifndef GEODE_COMMON_CORE_DEPRECATED_EXPORT
#  define GEODE_COMMON_CORE_DEPRECATED_EXPORT geode_common_core_api GEODE_COMMON_CORE_DEPRECATED
#endif

#ifndef GEODE_COMMON_CORE_DEPRECATED_NO_EXPORT
#  define GEODE_COMMON_CORE_DEPRECATED_NO_EXPORT GEODE_COMMON_CORE_NO_EXPORT GEODE_COMMON_CORE_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef GEODE_COMMON_CORE_NO_DEPRECATED
#    define GEODE_COMMON_CORE_NO_DEPRECATED
#  endif
#endif

#endif /* geode_common_core_api_H */
