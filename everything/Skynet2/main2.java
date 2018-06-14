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
    
    Node( Integer id, Node parent ) {
        _id = id;
        _hotness = new Integer( -1 );
        _visited = false;
        _parent = parent;
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
        return _id.equals( other._id ) && _parent.equals( other._parent );        
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

    private static void createGraphLink( int SI, boolean[][] graph, final Integer[] gateways, HashSet< Node > critical_paths, HashMap< Integer, Node > nodes ) {
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
                    Node adj_node;
                    if( !nodes.containsKey( adj_id ) ) {
                        adj_node = new Node( adj_id, current_node );
                        nodes.put( adj_id, adj_node );
                    }
                    else {
                        adj_node = nodes.get( adj_id );
                    }

                    if( Arrays.stream( gateways ).filter( g -> g.equals( adj_id ) ).findAny().isPresent() ) {
                        ++current_node._hotness;
                        adj_node._hotness = 0;
                        critical_paths.add( adj_node );
                    }
                    else {
                        queue.offerFirst( adj_id );
                        visited_nodes[ adj_id ] = true;
                    }
                }
            }
        }
        
        System.err.println("hotness:");
        nodes.forEach( (i, n) -> System.err.println( n._id + " " + n._hotness ) );
        System.err.println("critical paths:");
        critical_paths.forEach( n -> {
            Node p = n;
            while( p._parent != null ) {                
                System.err.printf( p._id + "(" + p._hotness + "), " );
                p = p._parent;
            }
            System.err.println( p._id );
        } );
    }
    
    public static Node findCriticalPath( HashSet< Node > critical_paths, HashMap< Integer, Node > nodes ) {
        Optional< Node > adj_gtw = critical_paths.stream().filter( p -> p.depth().intValue() == 1 ).findAny();
        if( adj_gtw.isPresent() ) {
            return adj_gtw.get();
        }
        
        Node critical_path = critical_paths.iterator().next();
        Integer max_hotness = new Integer( -999999 );
        for( Node path : critical_paths ) {
            Integer path_hotness = new Integer( 0 );
            Node p = path;
            while( p._parent != null ) {
                path_hotness += p._hotness;
                p = p._parent;
            }
            path_hotness += p._hotness;
            
            System.err.println("path: " + path._id + " " + path._parent._id + " depth: " + path.depth() + " hotness: " + path_hotness );
            if( path_hotness.intValue() >= ( path.depth().intValue() - 1 ) ) {
                System.err.println("force !");
                critical_path = path;
                break;
            }
            
            if( path_hotness.intValue() > max_hotness.intValue() ) {
                critical_path = path;
                max_hotness = path_hotness;
            }
        }
        return critical_path;
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
            
            HashSet< Node > critical_paths = new HashSet< Node >();
            HashMap< Integer, Node > nodes = new HashMap< Integer, Node >();
            createGraphLink( SI, graph, gateways, critical_paths, nodes );
            
            Node critical_path = findCriticalPath( critical_paths, nodes );
            
            graph[ critical_path._parent._id ][ critical_path._id ] = false;
            graph[ critical_path._id ][ critical_path._parent._id ] = false;
            
            System.out.println( critical_path._parent._id + " " + critical_path._id );
        }
    }
}
