// SuffixTrie.cc

#include <vector>
#include <iostream>
#include <algorithm>

class NodeTrie {
public:
    NodeTrie() = default;
    NodeTrie( char c ) : _c( c ) {
    }
    ~NodeTrie() = default;

    void setParent( NodeTrie* parent ) const {
        _parent = parent;
    }

    void addChild( NodeTrie* child ) {
        _children.push_back( child );
    }

    std::vector< NodeTrie* > const& getChildren() const {
        return _children;
    }
    
    char const _c = '\0';

private:
    NodeTrie* _parent = nullptr;
    std::vector< NodeTrie* > _children;
};

void
addStringToTrie( std::string const& s, std::vector< std::unique_ptr< NodeTrie > >& trie ) {
    using std::foreach; using std::rbegin; using std::rend; using std::string;

    NodeTrie* current = trie.front();

    foreach( rbegin( s ), rend( s ), [ &root ] ( string::value_type c ) {
        if( current->_c != c ) {
            auto child = std::find_if( begin( p->getChildren() ), end( p->getChildren() ), [ =c ] ( char child_c ) {
                return child_c == c;
            } );

            if( child == end( p->getChildren() ) ) {
                trie.emplace_back( c );
                inserted = true;
            }
            else {
                            
            }
        }
    } );
}

int main() {
    std::vector< std::unique_ptr< NodeTrie > > trie{ std::make_unique< NodeTrie >( '\0' ) };
    addStringToTrie( "AGATTA", trie );

    return 0;
}
