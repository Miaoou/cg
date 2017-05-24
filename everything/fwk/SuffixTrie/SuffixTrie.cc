// SuffixTrie.cc

#include "../graph/graph.h"

#include <unordered_map>

class Trie {
public:
    Trie() = default;
    ~Trie() = default;

    void init() {
        _root = _graph.new_node();
        auto end1 = _graph.new_node();
        auto e = _graph.new_edge( _root, end1 );
        _edge_values[ e._id ] = '$';
    }

    void add_string( std::string const& s ) {
        std::cout << "Adding new string: " << s << "\n";

        for( auto it = std::end( s ) - 1; it != std::begin( s ); --it ) {
            add_suffix( std::string( it, std::end( s ) ) );
        }
        add_suffix( s );
    }
   
    void print() {
        _graph.print_neighbours( _root );
        for( auto const& pair : _edge_values ) {
            std::cout << "e" << pair.first << " : " << pair.second << "\n";
        }
    }

private:
    void add_suffix( std::string const& s ) {
        std::cout << "\tAdding suffix " << s << "...\n";

        auto curr_node = _root;
        for( auto c : s ) {
            auto const& curr_edges = _graph.get_node( curr_node ).get_edges();
            auto it = std::find_if( std::cbegin( curr_edges ), std::cend( curr_edges ), [ this, c ]( cg::fwk::graph::EdgeView const& e ) {
                return _edge_values[ e._id ] == c;
            } );

            if( it == std::cend( curr_edges ) ) {
                auto end = _graph.new_node();
                auto e = _graph.new_edge( curr_node, end );
                _edge_values[ e._id ] = c;
                curr_node = end;
            }
            else {
                curr_node = _graph.get_edge( *it )._end;
            }
        }
    }

    cg::fwk::graph::Graph _graph;
    cg::fwk::graph::NodeView _root;
    std::unordered_map< uint32_t, char > _edge_values;
    std::unordered_map< uint32_t, char > _node_values;
};

int
main() {
    std::string const s1{ "AGATTA" };
    std::string const s2{ "GATTACA" };
    std::string const s3{ "TACAGA" };

    Trie trie;
    trie.init();
    trie.add_string( s1 );
    //trie.add_string( s2 );
    //trie.add_string( s3 );

    trie.print(); 
    return 0;
}
