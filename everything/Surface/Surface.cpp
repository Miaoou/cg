#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <array>
#include <cstdint>
#include <fstream>
#include <stack>
#include <deque>

using namespace std;

class Node;
class Coord;
Node& getNode( Coord const& coord );
bool isCoordWater( Coord const& coord );
bool checkBounds( Coord const& coord );

vector< string > rows;
vector< vector< Node > > nodes;
vector< uint32_t > surfaceCache;

class Coord
{
public:
    Coord( uint32_t X, uint32_t Y ) : _X{ X }, _Y{ Y } {}
    Coord& operator= ( Coord&& right ) = delete;
    Coord( Coord&& right ) : _X{ right._X }, _Y{ right._Y } {}
    Coord( Coord const& right ) = delete;
    Coord& operator= ( Coord const& right ) = delete;

    uint32_t const _X;
    uint32_t const _Y;
};

class Node
{
public:
    Node( uint32_t X, uint32_t Y ) : _coord{ X, Y }, _water{ isCoordWater( _coord ) } {}
    Node& operator= ( Node&& right ) = delete;
    Node( Node&& right ) : _coord( right._coord._X, right._coord._Y ), _water( right._water ), _analyzed( right._analyzed ), _awaiting( right._awaiting ), _cacheIndex( right._cacheIndex ) {}
    Node() = delete;
    Node& operator= ( Node const& right ) = delete;

    Coord const _coord;
    bool _water = false;
    bool _analyzed = false;
    bool _awaiting = false;
    int32_t _cacheIndex = -1;
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
    nodes.reserve( H );
    for( auto y = 0; y < H; ++y )
    {
        nodes.emplace_back();
        nodes.back().reserve( L );
        for( auto x = 0; x < L; ++x )
            nodes[ y ].emplace_back( x, y );
    }
}

bool
checkBounds( Coord const& coord )
{
    return coord._Y < nodes.size() && coord._X < nodes[ coord._Y ].size();
}

void
findLakeAroundNode( Node& node, int32_t cacheIndex, vector< Node* >& toBeAnalyzed )
{
    node._analyzed = true;
    node._cacheIndex = cacheIndex;
    ++surfaceCache[ cacheIndex ];

    auto findLakeAtNode = [ &toBeAnalyzed ] ( Coord&& coord )
    {
        if( checkBounds( coord ) )
        {
            auto& node = getNode( coord );
            if( node._water && !node._analyzed && !node._awaiting )
            {
                node._awaiting = true;
                toBeAnalyzed.push_back( &node );
            }
        }
    };
    
    findLakeAtNode( Coord( node._coord._X + 1, node._coord._Y ) );
    findLakeAtNode( Coord( node._coord._X - 1, node._coord._Y ) );
    findLakeAtNode( Coord( node._coord._X, node._coord._Y + 1 ) );
    findLakeAtNode( Coord( node._coord._X, node._coord._Y - 1 ) );
}

uint32_t
findLake( Coord const& coord )
{
    auto& root = getNode( coord );
    if( !root._water )
        return 0;

    if( root._analyzed )
        return surfaceCache[ root._cacheIndex ];

    surfaceCache.push_back( 0 );

    auto toBeAnalyzed = vector< Node* >{ &root };
    toBeAnalyzed.reserve( 2000 );

    auto cacheIndex = surfaceCache.size() - 1;
    auto head = begin( toBeAnalyzed );

    for( int index = 0; ; ++index )
    {
        head = begin( toBeAnalyzed ) + index;
        if( head == end( toBeAnalyzed ) )
            break;

        findLakeAroundNode( **head, cacheIndex, toBeAnalyzed );
    }

    return surfaceCache[ root._cacheIndex ];
}

int
main()
{
    ifstream cin( "D:\\kobra-local\\github\\cg\\everything\\Surface\\Test_9_input.txt" );
    int L;
    cin >> L; cin.ignore();
    int H;
    cin >> H; cin.ignore();

    rows.reserve( H );
    string row;
    row.reserve( L );

    for( int i = 0; i < H; i++ ) {
        cin >> row; cin.ignore();
        rows.push_back( row );
    }

    int N;
    cin >> N; cin.ignore();

    vector< Coord > coords;
    coords.reserve( N );
    for( int i = 0; i < N; i++ ) {
        int X;
        int Y;
        cin >> X >> Y; cin.ignore();
        coords.emplace_back( X, Y );
    }

    initNodes( L, H );

    for( auto const& coord : coords )
        cout << findLake( coord ) << "\n";

    cout << endl;

    return 0;
}