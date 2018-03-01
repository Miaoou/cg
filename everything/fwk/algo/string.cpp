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

namespace cg::fwk::str {

// longest common substring
auto LCSub( std::string const& s1, std::string const& s2 ) {
    std::vector< std::vector< uint32_t > > L;
    L.resize( s2.size() + 1, std::vector< uint32_t >( s1.size() + 1 ) );
    size_t z = 0;
    std::vector< std::string > longest_subs;

    for( auto i = 1; i < s1.size() + 1; ++i ) {
        for( auto j = 1; j < s2.size() + 1; ++j ) {
            if( s1[ i - 1 ] == s2[ j - 1 ] ) {
                if( i == 1 || j == 1 )
                    L[ j ][ i ] = 1;
                else
                    L[ j ][ i ] = L[ j - 1 ][ i - 1 ] + 1;

                if( L[ j ][ i ] > z ) {
                    z = L[ j ][ i ];
                    longest_subs.clear();
                    longest_subs.emplace_back( std::begin( s1 ) +    i - z,
                                                                          std::begin( s1 ) + i
                                                             );
                }
                else if( L[ j ][ i ] == z ) {
                    longest_subs.emplace_back(
                        std::begin( s1 ) + i - z, std::begin( s1 ) + i );
                }
            }
            else {
                L[ j ][ i ] = 0;
            }
        }
    }
    return longest_subs;
}
} // namespace cg::fwk::str

int main() {
    auto subs = cg::fwk::str::LCSub("maowplouf", "plouflechat");
    for( auto const& sub : subs )
        std::cout << sub.c_str() << "\n";
}

