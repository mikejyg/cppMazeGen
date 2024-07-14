/*
 * RectGraph.h
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

#ifndef RECTGRAPH_H_
#define RECTGRAPH_H_

#include "Graph.h"
#include <iostream>
#include <string>
#include <cstring>

// rectangular directions
enum class RectDirection : unsigned {
	Right=0
	, Up=1
	, Left=2
	, Down=3
};

/**
 * the ID of nodes in a rectangular graph is its coordinates (x,y), starting from (0,0).
 */
class RectGraphNode : public Node {
private:
	int x, y;

public:
	virtual ~RectGraphNode() {}

	virtual std::string idToString() const override {
		return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
	}

	void setXy(int x, int y) {
		this->x=x;
		this->y=y;
	}

	int getX() const {
		return x;
	}

	void setX(int x) {
		this->x=x;
	}

	int getY() const {
		return y;
	}

	void setY(int y) {
		this->y=y;
	}

};


/**
 * graph for nodes in a rectangular grid.
 */
class RectGraph {
public:
	typedef std::vector< RectGraphNode > NodesType;

private:
	int columns;
	int rows;

	NodesType nodes;

public:
	void init(int columns, int rows) {
		if (columns<0 || rows<0) {
			throw std::runtime_error("init() dimensions cannot be negative.");
		}

		this->columns=columns;
		this->rows = rows;

		// initialize cells
		getNodes().resize(columns * rows);

		for (auto y=0; y<rows; y++) {
			for (auto x=0; x<columns; x++) {

				auto & node = getNode(x,y);

				node.setXy(x,y);

				auto & conns = node.getConnections();
				if (x<columns-1)
					conns.emplace_back( & getNode(x+1, y) );
				if (y<rows-1)
					conns.emplace_back( & getNode(x, y+1) );
				if (x>0)
					conns.emplace_back( & getNode(x-1, y) );
				if (y>0)
					conns.emplace_back( & getNode(x, y-1) );

			}

		}

	}

	RectGraphNode const & getNode(int x, int y) const {
		return getNodes()[x + y*columns];
	}

	RectGraphNode & getNode(int x, int y) {
		return getNodes()[x + y*columns];
	}

	const NodesType & getNodes() const {
		return nodes;
	}

	NodesType & getNodes() {
		return nodes;
	}

	///////////////////////////////////////////////

	/**
	 * print a text representation of the nodes and paths.
	 */
	template<typename PathsType>
	void printGraphText(PathsType const & paths) const {
		auto textWidth=columns*2+2;
		auto textHeight=rows*2+1;

		auto * textPtr = new char[textWidth * textHeight];
		memset(textPtr, ' ', textWidth * textHeight);

		for (auto & node : getNodes()) {
			auto & conns = node.getConnections();

			auto x=node.getX();
			auto y=node.getY();

			auto textIdx = (y*2 +1) * textWidth + x * 2+1;
				textPtr[textIdx] = '+';

			for (auto * nPtr : conns ) {
				if ( paths.find( std::make_pair(&node, static_cast<RectGraphNode const *>(nPtr)) ) == paths.end() )
					continue;
				auto & n = * static_cast<RectGraphNode const *>(nPtr);
				if (n.getX()==x+1)
					textPtr[textIdx + 1] = '-';
				else if (n.getY()==y+1)
					textPtr[textIdx + textWidth] = '|';
				else if (n.getX()==x-1)
					textPtr[textIdx-1] = '-';
				else if (n.getY()==y-1)
					textPtr[textIdx - textWidth] = '|';
				else
					throw std::runtime_error( std::string("printGraphText() invalid connection: ") + n.toString() );

			}

		}

		for (auto l=0; l<textHeight; l++) {
			textPtr[l*textWidth + textWidth-1] = '\0';
			std::cout << textPtr + l*textWidth << std::endl;
		}

		delete [] textPtr;
	}

	void printConnections() const {
		for (auto & node : nodes) {
			std::cout << node.toString() << ":(";
			bool first=true;
			for (auto * cPtr : node.getConnections() ) {
				if (first)
					first=false;
				else
					std::cout << ", ";
				std::cout << cPtr->toString();
			}
			std::cout << ")" << std::endl;
		}
	}


};

#endif /* RECTGRAPH_H_ */
