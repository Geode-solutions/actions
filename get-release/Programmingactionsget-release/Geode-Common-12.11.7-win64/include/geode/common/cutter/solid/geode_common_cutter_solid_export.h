
#ifndef geode_common_cutter_solid_api_H
#define geode_common_cutter_solid_api_H

#ifdef GEODE_COMMON_CUTTER_SOLID_STATIC_DEFINE
#  define geode_common_cutter_solid_api
#  define GEODE_COMMON_CUTTER_SOLID_NO_EXPORT
#else
#  ifndef geode_common_cutter_solid_api
#    ifdef geode_common_cutter_solid_EXPORTS
        /* We are building this library */
#      define geode_common_cutter_solid_api __declspec(dllexport)
#    else
        /* We are using this library */
#      define geode_common_cutter_solid_api __declspec(dllimport)
#    endif
#  endif

#  ifndef GEODE_COMMON_CUTTER_SOLID_NO_EXPORT
#    define GEODE_COMMON_CUTTER_SOLID_NO_EXPORT 
#  endif
#endif

#ifndef GEODE_COMMON_CUTTER_SOLID_DEPRECATED
#  define GEODE_COMMON_CUTTER_SOLID_DEPRECATED __declspec(deprecated)
#endif

#ifndef GEODE_COMMON_CUTTER_SOLID_DEPRECATED_EXPORT
#  define GEODE_COMMON_CUTTER_SOLID_DEPRECATED_EXPORT geode_common_cutter_solid_api GEODE_COMMON_CUTTER_SOLID_DEPRECATED
#endif

#ifndef GEODE_COMMON_CUTTER_SOLID_DEPRECATED_NO_EXPORT
#  define GEODE_COMMON_CUTTER_SOLID_DEPRECATED_NO_EXPORT GEODE_COMMON_CUTTER_SOLID_NO_EXPORT GEODE_COMMON_CUTTER_SOLID_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef GEODE_COMMON_CUTTER_SOLID_NO_DEPRECATED
#    define GEODE_COMMON_CUTTER_SOLID_NO_DEPRECATED
#  endif
#endif

#endif /* geode_common_cutter_solid_api_H */
