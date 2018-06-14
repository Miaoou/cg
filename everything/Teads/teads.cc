#include <algorithm>
#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

using tree_t = unordered_map< int, vector< int > >;

int DFS( tree_t& tree, tree_t::reference root ) {
    unordered_set< int > visited{root.first};
    int depth = 0;
    int max_depth = 0;
    stack< tree_t::value_type > q;
    q.push( root );

    while( !q.empty() ) {
        auto next = q.top();
        //cerr << "next: " << next.first << endl;
        auto child_it = find_if( begin( next.second ), end( next.second ),
                                 [&]( int child ) {
                                     //cerr << "child: " << child << endl;
                                     return visited.find( child ) == end( visited );
                                 } );

        if( child_it != end( next.second ) ) {
            q.push( *tree.find( *child_it ) );
            visited.insert( *child_it );
            //cerr << "paf++" << endl;
            ++depth;
            if( depth > max_depth )
                max_depth = depth;
        }
        else {
            q.pop();
            //cerr << "paf--" << endl;
            --depth;
        }
    }
    return max_depth;
}

int main() {
    tree_t tree;
    int n; // the number of adjacency relations
    cin >> n;
    cin.ignore();
    for( int i = 0; i < n; i++ ) {
        int xi; // the ID of a person which is adjacent to yi
        int yi; // the ID of a person which is adjacent to xi
        cin >> xi >> yi;
        cin.ignore();

        tree[ xi ].push_back( yi );
        tree[ yi ].push_back( xi );
    }

    int min_depth = 9999999;
    for( auto& root : tree ) {
        //if( root.first == 2 ) {
        cerr << "root: " << root.first << endl;
        int depth = DFS( tree, root );
        cerr << "depth: " << depth << endl;
        if( depth < min_depth )
            min_depth = depth;
        //}
    }

    // The minimal amount of steps required to completely propagate the advertisement
    cout << min_depth << endl;
}
