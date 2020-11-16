Quicksilver
======

# Abstract
Massive graph-structured data collections are ubiquitous in contemporary data management scenarios such as social networks, Semantic Web, and data-driven exploration in life science such as chemical compound databases. Efficient implementations of graph database engines are the key ingredients to be able to extract and analyse data/patterns in that particular form. However, the lack of structural schema compared to relational databases makes it less straightorward to come up with good excution pipelines.   

To address this problem, we propose a simple but complete graph data base engine, *Quicksilver*. 

In this project we deal with data encoded in the RDF format :

![alt text](https://techcrunch.com/wp-content/uploads/2020/02/cypher_graph_v2a.png?w=1390&crop=1 "Graph database example")


| Source `s`    | Edge Label `l`| Target `t`|
|:-------------:|:-------------:|:---------:|
| John          | LIKES         | Java      |
| Jennifer      | IS_FRIEND_WITH| John      |
| Jennifer      | WORKS_FOR     | Neo4j     |
| Jennifer      | LIKES         | Graphs    |
| Diana         | LIKES         | Graphs    |
| John          | IS_FRIEND_WITH| Sally     |
| Jennifer      | IS_FRIEND_WITH| Melissa   |
| ...           | ...           | ...       |

This engine is simple in the sense that it is only able to deal with integer data types and supports exclusively regular path queries. A query is always of the form 

`(s,expr,t)`

where `s` and `t` belong to the set of nodes in the graph plus the special symbol `*` representing an unbounded source or target. We require that not both source and target are unbounded in a query.

The regular language `expr` has the following rules : 

1. `l>```````         : **Simple path**. If it exists an edge with label `l` that does from the source vertext `s` to the target vertex `t` then `(s,t)` is in the language defined by `l>`
2. `l<`         : **Reversed path**. Same as before but with `s` and `t` reversed
3. `expr1/expr2`: **Concatenation**. There exists a path linking expression 1 and 2, is other words if the tuple `(s1,t1)` is in expression 1 and `(s2,t2)` is in expression 2, `(s1,t2)` will be in the language of the resulting concatenation expression if and only if `t1=s2`
4. `l+`         : **Kleene Star**. The transitive closure of label `l`, a.k.a. the set of all possible edges `(s,t)` such that `(s,l>,t)` is a valid RDF tuple. 

Using this language, we can, for instance construct the following queries 

| n   | Query                                   | Output                                                |
|:----|:----------------------------------------|:------------------------------------------------------|
|1    | `(John,LIKES>,Java)`                    | `(John,Java)`                                         |
|2    | `(*,LIKES>,Graphs)`                     | `(Jennifer,Graphs), (Diana,Graphs) `                  |
|3    | `(*,IS_FRIEND_WITH>LIKES/,Java)`        | `(Jennifer,Java)`                                     |
|4    | `(Jennifer,IS_FRIEND_WITH+,*)`          | `(Jennifer,John),(Jennifer,Sally),(Jennifer,Melissa)` |
|5    | `(Jennifer,IS_FRIEND_WITH+/LIKES>,*)`   | `(Jennifer,Java)`                                     |

> One might think that the integer restriction hardly constrains the set of possible applications but we should notice that it is always possible to encode strings or other datatypes in terms of integers. In that sense, we can transform the previous RDF table as 

| Source `s`    | Edge Label `l`| Target `t`|
|:-------------:|:-------------:|:---------:|
| 0             | 0             | 3         |
| 1             | 1             | 1         |
| 1             | 2             | 4         |
| 1             | 0             | 5         |
| 2             | 0             | 5         |
| ...           | ...           | ...       |

Furthermore, this project is complete since a fully functional, Smart, Quick and Frugal, graph data base engine was built from scratch and contains all the ingredients that one may find a standard database engine:

* Special Index data structures such as a [K-path index](https://github.com/giedomak/Telepath/blob/master/README.md)
* Query optimization through cardinality estimation (path synopses) and query planning
* Special Query Evaluation operators : hash, merge and index joins as well as efficient transitive closure implementations.  

# Credits

Giordano Lucas. Quicksilver: A simple graph database engine.
*Database Technology*, BSc Course. Department of Mathematics and Computer Science, Eindhoven University of Technology. 2019. [PDF](link)


# Authors

|                                                   |                                                      |                                                |
|:-------------------------------------------------:|:----------------------------------------------------:|:----------------------------------------------:|
|[Lucas Giordano](https://github.com/giordan-lucas) | [Karina Mankevic](https://github.com/KarinaMankevic) | [ Wessel van Liero](https://github.com/valepr) |

