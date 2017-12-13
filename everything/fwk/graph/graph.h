// graph.h

// Copyright 2016-2017 Brice Gagnage
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
// http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <queue>
#include <optional>

namespace cg {
namespace fwk {
namespace graph {

class NodeView {
public:
    NodeView() = default;
    NodeView& operator=( NodeView const& ) = default;
    NodeView( std::size_t const id ) : _id{ id } {
    }

    std::size_t _id = 0;
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
    EdgeView& operator=( EdgeView const& e ) = default;

    EdgeView( std::size_t const id ) : _id{ id } {
    }

    std::size_t _id = 0;
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

    Node( std::size_t const id ) : _id{ id } {
    }

    std::size_t const _id;

    void add_edge( std::size_t id ) {
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
    friend std::ostream& operator<<( std::ostream& os, Edge const& e );

    Edge( std::size_t const id, NodeView start, NodeView end )
        : _id{ id }
        , _start{ start }
        , _end{ end } {
    }

    std::size_t const _id;
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

        return id;
    }

    EdgeView new_edge( NodeView n1, NodeView n2 ) {
        auto const id = _edges.size();
        _edges.emplace_back( id, n1, n2 );

        _nodes[n1._id].add_edge( id );
        _nodes[n2._id].add_edge( id );
        return EdgeView( id );
    }

    Node const& get_node( NodeView const& n ) const {
        return _nodes[n._id];
    }

    Edge const& get_edge( EdgeView const& e ) const {
        return _edges[e._id];
    }

private:
    std::vector< Node > _nodes;
    std::vector< Edge > _edges;
};

std::vector< EdgeView >
get_oriented_edges( Graph const& g, NodeView const& nv ) {
    std::vector< EdgeView > oe;
    auto const edges = g.get_node( nv ).get_edges();
    std::copy_if(
        std::cbegin( edges ),
        std::cend( edges ),
        std::back_inserter( oe ),
        [&g, &nv] ( EdgeView const& e ) {
        return g.get_edge( e )._start == nv;
    }
    );
    return oe;
}

template< typename EdgeHandler_t, typename NodeHandler_t >
std::optional< NodeView >
BFS( Graph const& g, NodeView const& root, EdgeHandler_t&& edge_handler, NodeHandler_t&& node_handler ) {
    std::optional< NodeView > res;
    bool found = false;
    std::queue< NodeView > q;
    q.push( root );

    while( !q.empty() && !found ) {
        auto const n = q.front();
        q.pop();

        auto const edges = get_oriented_edges( g, n );
        for( auto const& ev : edges ) {
            auto const& e = g.get_edge( ev );
            edge_handler( e );
            q.push( e._end );
        }

        found = node_handler( g.get_node( n ) );
        if( found )
            res = n;
    } 

    return res;
}

template< typename EdgePrinter_t >
void
print_edges( Graph const& g, NodeView const& n, bool oriented, EdgePrinter_t&& edge_printer ) {
    auto const edges = oriented ? get_oriented_edges( g, n ) : g.get_node( n ).get_edges();
    for( auto const& e : edges ) {
        edge_printer( g.get_edge( e ) );
    }
}

template< typename EdgePrinter_t, typename NodePrinter_t >
void
print_oriented_node( Graph const& g, NodeView const& n, EdgePrinter_t&& edge_printer, NodePrinter_t&& node_printer ) {
    if( !get_oriented_edges( g, n ).empty() ) {
        node_printer( g.get_node( n ) );
        print_edges( g, n, true, std::forward< EdgePrinter_t >( edge_printer ) );
        std::cout << "\n";
    }
}

template< typename EdgePrinter_t, typename NodePrinter_t >
void
print_oriented( Graph const& g, NodeView const& root, EdgePrinter_t&& edge_printer, NodePrinter_t&& node_printer ) {
    std::queue< NodeView > nodes_to_print;
    nodes_to_print.push( root );

    while( !nodes_to_print.empty() ) {
        auto const node = nodes_to_print.front();
        nodes_to_print.pop();

        auto const edges = get_oriented_edges( g, node );
        for( auto const& e : edges ) {
            nodes_to_print.push( g.get_edge( e )._end );
        }

        print_oriented_node( g, node, std::forward< EdgePrinter_t >( edge_printer ), std::forward< NodePrinter_t >( node_printer ) );
    }
}

}
}
} //namespace cg::fwk::graph


int
main() {
    using namespace ::cg::fwk::graph;

    Graph g;
    auto root = g.new_node();
    auto n1 = g.new_node();
    auto n2 = g.new_node();
    auto n3 = g.new_node();

    g.new_edge( root, n1 );
    g.new_edge( root, n2 );
    g.new_edge( n1, n3 );
    g.new_edge( n2, n3 );

    print_oriented( g, root,
        [] ( Edge const& e ) {
            std::cout << e;
        },
        [] ( Node const& n ) {
            std::cout << n;
        }
    );

    std::cout << "BFS: \n";
    auto n = BFS( g, root, [] ( Edge const& e ) {},
        [] ( Node const& n ) -> bool {
            std::cout << n._id << "...";
            return n._id == 2 ? true : false;
        }
    );

    system( "pause" );
    return 0;
}


#endif // GRAPH_H
