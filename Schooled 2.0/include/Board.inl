namespace Board
{
	// Adds tokens onto the board
	inline Board& Board::operator+=(Board const& a)
	{
		for (int w = 0; w < Stage::BOARD_WIDTH; w++)
		{
			for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
			{
				this->boardTiles[w][h].hasToken = 
					(this->boardTiles[w][h].hasToken | a.boardTiles[w][h].hasToken);
				//this->boardTiles[w][h].isPassable += a.boardTiles[w][h].isPassable;
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
		for (int w = 0; w < Stage::BOARD_WIDTH; w++)
		{
			for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
			{
				this->boardTiles[w][h].hasToken = 
					(this->boardTiles[w][h].hasToken ^ a.boardTiles[w][h].hasToken);
				//this->boardTiles[w][h].isPassable -= a.boardTiles[w][h].isPassable;
			}
		}
		return (*this);
	}

	inline Board const Board::operator-(Board const& a) const
	{
		return Board(*this) -= a;
	}
}