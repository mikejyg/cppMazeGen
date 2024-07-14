/*
 * RandomWalk.h
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

#ifndef RANDOMWALK_H_
#define RANDOMWALK_H_

#include "Graph.h"
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <set>
#include <tuple>
#include <map>

class RandomWalk {
public:

	/**
	 * use the random walk algorithm to generate a maze.
	 * graph is a collection of nodes.
	 */
	template<typename NodesType>
	static std::tuple< std::set< std::pair<Node const *, Node const *> >, unsigned, unsigned >
		randomWalk(NodesType const & nodes) {

		typedef std::pair<Node const *, Node const *> PathType;
		typedef std::set<PathType> PathsType;

		std::set< Node const *> mazeNodes;
		PathsType paths;

		std::vector< Node const *> walkNodeConnVec;

		// map to walkNodeVec index.
		std::map< Node const *, unsigned > walkNodeMap;

		// statistics
		unsigned walkCnt=0;
		unsigned loopEraseCnt=0;


		for (auto & startNode : nodes) {
			//			pick any node that is not already in the maze.
			if ( mazeNodes.find(&startNode) != mazeNodes.end() )
				continue;

			walkNodeConnVec.push_back(& startNode);
			walkNodeMap.emplace(& startNode, walkNodeConnVec.size()-1);

			// do the random walk.
			while (true) {
				auto & lastNode = * walkNodeConnVec.back();
				auto & conns = lastNode.getConnections();
				auto connCnt=conns.size();

				// do not count the walk back connection.
				if (walkNodeConnVec.size()!=1)
					connCnt--;

				auto conn=int( floor(drand48() * connCnt) );

				Node const * nextNodePtr=nullptr;

				for ( auto * nPtr : conns ) {
					// skip walk back.
					if (walkNodeConnVec.size()!=1) {
						auto * prevNodeConnsPtr=walkNodeConnVec[walkNodeConnVec.size()-2];
						if ( nPtr == prevNodeConnsPtr )
							continue;
					}

					if (conn!=0) {
						conn--;
						continue;
					}

					nextNodePtr = nPtr;
					break;
				}
				if ( ! nextNodePtr )
					throw std::runtime_error( std::string("failed to find next node for conn: ") + std::to_string(conn) );

				if ( walkNodeMap.find(nextNodePtr) != walkNodeMap.end() ) {	// loop formed

					// use forming a loop as the criterion for adding the initial walk.
					if (mazeNodes.empty())
						break;

					// in walk nodes, erase all nodes starting after s1
					auto idx=walkNodeMap[nextNodePtr];
					for (auto i=idx+1; i<walkNodeConnVec.size(); i++) {
						walkNodeMap.erase(walkNodeConnVec[i]);
					}
					walkNodeConnVec.resize(idx+1);

					loopEraseCnt++;
					continue;
				}

				walkNodeConnVec.push_back(nextNodePtr);
				walkNodeMap.emplace(nextNodePtr, walkNodeConnVec.size()-1);

				if ( mazeNodes.find(nextNodePtr) != mazeNodes.end() )	// joined the maze
					break;

			}

			if (mazeNodes.empty()) {
				// add the last node to the maze.
				mazeNodes.insert(walkNodeConnVec.back());
			}

			for (auto i=0u; i<walkNodeConnVec.size()-1; i++) {
				auto * nodePtr = walkNodeConnVec[i];
				mazeNodes.insert(nodePtr);

				// remove the wall to the next node
				auto * nextNodePtr=walkNodeConnVec[i+1];

				paths.emplace(nodePtr, nextNodePtr);
				paths.emplace(nextNodePtr, nodePtr);

			}

			walkNodeConnVec.clear();
			walkNodeMap.clear();

			walkCnt++;

		}

		std::cout << "verifying graph...";
		Graph::traverse(nodes, paths);
		std::cout << " passed." << std::endl;

		return std::make_tuple( paths, walkCnt, loopEraseCnt );

	}

	static void printStatistics(unsigned walkCnt, unsigned loopEraseCnt) {
		std::cout << "walkCnt: " << walkCnt << ", loopEraseCnt: " << loopEraseCnt << std::endl;
	}


};

#endif /* RANDOMWALK_H_ */
