// SuffixTrie.cc

#include "../graph/graph.h"

#include <unordered_map>
#include <unordered_set>

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
        std::cout << "Adding new string: " << s.c_str() << "\n";

        _strings_node_ends.push_back( std::vector< cg::fwk::graph::NodeView >() );
        
        for( auto it = std::end( s ) - 1; it != std::begin( s ); --it ) {
            add_suffix( std::string( it, std::end( s ) ) );
        }
        add_suffix( s );
    }

    void print() {
        auto edge_printer = [this](cg::fwk::graph::EdgeView const& e) {
            std::cout << "  e" << e._id << " (" << _edge_values.at(e._id) << ") -> n" << _graph.get_edge(e)._end._id;
            std::cout << " owned by: ";

            for (auto const& owners : _edge_owners.at(e._id)) {
                std::cout << "s" << owners << ", ";
            }
            std::cout << "\n";
        };

        auto node_printer = [this](cg::fwk::graph::NodeView const& n) {
            std::cout << "n" << n._id << ": \n";
        };

        print_oriented(_graph, _root, edge_printer, node_printer);

        uint32_t idx = 1;
        for (auto const& node_ends : _strings_node_ends) {
            std::cout << "s" << idx++ << ": ";
            for (auto const& nodes : node_ends) {
                std::cout << "n" << nodes._id << ", ";
            }
            std::cout << "\n";
        }
    }

private:
    void add_string_node_end( cg::fwk::graph::NodeView const& n ) {
        _strings_node_ends.back().push_back( n );
    }

    /*
    void remove_string_node_end( cg::fwk::graph::NodeView const& n ) {
        auto& curr_string_nodes = _strings_node_ends.back();
        curr_string_nodes.erase(
                std::remove(
                    std::begin( curr_string_nodes ),
                    std::end( curr_string_nodes ),
                    n ),
                std::end( curr_string_nodes ) );
    }
    */
    void add_suffix( std::string const& s ) {
        std::cout << "\tAdding suffix " << s.c_str() << "...\n";

        auto curr_node = _root;
        auto last_edge = cg::fwk::graph::EdgeView( 0 );
        auto const curr_string_idx = _strings_node_ends.size();

        for( auto c : s ) {
            auto const& curr_edges = _graph.get_node( curr_node ).get_edges();
            auto it = std::find_if(
                    std::cbegin( curr_edges ),
                    std::cend( curr_edges ),
                    [ this, c, &last_edge ]( cg::fwk::graph::EdgeView const& e ) {
                return ( e != last_edge ) ? _edge_values[ e._id ] == c : false;
            } );

            if( it == std::cend( curr_edges ) ) {
                auto end = _graph.new_node();
                auto e = _graph.new_edge( curr_node, end );
                _edge_values[ e._id ] = c;
                curr_node = end;
                last_edge = e;
            }
            else {
                auto const found_edge = _graph.get_edge( *it );
                curr_node = found_edge._end;
                last_edge = cg::fwk::graph::EdgeView( found_edge._id );
            }

            _edge_owners[ last_edge._id ].insert( curr_string_idx );
        }

        auto end_suffix = _graph.new_node();
        auto e = _graph.new_edge( curr_node, end_suffix );
        _edge_values[ e._id ] = '$';
        add_string_node_end( end_suffix );

        _edge_owners[ e._id ].insert( curr_string_idx );
    }

    cg::fwk::graph::Graph _graph;
    cg::fwk::graph::NodeView _root;
    std::unordered_map< std::size_t, char > _edge_values;
    // < edge, < strings > >
    std::unordered_map< std::size_t, std::unordered_set< std::size_t > > _edge_owners;
    std::vector< std::vector< cg::fwk::graph::NodeView > > _strings_node_ends;
};

int
main() {
    std::string const s1{ "AGATTA" };
    std::string const s2{ "GATTACA" };
    std::string const s3{ "TACAGA" };

    Trie trie;
    trie.init();
    trie.add_string( s1 );
    trie.add_string( s2 );
    trie.add_string( s3 );

    trie.print();
    system("pause");
    return 0;
}
