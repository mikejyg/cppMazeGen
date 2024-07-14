/*
 * RectMazeGenerator.h
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

#ifndef RECTMAZEGENERATOR_H_
#define RECTMAZEGENERATOR_H_

#include "Graph.h"
#include "RandomWalk.h"
#include "RectGraph.h"
#include <set>
#include <iostream>
#include <memory>

/**
 * a utility class with tests.
 */
class RectMazeGenerator {
public:
	typedef std::pair<Node const *, Node const *> PathType;
private:
	std::set< PathType > paths;
	unsigned loopEraseCnt;
	unsigned walkCnt;

public:
//	RectMazeGenerator();
//	virtual ~RectMazeGenerator();

	std::unique_ptr<RectGraph> generate(int width, int height) {
		//	std::cout << "generateMaze(" << width << ", " << height << ")...";

		std::unique_ptr<RectGraph> mazeUptr(new RectGraph);
		auto & maze=*mazeUptr;

		maze.init(width, height);

		std::tie(paths, walkCnt, loopEraseCnt) = RandomWalk::randomWalk( maze.getNodes() );

//		auto r  = randomWalk( maze.getNodeConnMap() );
//		paths = std::move( std::get<0>(r) );
//		walkCnt=std::get<1>(r);
//		loopEraseCnt=std::get<2>(r);

		//	std::cout << "done." << std::endl;

		//	maze.printMazeText();

		//	mg.printStatistics();

		return mazeUptr;
	}

	/////////////////////////////////

	std::set< PathType > const & getPaths() const {
		return paths;
	}

	unsigned getLoopEraseCnt() const {
		return loopEraseCnt;
	}

	unsigned getWalkCnt() const {
		return walkCnt;
	}

	//////////////////////////////////////

	static void test1() {
		RectMazeGenerator mazeGen;

		auto mazeUptr=mazeGen.generate(10,10);
		auto & maze=*mazeUptr;

		maze.printConnections();

		maze.printGraphText(mazeGen.getPaths());

		RandomWalk::printStatistics(mazeGen.getWalkCnt(), mazeGen.getLoopEraseCnt());

	}

	static void test2() {
		RectMazeGenerator mazeGen;
		std::cout << "40x20" << std::endl;
		auto mazeUptr = mazeGen.generate(40,20);
		mazeUptr->printGraphText(mazeGen.getPaths());

		std::cout << "20x40" << std::endl;
		auto mazeUptr1 = mazeGen.generate(20,40);
		RandomWalk::printStatistics(mazeGen.getWalkCnt(), mazeGen.getLoopEraseCnt());

		// find a path from one corner to another.
		auto & maze = *mazeUptr1;
		auto r = Graph::traverseNode( maze.getNode(0,0), mazeGen.getPaths(), & maze.getNode(19,39) );

		std::cout << ( std::get<1>(r) ? "true" : "false" ) << std::endl;

		// print out the path.
		auto & tPath = std::get<2>(r);
		for (auto * nPtr : tPath) {
			std::cout << nPtr->idToString() << " ";
		}
		std::cout << std::endl;
	}

	static void test() {
		test1();
		test2();

		RectMazeGenerator mazeGen;

		// chance of converge are low for extreme narrow mazes, so limit length.
		auto x=2;
		auto y=2;
		while (y<40) {
			std::cout << x << " x " << y << std::endl;
			auto mazeUptr = mazeGen.generate(x,y);
			mazeUptr->printGraphText(mazeGen.getPaths());
			RandomWalk::printStatistics(mazeGen.getWalkCnt(), mazeGen.getLoopEraseCnt());
			y++;
		}

		y=2;
		x=2;
		while (x<40) {
			std::cout << x << " x " << y << std::endl;
			auto mazeUptr = mazeGen.generate(x,y);
			mazeUptr->printGraphText(mazeGen.getPaths());
			RandomWalk::printStatistics(mazeGen.getWalkCnt(), mazeGen.getLoopEraseCnt());
			x++;
		}

		y=3;
		while (y<50) {
			x=3;
			while (x<50) {
				std::cout << x << " x " << y << std::endl;
				auto mazeUptr = mazeGen.generate(x,y);
				mazeUptr->printGraphText(mazeGen.getPaths());
				RandomWalk::printStatistics(mazeGen.getWalkCnt(), mazeGen.getLoopEraseCnt());
				x++;
			}
			y++;
		}
	}


};

#endif /* RECTMAZEGENERATOR_H_ */
