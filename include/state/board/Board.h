#pragma once

#include "state/Types.h"
#include "state/board/Player.h"

namespace state
{
	namespace board
	{
		class Board
		{
		public:
			Board() : first_(), second_() {}

			void FillWithBase(Board const& base)
			{
				first_.FillWithBase(base.first_);
				second_.FillWithBase(base.second_);
			}

			Player & Get(PlayerSide side) { return Get(PlayerIdentifier(side)); }
			Player const& Get(PlayerSide side) const { return Get(PlayerIdentifier(side)); }

			Player & Get(PlayerIdentifier identifier)
			{
				if (identifier.IsFirst()) return first_;
				else return second_;
			}

			const Player & Get(PlayerIdentifier identifier) const
			{
				if (identifier.IsFirst()) return first_;
				else return second_;
			}

			Player & GetFirst() { return first_; }
			Player const& GetFirst() const { return first_; }

			Player & GetSecond() { return second_; }
			Player const& GetSecond() const { return second_; }

		private:
			Player first_;
			Player second_;
		};
	}
}