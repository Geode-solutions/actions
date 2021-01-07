/*
 * Copyright (c) 2019 - 2021 Geode-solutions. All rights reserved.
 */

#pragma once

#include <absl/algorithm/container.h>
#include <absl/container/fixed_array.h>
#include <absl/container/flat_hash_map.h>
#include <absl/container/flat_hash_set.h>
#include <absl/container/inlined_vector.h>
#include <absl/types/span.h>

#include <geode/basic/algorithm.h>

#include <geode/common/modifier/common/common.h>

#include <geode/basic/logger.h>

namespace geode
{
    enum struct SplitSide
    {
        left,
        right
    };

    template < typename Type >
    struct Mapping
    {
        Mapping() = default;
        Mapping( Type new_id_in, Type old_id_in )
            : new_id( std::move( new_id_in ) ), old_id( std::move( old_id_in ) )
        {
        }
        bool operator==( const Mapping& other ) const
        {
            return new_id == other.new_id && old_id == other.old_id;
        }
        bool operator<( const Mapping& other ) const
        {
            if( new_id == other.new_id )
            {
                return old_id < other.old_id;
            }
            return new_id < other.new_id;
        }
        Type new_id;
        Type old_id;
    };

    template < typename Type >
    struct MultiMapping
    {
        MultiMapping() = default;
        MultiMapping( Type new_id_in, absl::InlinedVector< Type, 1 > old_id_in )
            : new_id( std::move( new_id_in ) ), old_id( std::move( old_id_in ) )
        {
        }
        MultiMapping( Mapping< Type > mapping )
            : new_id( std::move( mapping.new_id ) ),
              old_id{ std::move( mapping.old_id ) }
        {
        }
        bool operator==( const MultiMapping& other ) const
        {
            return new_id == other.new_id && old_id == other.old_id;
        }
        Type new_id;
        absl::InlinedVector< Type, 1 > old_id;
    };

    struct SplitInfo
    {
        SplitInfo() = default;
        SplitInfo( index_t new_id_in, index_t old_id_in, SplitSide side_in )
            : mapping{ new_id_in, old_id_in }, side( side_in )
        {
        }
        bool operator==( const SplitInfo& other ) const
        {
            return mapping == other.mapping && side == other.side;
        }
        bool operator<( const SplitInfo& other ) const
        {
            if( mapping == other.mapping )
            {
                return side < other.side;
            }
            return mapping < other.mapping;
        }
        Mapping< index_t > mapping;
        SplitSide side;
    };

    absl::FixedArray< Mapping< index_t > >
        geode_common_modifier_common_api extract_mappings_from_split_info(
            absl::Span< const SplitInfo > split_info );

    template < typename T >
    void add_mappings_in_multi_mappings(
        std::vector< MultiMapping< T > >& multi_mappings,
        absl::Span< const Mapping< T > > new_mappings )
    {
        for( const auto& mapping : new_mappings )
        {
            const auto& new_id = mapping.new_id;
            auto it = absl::c_find_if(
                multi_mappings, [&new_id]( const MultiMapping< T >& value ) {
                    return value.new_id == new_id;
                } );
            if( it == multi_mappings.end() )
            {
                multi_mappings.emplace_back(
                    MultiMapping< T >{ mapping.new_id, { mapping.old_id } } );
            }
            else
            {
                it->old_id.push_back( mapping.old_id );
            }
        }
    }

    template < typename T >
    void process_mapping_element( geode::Mapping< T >& element,
        absl::flat_hash_set< T >& to_remove,
        absl::flat_hash_map< T, T >& new_to_old,
        const T& no_value )
    {
        geode_unused( no_value );
        auto it = new_to_old.find( element.old_id );
        while( it != new_to_old.end() )
        {
            if( it->second == element.old_id )
            {
                break;
            }
            new_to_old.at( element.new_id ) = it->second;
            to_remove.insert( it->second );
            it = new_to_old.find( it->second );
        }
    }

    template < typename T >
    void process_mapping_element( geode::MultiMapping< T >& element,
        absl::flat_hash_set< T >& to_remove,
        absl::flat_hash_map< T, absl::InlinedVector< T, 1 > >& new_to_olds,
        const T& no_value )
    {
        bool older{ false };
        do
        {
            older = false;
            absl::InlinedVector< T, 1 > updated_olds;
            for( const auto& old_id : element.old_id )
            {
                if( old_id == no_value )
                {
                    continue;
                }
                const auto it = new_to_olds.find( old_id );
                if( it != new_to_olds.end() )
                {
                    if( it->second.size() == 1 && it->second.front() == old_id )
                    {
                        // old == new
                        updated_olds.emplace_back( old_id );
                    }
                    else
                    {
                        older = true;
                        updated_olds.insert( updated_olds.end(),
                            it->second.begin(), it->second.end() );
                        to_remove.insert( old_id );
                    }
                }
                else
                {
                    updated_olds.emplace_back( old_id );
                }
            }
            element.old_id = std::move( updated_olds );
        } while( older );
        if( element.old_id.empty() )
        {
            new_to_olds.at( element.new_id ) = { no_value };
        }
        else
        {
            geode::sort_unique( element.old_id );
            new_to_olds.at( element.new_id ) = std::move( element.old_id );
        }
    }

    template < typename T, typename MappingType, typename NewToOldMap >
    void update_mappings( std::vector< MappingType >& mappings,
        NewToOldMap& new_to_old,
        const T& no_value )
    {
        absl::flat_hash_set< geode::index_t > to_remove;
        for( const auto& element : mappings )
        {
            new_to_old.emplace( element.new_id, element.old_id );
        }
        for( auto& element : mappings )
        {
            process_mapping_element( element, to_remove, new_to_old, no_value );
        }
        for( const auto id : to_remove )
        {
            new_to_old.erase( id );
        }
        mappings.clear();
        for( const auto& mapping : new_to_old )
        {
            mappings.emplace_back( mapping.first, mapping.second );
        }
    }

    template < typename T >
    void update_mappings(
        std::vector< geode::Mapping< T > >& mappings, const T& no_value )
    {
        absl::flat_hash_map< T, T > new_to_old;
        update_mappings( mappings, new_to_old, no_value );
    }

    template < typename T >
    void update_multi_mappings(
        std::vector< geode::MultiMapping< T > >& multi_mappings,
        const T& no_value )
    {
        absl::flat_hash_map< T, absl::InlinedVector< T, 1 > > new_to_olds;
        update_mappings( multi_mappings, new_to_olds, no_value );
    }
} // namespace geode
