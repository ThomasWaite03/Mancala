#pragma once
#include "Board.h"
#include <map>
#include <string>

namespace Mancala {
	class AsciiBoard : public Board {
	private:
		const int _horizontalLine;
		const int _verticalLine;
		const int _topLeftCorner;
		const int _topRightCorner;
		const int _bottomLeftCorner;
		const int _bottomRightCorner;
		const int _space;

		const int _storeWidth;
		const int _pocketWidth;
		const int _rows;
		const int _cols;

		void drawTopOfBoard();
		void drawBottomOfBoard();
		void drawMiddleOfBoard();

		int getBoardIndexByCoords(int row, int col);
		void printIntValue(int val);

	public:
		AsciiBoard();
		void draw();
		int labelToBoardPosition(int labelValue, std::string player);
	};
}


