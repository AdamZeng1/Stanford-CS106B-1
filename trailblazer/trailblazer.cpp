/* Ali Parsaei  (aparsaei)      ID: 05950982
 * CS 106B 7th HW       Trailblazer.
 * Sources:
 *
 */

#include "trailblazer.h"
#include "queue.h"
#include "stack.h"
#include "pqueue.h"
// TODO: include any other headers you need; remove this comment

using namespace std;
/* This function is a helper function for DFS with an extra paramater, path Vector. I
 * encode the pseudo code in the lecture slides. */
bool depthFirstHelper(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*>& path) {
    path.add(start);
    start->visited = true;
    start->setColor(GREEN);
    if (start == end)
        return true; // if we reach the end point, return

    for (Vertex* neighbor : graph.getNeighbors(start)) {
        if (!neighbor->visited) // for all unvisited neighbors of the current point,
            if (depthFirstHelper(graph, neighbor,end,path)) // do a DFS, and if found a path
                return true;    // return.
    }

    start->setColor(GRAY);  // change the color of dead-end paths to gray.
    path.remove(path.size()-1);
    return false;           // if no path found, return false
}
/* This function uses depthFirstHelper() to find a path between 2 points if it's feasible. */
Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();      // If we do not reset the data, it won't work after "Clear" button
    Vector<Vertex*> path;   // being pressed on GUI screen.
    path.clear();
    depthFirstHelper(graph,start,end,path);
    return path;
}

/* Helper function for BFS I encode the pseudo code in the lecture slides. extra parameter
 *  is  pathqueue */
Vertex* breadthFirstHelper(BasicGraph& graph, Vertex* start, Vertex* end, Queue<Vertex*>& pathQueue) {
    while (!pathQueue.isEmpty()) {
        Vertex* currentElmn = pathQueue.dequeue();  // dequeue the first element in queue
        currentElmn->setColor(GREEN);   // set it as green
        if (currentElmn == end) {
            return currentElmn;         // if we reach the end return the element
        }

        for (Vertex* neighbor : graph.getNeighbors(currentElmn)) {
            if (!neighbor->visited) {
                neighbor->visited = true;   // whenever visit a node, save it as visited
                neighbor->setColor(YELLOW);
                neighbor->previous = currentElmn;   // link the node to its previous one
                pathQueue.enqueue(neighbor);        // enqueueit in the queue
            }
        }

    }
    return start;   // if we do not find a oath, return the start node as a notion of
}                   // not finding a path.

/* This function uses breadthFirstHelper() to find the shortest path between 2 points
 * if it's feasible. */
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    Queue<Vertex*> pathQueue;

    pathQueue.enqueue(start);   //enqueue start in the queue.
    start->visited = true;
    start->setColor(YELLOW);
    start->setColor(GREEN);
    path.add(start);            // add it to the path.


    Stack<Vertex*> pathStack;
    pathStack.clear();

    if (start != end) {
        Vertex* endVert = breadthFirstHelper(graph, start, end, pathQueue);
        if (endVert == end) {   // if we found a path
            while (endVert->previous != NULL) {
                endVert->setColor(GREEN);   // we set the color to green
                pathStack.add(endVert);     // collect the node in a stack
                endVert = endVert->previous; // set the previous to current node
            }
            while (!pathStack.isEmpty()) {
                path.add(pathStack.pop()); // path is built
            }
        }
    }

    return path;
}

/* This function is a helper function for Dijkstras Algorithm with an extra paramater, pathQueue. I
 * encode the pseudo code in the lecture slides. */
Vertex* dijkstrasHelper(BasicGraph& graph, Vertex* start, Vertex* end, PriorityQueue<Vertex*>& pathQueue) {
    while (!pathQueue.isEmpty()) {
        Vertex* currentElmn = pathQueue.dequeue();
        currentElmn->setColor(GREEN);   // taking out the most urgent node and marking it
        currentElmn->visited = true;    // as visited.
        if (currentElmn == end)
            return currentElmn;         // if we reach the dstination, we have found a path
        // Otherwise, recurse over unvisited neighbors
        for (Vertex* neighbor : graph.getNeighbors(currentElmn)) {

            if (!neighbor->visited) {
                double newCost = currentElmn->cost + graph.getEdge(currentElmn, neighbor)->weight;
                if (newCost < neighbor->cost) {
                    neighbor->cost = newCost;   // if we find a cheaper path, we set it
                    neighbor->setColor(YELLOW); // as the path to this node.
                    neighbor->previous = currentElmn;
                    pathQueue.enqueue(neighbor, newCost);   // enqueue the neighbor with new
                }                                           // cost into p-queue.
            }

        }

    }

    return start;   // if we do not find a oath, return the start node as a notion of
}                   // not finding a path.


/* This function uses dijkstrasHelper() to execute Dijkstras Algorithm to find an optimal path. */
Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    PriorityQueue<Vertex*> pathQueue;

    for (Vertex* node : graph)          // For all nodes,
        node->cost = POSITIVE_INFINITY; // Initializing the cost to infifnity.

    start->cost = 0;            // starting point's cost is zero
    start->setColor(GREEN);
    pathQueue.enqueue(start,0); // enqueue start node into the p-queue.
    path.add(start);            // add the start point to path, too.

    Stack<Vertex*> pathStack;
    pathStack.clear();

    if (start != end) {
        Vertex* endVert = dijkstrasHelper(graph, start, end, pathQueue);
        if (endVert == end) {   // if we found a path
            while (endVert->previous != NULL) {
                endVert->setColor(GREEN);   // we set the color to green
                pathStack.add(endVert);     // collect the node in a stack
                endVert = endVert->previous; // set the previous to current node
            }
            while (!pathStack.isEmpty()) {
                path.add(pathStack.pop()); // path is built
            }
        }
    }

    return path;
}

