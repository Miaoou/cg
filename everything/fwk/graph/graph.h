// graph.h

#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdlib>

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

    void print_edge( EdgeView const& e ) const {
        std::cout << _edges[ e._id ];
    }

    void print_neighbours( NodeView const& n ) const {
        std::cout << "neighbours of " << n._id << "\n";
        for( auto const& e : _nodes[ n._id ].get_edges() ) {
            print_edge( e );
        }
    }

    Node const& get_node( NodeView const& n ) const {
        return _nodes[ n._id ];
    }

    Edge const& get_edge( EdgeView const& e ) const {
        return _edges[ e._id ];
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
