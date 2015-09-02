#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <array>
#include <cstdint>
#include <fstream>
#include <stack>
#include <list>

using namespace std;

class Node;
class Coord;
Node& getNode( Coord const& coord );
bool isCoordWater( Coord const& coord );
bool checkBounds( Coord const& coord );

vector< string > rows;
vector< vector< Node > > nodes;

class Coord
{
public:
    Coord( uint8_t X, uint8_t Y ) : _X{ X }, _Y{ Y } {}
    Coord& operator= ( Coord&& right ) = delete;
    Coord( Coord&& right ) : _X{ right._X }, _Y{ right._Y } {}
    Coord( Coord const& right ) = delete;
    Coord& operator= ( Coord const& right ) = delete;

    uint8_t const _X;
    uint8_t const _Y;
};

class Node
{
public:
    Node( uint8_t X, uint8_t Y ) : _coord( X, Y )
    {
        _water = isCoordWater( _coord );
    }
    Node& operator= ( Node&& right ) = delete;
    Node( Node&& right ) : _coord( right._coord._X, right._coord._Y ), _water( right._water ), _analyzed( right._analyzed ) {}
    Node() = delete;
    Node& operator= ( Node const& right ) = delete;

    Coord const _coord;
    bool _water = false;
    bool _analyzed = false;
    bool _awaiting = false;
};

bool
isCoordWater( Coord const& coord )
{
    return rows[ coord._Y ][ coord._X ] == 'O';
}

Node&
getNode( Coord const& coord )
{
    return nodes[ coord._Y ][ coord._X ];
}

void
initNodes( int L, int H )
{
    for( auto y = 0; y < H; ++y )
    {
        nodes.emplace_back();
        for( auto x = 0; x < L; ++x )
            nodes[ y ].emplace_back( x, y );
    }
}

void
resetNodes( int L, int H )
{
    for( auto y = 0; y < H; ++y )
    {
        for( auto x = 0; x < L; ++x )
        {
            nodes[ y ][ x ]._analyzed = false;
            nodes[ y ][ x ]._awaiting = false;
        }
    }
}

bool
checkBounds( Coord const& coord )
{
    return coord._Y < nodes.size() && coord._X < nodes[ coord._Y ].size();
}

list< Node* >
findLakeFromHere( Node& node, uint64_t& surface )
{
    node._analyzed = true;
    auto neighbours = list< Node* >{};

    if( !node._water )
        return neighbours;

    ++surface;

    auto findLakeNeighbour = [ &neighbours ] ( Coord&& coord )
    {
        if( checkBounds( coord ) )
        {
            auto& node = getNode( coord );
            if( node._water && !node._analyzed && !node._awaiting )
            {
                node._awaiting = true;
                neighbours.push_back( &node );
            }
        }
    };
    
    findLakeNeighbour( Coord( node._coord._X + 1, node._coord._Y ) );
    findLakeNeighbour( Coord( node._coord._X - 1, node._coord._Y ) );
    findLakeNeighbour( Coord( node._coord._X, node._coord._Y + 1 ) );
    findLakeNeighbour( Coord( node._coord._X, node._coord._Y - 1 ) );
    
    return neighbours;
}

uint64_t
findLake( Coord const& coord )
{
    auto& root = getNode( coord );
    list< Node* > toBeAnalyzed{ &root };
    auto surface = uint64_t{ 0 };

    while( !toBeAnalyzed.empty() )
    {
        auto node = toBeAnalyzed.front();
        toBeAnalyzed.pop_front();

        auto neighbours = findLakeFromHere( *node, surface );

        for( auto neighbour : neighbours )
            toBeAnalyzed.push_back( neighbour );
    }

    return surface;
}

int
main()
{
    int L = 900;
    int H = 400;

    ifstream ifs( "Test_9_input.txt" );
    string row;
    while(getline(ifs, row))
        rows.push_back( row );

    int N = 1;

    vector< Coord > coords;
    for( int i = 0; i < N; i++ ) {
        uint8_t X = 43;
        uint8_t Y = 54;
        
        coords.emplace_back( X, Y );
    }

    initNodes( L, H );

    for( auto const& coord : coords )
    {
        resetNodes( L, H );
        cout << findLake( coord ) << endl;
    }

    return 0;
}