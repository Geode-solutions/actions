/*
 * Copyright (c) 2019 - 2021 Geode-solutions. All rights reserved.
 */

#pragma once

#include <geode/basic/common.h>
#include <geode/common/core/geode_common_core_export.h>

namespace geode
{
    class geode_common_core_api LicensedModule
    {
    public:
        LicensedModule( const char* name );

    private:
        void check_license();

    private:
        const std::string name_;
    };
} // namespace geode

#define OPENGEODE_LICENSED_LIBRARY_INITIALIZE( library_name )                  \
    class lib_library_name : public geode::LicensedModule                      \
    {                                                                          \
    public:                                                                    \
        lib_library_name( const char* name ) : LicensedModule( name )          \
        {                                                                      \
            initialize();                                                      \
        }                                                                      \
                                                                               \
    private:                                                                   \
        void initialize();                                                     \
    };                                                                         \
    lib_library_name library_name_instance( #library_name ); /* NOLINT */      \
    void lib_library_name::initialize()

namespace geode
{
    void geode_common_core_api initialize_common_core();
} // namespace geode