// string.cpp
#include <algorithm>
#include <iostream>
#include <iterator>
#include <queue>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

// return all common substrings of s1 and s2 and the matrix associated
auto find_all_common_subs( std::string const& s1, std::string const& s2 ) {
    std::vector< std::vector< uint32_t > > L;
    L.resize( s2.size() + 1, std::vector< uint32_t >( s1.size() + 1 ) );
    std::vector< std::tuple< std::string, size_t, size_t > > subs;

    for( auto i = 1; i < s1.size() + 1; ++i ) {
        for( auto j = 1; j < s2.size() + 1; ++j ) {
            if( s1[ i - 1 ] == s2[ j - 1 ] ) {
                if( i == 1 || j == 1 )
                    L[ j ][ i ] = 1;
                else
                    L[ j ][ i ] = L[ j - 1 ][ i - 1 ] + 1;

                subs.push_back( std::make_tuple(
                    std::string( std::begin( s1 ) + i - L[ j ][ i ], std::begin( s1 ) + i ), i, j ) );
            }
            else {
                L[ j ][ i ] = 0;
            }
        }
    }
    return std::make_pair( subs, L );
}

auto merge_words( std::string const& s1, std::string const& s2 ) {
    auto pair = find_all_common_subs( s1, s2 );
    auto& subs = pair.first;
    auto const& matrix = pair.second;
    std::sort( std::begin( subs ), std::end( subs ),
               std::greater< decltype( subs.front() ) >() );

    std::string merged_s;
    for( auto const& tuple : subs ) {
        size_t i, j;
        std::string sub;
        std::tie( sub, i, j ) = tuple;

        if( i == s1.size() ) {
            // suffix of s1
            if( j < s2.size() && ( j - matrix[ j ][ i ] == 0 ) )
                merged_s = std::string( s1 ) +
                           std::string( std::begin( s2 ) + sub.size(), std::end( s2 ) );
            // s1 fully contained in s2
            else if( matrix[ j ][ i ] == s1.size() )
                merged_s = s2;
        }
        else if( j == s2.size() ) {
            // prefix of s1
            if( i < s1.size() && ( i - matrix[ j ][ i ] == 0 ) )
                merged_s = std::string( s2 ) +
                           std::string( std::begin( s1 ) + sub.size(), std::end( s1 ) );
            // s2 fully contained in s1
            else if( matrix[ j ][ i ] == s2.size() )
                merged_s = s1;
        }

        if( !merged_s.empty() )
            break;
    }

    if( merged_s.empty() )
        merged_s = s1 + s2;

    return merged_s;
}

// generate all permutations of  { x, y, z...}
std::vector< std::vector< size_t > >
generate_tree_indexes( std::vector< size_t >& indexes, size_t i_start = 0 ) {
    std::vector< std::vector< size_t > > sub_indexes;

    if( i_start == indexes.size() )
        sub_indexes.push_back( indexes );

    for( auto i = i_start; i < indexes.size(); ++i ) {
        std::swap( indexes[ i_start ], indexes[ i ] );
        auto ret = generate_tree_indexes( indexes, i_start + 1 );
        std::copy( std::begin( ret ), std::end( ret ), std::back_inserter( sub_indexes ) );
        std::swap( indexes[ i ], indexes[ i_start ] );
    }

    return sub_indexes;
}

int main() {
    std::vector< std::string > words{"AGATTA", "GATTACA", "CATAGA"};

    std::vector< size_t > root( words.size() );
    size_t n = 0;
    std::generate( std::begin( root ), std::end( root ), [&n]() { return n++; } );
    auto indexes = generate_tree_indexes( root );

    size_t min_merged_size = std::numeric_limits< size_t >::max();

    for( auto const& permutation : indexes ) {
        auto merged_s = words[ permutation[ 0 ] ];
        for( auto i = 1; i < permutation.size(); ++i )
            merged_s = merge_words( merged_s, words[ permutation[ i ] ] );

        min_merged_size = ( merged_s.size() < min_merged_size ) ? merged_s.size() : min_merged_size;
    }

    std::cout << min_merged_size << std::endl;
    return 0;
}
