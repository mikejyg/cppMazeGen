/*
 * Graph.h
 *
Copyright (C) 2024 Junyang Gu

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <set>
#include <string>
#include <stdexcept>
#include <tuple>

class Node {
private:
	std::vector<Node *> connections;

public:
	Node() {}
	virtual ~Node() {}

	const std::vector<Node*> & getConnections() const {
		return connections;
	}

	std::vector<Node*> & getConnections() {
		return connections;
	}

	virtual std::string idToString() const {
		return std::to_string( (long long unsigned) this );
	}

	virtual std::string toString() const {
		auto s = idToString() + ":(";
		bool first=true;
		for (auto c : connections) {
			if (first)
				first=false;
			else
				s += ", ";
			s += c->idToString();
		}
		s += ")";

		return s;
	}

};

/**
 * a graph is a collection of nodes
 */
class Graph {
protected:
	template<typename PathsType>
	static bool traverse_rec(PathsType const & paths, std::set<Node const *> & visitedNodes, std::vector<Node const *> & tPath
			, Node const & node, Node const * stopNodePtr=nullptr, Node const * prevNodePtr=nullptr) {
		visitedNodes.insert(&node);
		tPath.push_back(&node);

		if ( stopNodePtr && &node == stopNodePtr )
			return true;

		for (auto * cPtr : node.getConnections() ) {
			if ( paths.find(std::make_pair(&node, cPtr)) == paths.end() )
				continue;
			if ( prevNodePtr && cPtr==prevNodePtr )	// do not go backwards.
				continue;

			if ( visitedNodes.find(cPtr) != visitedNodes.end() ) {
				throw std::runtime_error( std::string("traverse() loop at: ") + cPtr->toString() );
			}

			if ( traverse_rec(paths, visitedNodes, tPath, *cPtr, stopNodePtr, &node) )
				return true;
		}

		tPath.pop_back();
		return false;
	}

public:

	/**
	 * traverse fron a given node.
	 */
	template<typename PathsType>
	static auto traverseNode(Node const & node, PathsType const & paths, Node const * stopNodePtr=nullptr)
		-> std::tuple<unsigned, bool, std::vector<Node const *> >
	{
		std::set<Node const *> visitedNodes;

		std::vector<Node const *> tPath;	// to record the traversal path.

		auto k = traverse_rec(paths, visitedNodes, tPath, node, stopNodePtr);

		return std::make_tuple( visitedNodes.size(), k, tPath );

	}

	/**
	 * check whether all nodes in the graph is connected,
	 *   and there is no loop.
	 * paths is something like: std::set<NodeType const *, NodeType const *>
	 *
	 * return the number of nodes traversed.
	 */
	template<typename GraphType, typename PathsType>
	static void traverse(GraphType const & graph, PathsType const & paths) {
		auto it=graph.begin();
		if (it==graph.end())
			return;

		unsigned k;
		std::tie(k, std::ignore, std::ignore) = traverseNode(*it, paths);

		if ( k != graph.size() ) {
			throw std::runtime_error( std::string("traverse() not all nodes visited: ")
					+ std::to_string(k) + " vs " + std::to_string(graph.size()) );
		}

	}


};

#endif /* GRAPH_H_ */
