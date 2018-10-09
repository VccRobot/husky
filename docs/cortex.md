# Husky Cortex Module
This module provides high level perception and path planning for the robot.
## husky_cortex ROS Package Documentation
### Nodes and Topics
`cortexMap` subscribes `location`, publishes `meshmap` and `reebgraph`
`cortexPlanner` subscribes `location`, `meshmap` and `reebgraph`, publishes `next_waypoints`

#### Nodes
* **`cortexMap`** builds the simplified 3d map incrementally while moving. It subscribes `location`, publishes `meshmap` and `reebgraph`

* **`cortexPlanner`** computes coverage path and plans the next move. It subscribes `location`, `meshmap` and `reebgraph`, publishes `next_waypoints`
* **`cortexWorld`** provides APIs for the cortex module. It subscribes `next_waypoints`, publishes `location`
* **`cortexViewer`** display meshes, topological graphs and other informations. It subscribes to all the topics listed below.
#### Topics
* **`meshmap`**
* **`reebgraph`**
* **`next_waypoint`**
* **`location`**

### Message Types
* **`meshmap`** which contains:
    * `V` a V by 1 vector.
    * `E` a E by 1 vector.
    * `reebi` a V by 1 vector, which encodes which reebgraph node a vertex belongs.
    * `vstat` a V by 1 vector, which encodes the status of the vertices.
        * `0` means the vertex is undiscovered.
        * `1` means the vertex is discovered.
    * `vtype` a V by 1 vector, which encodes the type of the vertices.
        * `0` means the vertex is an ordinary point.
        * `1` means the vertex is a boundary point.
        * `2` means the vertex is a criticle point.
* **`reebgraph`**
    * 'graph' an adjacency list which represents the graph. 
    * 'vtype' a  V by 1 vector, which encodes type of the vertices.
        * `0` means the vertex is unknown, which means we know there is a vertex undiscovered, but we haven't met it yet.
        * `1` means the vertex is known.
    * 'etype' an E by 1 vector, which encodes type of the edges.
        * `0` means the edge is unknown, which means we know there is a vertex undiscovered, but we haven't met it yet.
        * `1` means the edge is known.
        * `2` means the edge is partially known, which means we know there is an edge, but the other end of it is unknown.
* **`adjlist`** a V by 1 vector of `adjedgelist` 
* **`adjedgelist`** a variable-length vector of `adjedge`
* **`adjedge`**
    * 'from' a vertex index
    * 'to' a vertex index
    * 'cost' a float64


