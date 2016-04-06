namespace Board
{
	// Adds tokens onto the board
	inline Board& Board::operator+=(Board const& a)
	{
		for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
		{
			for (int w = 0; w < Stage::BOARD_WIDTH; w++)
			{
				// Bitwise OR
				this->boardTiles[h][w].hasToken = 
					(this->boardTiles[h][w].hasToken | a.boardTiles[h][w].hasToken);
			}
		}
		return (*this);
	}

	inline Board const Board::operator+(Board const& a) const
	{
		return Board(*this) += a;
	}

	// Subtract tokens from the board
	inline Board& Board::operator-=(Board const& a)
	{
		for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
		{
			for (int w = 0; w < Stage::BOARD_WIDTH; w++)
			{
				// Bitwise XOR
				this->boardTiles[h][w].hasToken = 
					(this->boardTiles[h][w].hasToken ^ a.boardTiles[h][w].hasToken);
			}
		}
		return (*this);
	}

	inline Board const Board::operator-(Board const& a) const
	{
		return Board(*this) -= a;
	}
}