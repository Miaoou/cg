import java.util.*;
import java.io.*;
import java.math.*;
import java.util.stream.Stream;
import java.util.function.Supplier;

class Node {
    public Integer _id;
    public Integer _hotness;
    public boolean _visited;
    public Node _parent;
    public ArrayList< Integer > _adj_gateways;
    
    Node( Integer id, Node parent ) {
        _id = id;
        _hotness = new Integer( -1 );
        _visited = false;
        _parent = parent;
        _adj_gateways = new ArrayList< Integer >();
    }
    
    @Override
    public boolean equals( Object obj ) {
        if (obj == null) {
            return false;
        }
        if (!Node.class.isAssignableFrom(obj.getClass())) {
            return false;
        }
        final Node other = (Node) obj;
        return _id.equals( other._id );        
    }
    
    public Integer depth() {
        Integer depth = new Integer( 0 );
        Node p = this;
        while( p._parent != null ) {
            ++depth;
            p = p._parent;
        }
        return depth;
    }
}

class Player {

    private static void createGraphLink( int SI, boolean[][] graph, final Integer[] gateways, ArrayList< Node > critical_paths ) {
        HashMap< Integer, Node > nodes = new HashMap< Integer, Node >();
        boolean[] visited_nodes = new boolean[ graph.length ];
        ArrayDeque< Integer > queue = new ArrayDeque< Integer >();
        Node root = new Node( SI, null );
        root._hotness = 0;
        nodes.put( SI, root );
        queue.add( SI );
        visited_nodes[ SI ] = true;
        
        while( !queue.isEmpty() ) {
            Integer current_id = queue.pollLast();
            Node current_node = nodes.get( current_id );            
                
            for( int i = 0; i < graph[ current_id ].length; ++i ) {
                if( graph[ current_id ][ i ] && !visited_nodes[ i ] ) {
                    final Integer adj_id = new Integer( i );
                    if( Arrays.stream( gateways ).filter( g -> g.equals( adj_id ) ).findAny().isPresent() ) {
                        ++current_node._hotness;
                        current_node._adj_gateways.add( adj_id );
                        if( !critical_paths.contains( current_node ) ) {
                            critical_paths.add( current_node );
                        }
                    }
                    else {
                        if( !nodes.containsKey( adj_id ) ) {
                            nodes.put( adj_id, new Node( adj_id, current_node ) );
                        }                    
                    
                        queue.offerFirst( adj_id );
                        visited_nodes[ adj_id ] = true;
                    }
                }
            }
        }
    }
    
    public static Node findCriticalPath( boolean[][] graph, final Integer[] gateways, ArrayList< Node > critical_paths ) {
        Optional< Node > adj_gtw = critical_paths.stream().filter( p -> p.depth().intValue() == 0 ).findAny();
        if( adj_gtw.isPresent() ) {
            for( int i = 0; i < graph[ adj_gtw.get()._id ].length; ++i ) {
                final Integer adj_id = new Integer( i );
                if( graph[ adj_gtw.get()._id ][ adj_id ] && Arrays.stream( gateways ).filter( g -> g.equals( adj_id ) ).findAny().isPresent() ) {
                    return new Node( adj_id, adj_gtw.get() );
                }
            }
        }
        
        Node hottest_node = critical_paths.iterator().next();
        Integer max_hotness = new Integer( -999999 );
        
        for( Node path : critical_paths ) {
            Integer path_hotness = new Integer( 0 );
            Node p = path;
            Node current_hottest_node = path;
            while( p._parent != null ) {
                if( p._hotness > hottest_node._hotness ) {
                    current_hottest_node = p;
                }
                path_hotness += p._hotness;
                p = p._parent;
            }
            path_hotness += p._hotness;
            
            //System.err.println("path: " + path._id + " " + path._parent._id + " depth: " + path.depth() + " hotness: " + path_hotness );    
            if( path_hotness.intValue() > max_hotness.intValue() ) {
                hottest_node = current_hottest_node;
                max_hotness = path_hotness;
            }
        }
        
        //System.err.println("hottest: " + hottest_node._id);
        
        for( int i = 0; i < graph[ hottest_node._id ].length; ++i ) {
            final Integer adj_id = new Integer( i );
            if( graph[ hottest_node._id ][ adj_id ] && Arrays.stream( gateways ).filter( g -> g.equals( adj_id ) ).findAny().isPresent() ) {
                return new Node( adj_id, hottest_node );
            }
        }
        
        return null;
    }
    
    public static void main(String args[]) {
        Scanner in = new Scanner(System.in);
        int N = in.nextInt(); // the total number of nodes in the level, including the gateways
        int L = in.nextInt(); // the number of links
        int E = in.nextInt(); // the number of exit gateways
        
        boolean[][] graph = new boolean[ N ][ N ];
        for (int i = 0; i < L; i++) {
            int N1 = in.nextInt(); // N1 and N2 defines a link between these nodes
            int N2 = in.nextInt();
            graph[ N1 ][ N2 ] = true;
            graph[ N2 ][ N1 ] = true;
        }
        Integer[] gateways = new Integer[ E ];
        for (int i = 0; i < E; i++) {
            int EI = in.nextInt(); // the index of a gateway node
            gateways[ i ] = EI;
        }
        
        while (true) {
            Integer SI = new Integer( in.nextInt() );
            
            ArrayList< Node > critical_paths = new ArrayList< Node >();
            createGraphLink( SI, graph, gateways, critical_paths );

            Node critical_path = findCriticalPath( graph, gateways, critical_paths );
            
            graph[ critical_path._parent._id ][ critical_path._id ] = false;
            graph[ critical_path._id ][ critical_path._parent._id ] = false;
            
            System.out.println( critical_path._parent._id + " " + critical_path._id );
        }
    }
}
