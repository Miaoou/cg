#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

class Node;
class Coord;
pair< Node, bool >
getNode( Coord const& coord );
bool
isCoordWater( Coord const& coord );

vector< string > rows
{
    "####",
    "##O#",
    "#OO#",
    "####"
};

vector< vector< Node > > nodes;

class Coord
{
public:
    Coord( int X, int Y ) : _X{ X }, _Y{ Y } {}
    Coord( Coord const& ) = default;
    int _X;
    int _Y;
};

class Node
{
public:
    Node( Coord const& coord ) : _coord( coord ), _water( isCoordWater( coord ) ) {}

    int findLakeFromHere( Coord const& coord )
    {
        auto pair = getNode( coord );
        if( pair.second && !pair.first._water )
            return 0;

        int lakeSurface = 0;
        _analyzed = true;

        pair = getNode( Coord( coord._X + 1, coord._Y ) );
        if( pair.second && pair.first._water && !pair.first._analyzed )
            lakeSurface += findLakeFromHere( pair.first._coord );

        pair = getNode( Coord( coord._X - 1, coord._Y ) );
        if( pair.second && pair.first._water && !pair.first._analyzed )
            lakeSurface += findLakeFromHere( pair.first._coord );

        pair = getNode( Coord( coord._X, coord._Y + 1) );
        if( pair.second && pair.first._water && !pair.first._analyzed )
            lakeSurface += findLakeFromHere( pair.first._coord );

        pair = getNode( Coord( coord._X, coord._Y - 1 ) );
        if( pair.second && pair.first._water && !pair.first._analyzed )
            lakeSurface += findLakeFromHere( pair.first._coord );

        return lakeSurface == 0 ? 1 : lakeSurface;
    }

    Coord _coord;
    bool _water;
    bool _analyzed = false;
};

bool
isCoordWater( Coord const& coord )
{
    return rows[ coord._Y ][ coord._X ] == 'O';
}

pair< Node, bool >
getNode( Coord const& coord )
{
    if( coord._Y < nodes.size() && coord._X < nodes[ coord._Y ].size() )
        return make_pair( nodes[ coord._Y ][ coord._X ], true );
    else
        return make_pair( nodes[ 0 ][ 0 ], false );
}

void
initNodes( int L, int H )
{
    for( auto y = 0; y < H; ++y )
    {
        nodes.push_back( vector< Node >{} );
        for( auto x = 0; x < L; ++x )
            nodes[ y ].push_back( Node( Coord( x, y ) ) );
    }
}

int
main()
{
    int L = 4;
    int H = 4;

    int N = 3;

    auto coords = vector< Coord >{ { 0, 0 }, { 1, 2 }, { 2, 1 } };
    initNodes( L, H );

    for( auto const& coord : coords )
        cout << getNode( coord ).first.findLakeFromHere( coord ) << endl;

    return 0;
}