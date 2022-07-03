#pragma once
#include "Board.h"

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

		void drawTopOfBoard(int cols);
		void drawBottomOfBoard(int cols);
		void drawMiddleOfBoard(int rows, int cols);
	public:
		AsciiBoard();
		void draw();
	};
}


