//  Prim’s algorithm

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;
import java.lang.*;

// Global variable for debugging purposes.
class Global
{
    public static int DEBUG = 0;
}

public class Lab01
{
    // Main file for terminal.
    public static void main(String[] args) throws FileNotFoundException
    {
        int i, vertex1, vertex2, numOfEdges, numOfVertices;
        float weight;

        // Read the file.
        File file = new File(args[0]);
        Scanner input = new Scanner(file);
        ArrayList<String> line = new ArrayList<String>();
        MST t = new MST();

        // Scan all lines from the file.
        while (input.hasNextLine())
        {
            line.add(input.nextLine());
        }

        // Get the number of vertices.
        String[] parsed = line.get(0).split(" ");
        numOfVertices = stringToInt(parsed[0]);

        // Get the number of edges.
        String[] parsed2 = line.get(1).split(" ");
        numOfEdges = stringToInt(parsed2[0]);

        // Make the graph variable using the number of vertices (adjacency matrix).
        Graph g = new Graph(numOfVertices);

        if (Global.DEBUG == 1)
        {
            System.out.printf("Maximum number of vertices: %d\n", numOfVertices);
            System.out.printf("Number of edges: %d\n", numOfEdges);
        }

        // Execute all commands from the file.
        for (i = 2; i < line.size(); i++)
        {
            // Get the edge for the graph. (Vertex 1, vertex 2, weight).
            String[] parsed3 = line.get(i).split(" ");

            vertex1 = stringToInt(parsed3[0]);
            vertex2 = stringToInt(parsed3[1]);
            weight = stringToFloat(parsed3[2]);

            // Add the edge to the adjacency matrix.
            g.addEdge(vertex1, vertex2, weight);

            if (Global.DEBUG == 1)
            {
                System.out.printf("Vertex 1: %d\tVertex 2: %d\tFloat: %.2f\n", vertex1, vertex2, weight);
            }
        }

        // Prints out graph.
        if (Global.DEBUG == 1)
        {
            System.out.println(g.printString());
        }

        // Print out the MST using prims algorithm using the graph matrix and vertices.
        t.primMST(g.returnArray(), numOfVertices);
    }

    // Convert a string to an integer.
    public static int stringToInt(String s)
    {
        return Integer.parseInt(s);
    }

    // Convert a string to a float.
    public static float stringToFloat(String s)
    {
        return Float.parseFloat(s);
    }
}

// Adjacency matrix.
class Graph
{
    private float adjMatrix[][];
    private int numVertices;

    // Print the adjacency matrix.
    public boolean printString()
    {
        for (int i = 0; i < numVertices; i++)
        {
            for (int j = 0; j < numVertices; j++)
            {
                System.out.printf("%.5f%c", adjMatrix[i][j], (j == numVertices - 1) ? '\n' : ' ');
            }
        }
        return false;
    }

    // Returns the adjacency matrix.
    public float[][] returnArray()
    {
        return adjMatrix;
    }

    // Add new edges.
    public void addEdge(int i, int j, float weight)
    {
        adjMatrix[i][j] = weight;
        adjMatrix[j][i] = weight;
    }

    // Remove the edges.
    public void removeEdge(int i, int j, float weight)
    {
        adjMatrix[i][j] = weight;
        adjMatrix[j][i] = weight;
    }

    // Initialize the matrix.
    public Graph(int numVertices)
    {
        this.numVertices = numVertices;
        adjMatrix = new float[numVertices][numVertices];
    }
}

class MST
{
    // Number of vertices in the graph
    private static int V = 5;

    // Find the vertex with the minimum key value
    // from the set of vertices not yet included in MST. Return that index.
    int minKey(float[] key, Boolean[] mstSet, int numOfVertices)
    {
        V = numOfVertices;

        // Initialize min value
        int min = Integer.MAX_VALUE, min_index = -1;

        for (int v = 0; v < V; v++)
        {
            if (!mstSet[v] && key[v] < min)
            {
                min = (int) key[v];
                min_index = v;
            }
        }

        return min_index;
    }

    // Print the constructed MST stored in parent[].
    void printMST(float[] parent, float[][] graph, int numOfVertices)
    {
        V = numOfVertices;

        float sum = 0;
        for (int i = 1; i < V; i++)
        {
            System.out.printf("%.0f-%d %.5f\n", parent[i], i, graph[i][(int) parent[i]]);
            sum += graph[i][(int) parent[i]];
        }

        System.out.printf("%.5f\n", sum);
    }

    // Construct MST for a graph represented using adjacency matrix representation.
    void primMST(float graph[][], int numVertices)
    {
        V = numVertices;

        // Array to store constructed MST.
        float[] parent = new float[V];

        // Key values used to pick minimum weight edge in cut.
        float[] key = new float[V];

        // To represent set of vertices included in MST.
        Boolean[] mstSet = new Boolean[V];

        // Initialize all keys as INFINITE.
        for (int i = 0; i < V; i++)
        {
            key[i] = Integer.MAX_VALUE;
            mstSet[i] = false;
        }

        // Make key 0 so that this vertex is picked as the 1st vertex.
        // Always include first 1st vertex in MST.
        key[0] = 0;

        // First node is always root of MST.
        parent[0] = -1;

        // The MST will have V vertices.
        for (int count = 0; count < V - 1; count++)
        {
            int minindex = minKey(key, mstSet, numVertices);
            mstSet[minindex] = true;

            for (int j = 0; j < V; j++)
            {
                if (graph[minindex][j] != 0 && !mstSet[j] && graph[minindex][j] < key[j])
                {
                    parent[j] = minindex;
                    key[j] = graph[minindex][j];
                }
            }
        }

        // Print the constructed MST.
        printMST(parent, graph, numVertices);
    }
}