/* This function is a helper function for A* Algorithm with an extra paramater, pathQueue. I
 * encode the pseudo code in the lecture slides. */
Vertex* aStarHelper(BasicGraph& graph, Vertex* start, Vertex* end, PriorityQueue<Vertex*>& pathQueue) {
    while (!pathQueue.isEmpty()) {
        Vertex* currentElmn = pathQueue.dequeue();
        currentElmn->setColor(GREEN);   // taking out the most urgent node and marking it
        currentElmn->visited = true;    // as visited.
        if (currentElmn == end)
            return currentElmn;         // if we reach the dstination, we have found a path
        // Otherwise, recurse over unvisited neighbors
        for (Vertex* neighbor : graph.getNeighbors(currentElmn)) {

            if (!neighbor->visited) {
                double newCost = currentElmn->cost + graph.getEdge(currentElmn, neighbor)->weight;
                if (newCost < neighbor->cost) {
                    neighbor->cost = newCost;   // if we find a cheaper path, we set it
                    neighbor->setColor(YELLOW); // as the path to this node.
                    neighbor->previous = currentElmn;
                    pathQueue.enqueue(neighbor, newCost + heuristicFunction(neighbor, end));
                }   // Enqueue the neighbor with new cost + heuristic cost to reach the end.
            }

        }

    }

    return start;   // if we do not find a oath, return the start node as a notion of
}                   // not finding a path.

/* This function uses aStarHelper() to execute A* Algorithm. The deifference between
 * A* and Dijkstra is the heuristic cost considered in A* algorithm which results in
 * exploring fewer # of nodes while getting the path with same cost as dijkstra. */
Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    PriorityQueue<Vertex*> pathQueue;

    for (Vertex* node : graph)          // For all nodes,
        node->cost = POSITIVE_INFINITY; // Initializing the cost to infifnity.

    start->cost = 0;            // starting point's cost is zero
    start->setColor(GREEN);
    pathQueue.enqueue(start,heuristicFunction(start, end)); // enqueue start node into the p-queue.
    path.add(start);            // add the start point to path, too

    Stack<Vertex*> pathStack;
    pathStack.clear();

    if (start != end) {
        Vertex* endVert = aStarHelper(graph, start, end, pathQueue);
        if (endVert == end) {   // if we found a path
            while (endVert->previous != NULL) {
                endVert->setColor(GREEN);   // we set the color to green
                pathStack.add(endVert);     // collect the node in a stack
                endVert = endVert->previous; // set the previous to current node
            }
            while (!pathStack.isEmpty()) {
                path.add(pathStack.pop()); // path is built
            }
        }
    }

    return path;
}

/* This function is used in kruskal, and returns true if the 2 vertices are
 * in the same cluster. */
bool sameCluster(Vertex* v1, Vertex* v2, Set<Set<Vertex*> >& clustersSet) {
    for (Set<Vertex*> cluster : clustersSet) {
        if (cluster.contains(v1) && cluster.contains(v2))
            return true;
    }
    return false;
}

/* This helper function finds the cluster a node belongs to. -*/
Set<Vertex*> findCluster(Vertex* node, Set<Set<Vertex*> >& clustersSet) {
    Set<Vertex*> foundCluster;
    for (Set<Vertex*> cluster : clustersSet) {
        if (cluster.contains(node))
            foundCluster = cluster;
    }
    return foundCluster;
}

/* kruskal gives us the minimum spanning tree of a graph. */
Set<Edge*> kruskal(BasicGraph& graph) {
    Set<Edge*> allEdgeSet = graph.getEdgeSet();
    Set<Edge*> MST; // The Set of edges which form the spanning tree
    MST.clear();
    // each cluster = a set of vertices.
    Set<Set<Vertex*> > clustersSet; // set of all clusters at each moment
    for (Vertex* node : graph) { // adding each vertex to its own cluster
        Set<Vertex*> newCluster;
        newCluster.add(node);
        clustersSet.add(newCluster);
    }

    PriorityQueue<Edge*> edgeQueue;
    for (Edge* edge : allEdgeSet) // finding all the edges in the graph.
        edgeQueue.enqueue(edge, edge->weight);

    while (clustersSet.size() > 1) { // while we have more than a single cluster
        Edge* edge = edgeQueue.dequeue(); // dequeue an edge and see if its start and finish
        if (!sameCluster(edge->start, edge->finish, clustersSet)) { // points are from different clusters
            Set<Vertex*> startCluster = findCluster(edge->start, clustersSet);
            Set<Vertex*> finishCluster = findCluster(edge->finish, clustersSet);
            // if so, remove the clusters from the set and add the union of both
            clustersSet.remove(findCluster(edge->start, clustersSet));
            clustersSet.remove(findCluster(edge->finish, clustersSet));
            Set<Vertex*> mergeCluster = startCluster + finishCluster;
            clustersSet.add(mergeCluster);
            MST.add(edge); // finally, add the edge to MST
        }
    }

    return MST;
}
