// graph.h

#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <queue>

namespace cg { namespace fwk { namespace graph {

class NodeView {
public:
    NodeView() = default;
    NodeView( uint32_t const id ) : _id{ id } {
    }

    uint32_t _id = 0;
};

bool
operator==( NodeView const& n1, NodeView const& n2 ) {
    return n1._id == n2._id;
}

bool
operator!=( NodeView const& n1, NodeView const& n2 ) {
    return !( n1 == n2 );
}

class EdgeView {
public:
    EdgeView() = default;
    EdgeView( EdgeView const& e ) : _id{ e._id } {}
    EdgeView& operator=( EdgeView const& e ) {
        _id = e._id;
        return *this;
    }

    EdgeView( uint32_t const id ) : _id{ id } {
    }

    uint32_t _id = 0;
};

bool
operator==( EdgeView const& e1, EdgeView const& e2 ) {
    return e1._id == e2._id;
}

bool
operator!=( EdgeView const& e1, EdgeView const& e2 ) {
    return !( e1 == e2 );
}

class Node {
public:
    friend std::ostream& operator<<( std::ostream& os, Node const& e );
    
    Node( uint32_t const id ) : _id{ id } {
    }
    
    uint32_t const _id;

    void add_edge( uint32_t id ) {
        _edges.emplace_back( id );
    }

    std::vector< EdgeView > const& get_edges() const {
        return _edges;
    }

private:
    std::vector< EdgeView > _edges;
};

std::ostream&
operator<<( std::ostream& os, Node const& n ) {
    os << "node : " << n._id << " (";
    for( auto const& e : n._edges ) {
        os << "e" << e._id << ", ";
    }
    os << ")" << "\n";

    return os;
}

class Edge {
public:
    friend std::ostream& operator<<( std::ostream& os, Edge const& e  );

    Edge( uint32_t const id, NodeView start, NodeView end )
        : _id{ id }
        , _start{ start }
        , _end{ end } {
    }
    
    uint32_t const _id;
    NodeView const _start;
    NodeView const _end;
};

std::ostream&
operator<<( std::ostream& os, Edge const& e ) {
    os << "edge : " << e._id << " (start: n" << e._start._id << ", end: n" << e._end._id << ")\n";
    return os;
}

class Graph {
public:
    Graph() = default;
    ~Graph() = default;

    NodeView new_node() {
        auto const id = _nodes.size();
        _nodes.emplace_back( id );

        return NodeView( id );
    }

    EdgeView new_edge( NodeView n1, NodeView n2 ) {
        auto const id = _edges.size();
        _edges.emplace_back( id, n1, n2 );

        _nodes[ n1._id ].add_edge( id );
        _nodes[ n2._id ].add_edge( id );
        return EdgeView( id );
    }

    void print_node( NodeView const& n ) const {
        std::cout << _nodes[ n._id ];
    }
    
    template< typename EdgePrinter_t, typename NodePrinter_t >
    void print_oriented_node( NodeView const& n, EdgePrinter_t&& edge_printer, NodePrinter_t&& node_printer ) const {
        if( !get_oriented_edges( n ).empty() ) {
            node_printer( n );
            print_edges( n, true, std::forward< EdgePrinter_t >( edge_printer ) );
            std::cout << "\n";
        }
    }

    template< typename EdgePrinter_t >
    void print_edge( EdgeView const& e, EdgePrinter_t&& edge_printer ) const {
        edge_printer( e );
    }

    template< typename EdgePrinter_t >
    void print_edges( NodeView const& n, bool oriented, EdgePrinter_t&& edge_printer ) const {
        auto const edges = oriented ? get_oriented_edges( n ) : get_node( n ).get_edges();
        for( auto const& e : edges ) {
            print_edge( e, std::forward< EdgePrinter_t >( edge_printer ) );
        }
    }

    Node const& get_node( NodeView const& n ) const {
        return _nodes[ n._id ];
    }

    Edge const& get_edge( EdgeView const& e ) const {
        return _edges[ e._id ];
    }

    std::vector< EdgeView > get_oriented_edges( NodeView const& nv ) const {
        std::vector< EdgeView > oe;
        auto const edges = get_node( nv ).get_edges();
        std::copy_if(
                std::cbegin( edges ),
                std::cend( edges ),
                std::back_inserter( oe ),
                [ this, &nv ]( EdgeView const& e ) {
                    return get_edge( e )._start == nv;
                }
        );
        return oe;
    }

    template< typename EdgePrinter_t, typename NodePrinter_t >
    void print_oriented( NodeView const& root, EdgePrinter_t&& edge_printer, NodePrinter_t&& node_printer ) const {
        std::queue< NodeView > nodes_to_print;
        nodes_to_print.push( root );

        while( !nodes_to_print.empty() ) {
            auto const node = nodes_to_print.front();
            nodes_to_print.pop();

            auto const edges = get_oriented_edges( node );
            for( auto const& e : edges ) {
                nodes_to_print.push( get_edge( e )._end );
            }

            print_oriented_node( node, std::forward< EdgePrinter_t >( edge_printer ), std::forward< NodePrinter_t >( node_printer ) );
        }
    }

private:
    std::vector< Node > _nodes;
    std::vector< Edge > _edges;
};

}}} //namespace cg::fwk::graph

/*
int
main() {
    Graph g;
    auto n0 = g.new_node();
    auto n1 = g.new_node();
    auto n2 = g.new_node();
    auto n3 = g.new_node();

    g.new_edge( n0, n1 );
    g.new_edge( n0, n2 );
    g.new_edge( n1, n3 );
    g.new_edge( n2, n3 );

    g.print_neighbours( n0 ); std::cout << "\n";
    g.print_neighbours( n1 ); std::cout << "\n";
    g.print_neighbours( n2 ); std::cout << "\n";
    g.print_neighbours( n3 ); std::cout << "\n";

    return 0;
}
*/

#endif // GRAPH_H
