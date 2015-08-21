#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

vector< string > rows
{
    "####",
    "##O#",
    "#OO#",
    "####"
};

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

    unique_ptr< Node > _left;
    unique_ptr< Node > _right;
    unique_ptr< Node > _top;
    unique_ptr< Node > _bot;
    Coord _coord;
    bool _analyzed = false;
    bool _water;
};

bool
isCoordWater( Coord const& coord )
{
    return rows[ coord._Y ][ coord._X ] == 'O';
}

vector< Node >
findAdjacent( Node const& node )
{
    auto adjacents = vector< Node >{};
    return adjacents;
}

bool
analyze( Node const& node )
{
    bool endNode = false;
    if( node._left->_water )

}

int
findLakeSurface( Coord const& coord )
{
    auto node = Node( coord );
    if( !node._water )
        return 0;

    auto adjacents = findAdjacent( node );

    bool analyze = true;
    while( analyze )
    {

    }

    return 0;
}

int main()
{
    int L = 4;
    int H = 4;

    int N = 3;

    auto coords = vector< Coord >{ { 0, 0 }, { 1, 2 }, { 2, 1 } };

    for( auto const& coord : coords )
        cout << findLakeSurface( coord ) << endl;

    return 0;
}