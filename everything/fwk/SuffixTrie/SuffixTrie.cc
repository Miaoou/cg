// SuffixTrie.cc

#include "../graph/graph.h"

#include <unordered_map>

class Trie {
public:
    Trie() = default;
    ~Trie() = default;

    void init() {
        auto root = _graph.new_node();
    }

    void add_string( std::string const& s ) {
        for( auto it = std::end( s ) - 1; it != std::begin( s ); --it ) {
            add_suffix( std::string( it, std::end( s ) ) );
        }
        add_suffix( s );
    }
    
private:
    void add_suffix( std::string const& s ) {
        std::cout << s << "\n";
    }

    cg::fwk::graph::Graph _graph;
    std::unordered_map< uint32_t, char > _trie_values;
};

int
main() {
    std::string const s1{ "AGATTA" };
    std::string const s2{ "GATTACA" };
    std::string const s3{ "TACAGA" };

    Trie trie;
    trie.add_string( s1 );
    trie.add_string( s2 );
    trie.add_string( s3 );

    
    return 0;
}
