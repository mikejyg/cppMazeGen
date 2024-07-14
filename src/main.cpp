/*
 * main.cpp
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

#include "RectMazeGenerator.h"
#include <iostream>

void help() {
	std::cout << "usages: [-b] | width height" << std::endl;
	std::cout << "\t-b: built-in-test." << std::endl;
}

void generateMazeInteractive(int width, int height) {
	std::cout << "generateMaze(" << width << ", " << height << ")...";

	RectGraph graph;
	graph.init(width, height);

	auto r = RandomWalk::randomWalk( graph.getNodes() );

	std::cout << "done." << std::endl;

	graph.printGraphText(std::get<0>(r));

	RandomWalk::printStatistics(std::get<1>(r), std::get<2>(r));

}

void exitMsg(std::string const & s) {
	std::cout << s << std::endl;
	exit(1);
}

int main(int argc, char **argv) {
	if (argc<2) {
		help();
	} else {
		if ( strcmp("-b", argv[1])==0 ) {
			RectMazeGenerator::test();
		} else {
			if (argc<3) {
				exitMsg("missing argument w, h.");
			}
			generateMazeInteractive(atoi(argv[1]), atoi(argv[2]));
		}
	}

}
