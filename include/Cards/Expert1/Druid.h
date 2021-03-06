#pragma once

// http://www.hearthpwn.com/cards?filter-set=3&filter-class=4&sort=-cost&display=1
// Done.

namespace Cards
{
	struct Card_EX1_161 : public SpellCardBase<Card_EX1_161> {
		Card_EX1_161() {
			onplay_handler.SetSpecifyTargetCallback([](Contexts::SpecifiedTargetGetter & context) {
				context.SetRequiredSpellTargets(context.player_).Minion();
			});
			onplay_handler.SetOnPlayCallback([](FlowControl::onplay::context::OnPlay const& context) {
				context.manipulate_.OnBoardMinion(context.GetTarget()).Destroy();
				context.manipulate_.Player(context.player_.Opposite()).DrawCard();
				context.manipulate_.Player(context.player_.Opposite()).DrawCard();
			});
		}
	};

	struct Card_EX1_578 : public SpellCardBase<Card_EX1_578> {
		Card_EX1_578() {
			onplay_handler.SetSpecifyTargetCallback([](Contexts::SpecifiedTargetGetter & context) {
				context.SetRequiredSpellTargets(context.player_).Minion();
			});
			onplay_handler.SetOnPlayCallback([](FlowControl::onplay::context::OnPlay const& context) {
				auto & hero = context.manipulate_.Board().Player(context.player_);
				state::CardRef hero_ref = hero.GetHeroRef();
				context.manipulate_.OnBoardMinion(context.GetTarget())
					.Damage(context.card_ref_, context.manipulate_.GetCardAttackConsiderWeapon(hero_ref));
			});
		}
	};

	struct Card_EX1_160be : public Enchantment<Card_EX1_160be, Attack<1>, MaxHP<1>> {};
	struct Card_EX1_160 : public SpellCardBase<Card_EX1_160> {
		Card_EX1_160() {
			static std::vector<Cards::CardId> choices{
				Cards::ID_EX1_160a,
				Cards::ID_EX1_160b
			};
			onplay_handler.SetOnPlayCallback([](FlowControl::onplay::context::OnPlay const& context) {
				Cards::CardId choice = context.manipulate_.UserChooseOne(choices);
				if (choice == choices[0]) {
					SummonToRightmost(context.manipulate_, context.player_, Cards::ID_EX1_160t);
				}
				else {
					context.manipulate_.Board().Player(context.player_).minions_.ForEach([&](state::CardRef card_ref) {
						context.manipulate_.OnBoardMinion(card_ref).Enchant().Add<Card_EX1_160be>();
						return true;
					});
				}
			});
		}
	};

	struct Card_EX1_154 : public SpellCardBase<Card_EX1_154> {
		Card_EX1_154() {
			static std::vector<Cards::CardId> choices{
				Cards::ID_EX1_154a,
				Cards::ID_EX1_154b
			};
			onplay_handler.SetSpecifyTargetCallback([](Contexts::SpecifiedTargetGetter & context) {
				context.SetRequiredSpellTargets(context.player_).Minion();
			});
			onplay_handler.SetOnPlayCallback([](FlowControl::onplay::context::OnPlay const& context) {
				Cards::CardId choice = context.manipulate_.UserChooseOne(choices);
				if (choice == choices[0]) {
					context.manipulate_.OnBoardMinion(context.GetTarget()).Damage(context.card_ref_, 3);
				}
				else {
					context.manipulate_.OnBoardMinion(context.GetTarget()).Damage(context.card_ref_, 1);
					context.manipulate_.Player(context.player_).DrawCard();
				}
			});
		}
	};

	struct Card_EX1_155ae : public Enchantment<Card_EX1_155ae, Attack<4>> {};
	struct Card_EX1_155be : public Enchantment<Card_EX1_155be, MaxHP<4>> {};
	struct Card_EX1_155 : public SpellCardBase<Card_EX1_155> {
		Card_EX1_155() {
			static std::vector<Cards::CardId> choices{
				Cards::ID_EX1_155a,
				Cards::ID_EX1_155b
			};
			onplay_handler.SetSpecifyTargetCallback([](Contexts::SpecifiedTargetGetter & context) {
				context.SetRequiredSpellTargets(context.player_).Minion();
			});
			onplay_handler.SetOnPlayCallback([](FlowControl::onplay::context::OnPlay const& context) {
				Cards::CardId choice = context.manipulate_.UserChooseOne(choices);
				if (choice == choices[0]) {
					context.manipulate_.OnBoardMinion(context.GetTarget()).Enchant().Add<Card_EX1_155ae>();
				}
				else {
					context.manipulate_.OnBoardMinion(context.GetTarget()).Enchant().Add<Card_EX1_155be>();
					context.manipulate_.OnBoardMinion(context.GetTarget()).Taunt(true);
				}
			});
		}
	};

	struct Card_EX1_570e : public EnchantmentForThisTurn<Card_EX1_570e, Attack<4>> {};
	struct Card_EX1_570 : public SpellCardBase<Card_EX1_570> {
		Card_EX1_570() {
			onplay_handler.SetOnPlayCallback([](FlowControl::onplay::context::OnPlay const& context) {
				context.manipulate_.Hero(context.player_).Enchant().Add<Card_EX1_570e>();
				context.manipulate_.Hero(context.player_).GainArmor(4);
			});
		}
	};

	struct Card_EX1_158 : public SpellCardBase<Card_EX1_158> {
		Card_EX1_158() {
			onplay_handler.SetOnPlayCallback([](FlowControl::onplay::context::OnPlay const& context) {
				context.manipulate_.Board().Player(context.player_).minions_.ForEach([&](state::CardRef card_ref) {
					context.manipulate_.OnBoardMinion(card_ref).AddDeathrattle([](FlowControl::deathrattle::context::Deathrattle const& context) {
						SummonAt(context, context.player_, context.zone_pos_, Cards::ID_EX1_158t);
					});
					return true;
				});
			});
		}
	};

	struct Card_EX1_166 : public MinionCardBase<Card_EX1_166> {
		static void GetSpecifiedTargets(Contexts::SpecifiedTargetGetter & context) {
			context.SetOptionalBattlecryTargets(context.player_).Minion();
		}
		static void Battlecry(Contexts::OnPlay const& context) {
			state::CardRef target = context.GetTarget();
			if (!target.IsValid()) return;

			static std::vector<Cards::CardId> choices{
				Cards::ID_EX1_166a,
				Cards::ID_EX1_166b
			};
			Cards::CardId choice = context.manipulate_.UserChooseOne(choices);
			if (choice == choices[0]) {
				context.manipulate_.OnBoardMinion(target).Damage(context.card_ref_, 2);
			}
			else {
				context.manipulate_.OnBoardMinion(target).Silence();
			}
		}
	};

	struct Card_EX1_571 : public SpellCardBase<Card_EX1_571> {
		Card_EX1_571() {
			onplay_handler.SetOnPlayCallback([](FlowControl::onplay::context::OnPlay const& context) {
				SummonToRightmost(context.manipulate_, context.player_, Cards::ID_EX1_158t);
				SummonToRightmost(context.manipulate_, context.player_, Cards::ID_EX1_158t);
				SummonToRightmost(context.manipulate_, context.player_, Cards::ID_EX1_158t);
			});
		}
	};

	struct Card_EX1_164 : public SpellCardBase<Card_EX1_164> {
		Card_EX1_164() {
			static std::vector<Cards::CardId> choices{
				Cards::ID_EX1_164a,
				Cards::ID_EX1_164b
			};
			onplay_handler.SetOnPlayCallback([](FlowControl::onplay::context::OnPlay const& context) {
				Cards::CardId choice = context.manipulate_.UserChooseOne(choices);
				if (choice == choices[0]) {
					context.manipulate_.Board().Player(context.player_).GetResource().GainCrystal(2);
				}
				else {
					context.manipulate_.Player(context.player_).DrawCard();
					context.manipulate_.Player(context.player_).DrawCard();
					context.manipulate_.Player(context.player_).DrawCard();
				}
			});
		}
	};

	struct Card_NEW1_007 : public SpellCardBase<Card_NEW1_007> {
		Card_NEW1_007() {
			static std::vector<Cards::CardId> choices{
				Cards::ID_NEW1_007a,
				Cards::ID_NEW1_007b,
			};
			onplay_handler.SetChooseOneOptionsCallback([](Contexts::GetChooseOneOptions & context) {
				return context.manipulate_.UserChooseOne(choices);
			});
			onplay_handler.SetSpecifyTargetCallback([](Contexts::SpecifiedTargetGetter & context) {
				Cards::CardId choice = context.GetChooseOneChoice();
				if (choice == Cards::kInvalidCardId) {
					// get default playable condition do check if this card is playable
				}
				else if (choice == choices[0]) {
				}
				else {
					assert(choice == choices[1]);
					context.SetOptionalSpellTargets(context.player_).Minion();
				}
			});
			onplay_handler.SetOnPlayCallback([](FlowControl::onplay::context::OnPlay const& context) {
				if (context.manipulate_.GetSavedUserChoice() == choices[0]) {
					context.manipulate_.Board().Player(context.player_).minions_.ForEach([&](state::CardRef card_ref) {
						context.manipulate_.OnBoardMinion(card_ref).Damage(context.card_ref_, 2);
						return true;
					});
				}
				else {
					assert(context.manipulate_.GetSavedUserChoice() == choices[1]);
					state::CardRef target = context.GetTarget();
					if (!target.IsValid()) return;
					context.manipulate_.OnBoardMinion(target).Damage(context.card_ref_, 5);
				}
			});
		}
	};

	struct Card_EX1_165t1 : public MinionCardBase<Card_EX1_165t1, Charge> {};
	struct Card_EX1_165t2 : public MinionCardBase<Card_EX1_165t2, Taunt> {};
	struct Card_EX1_165 : public MinionCardBase<Card_EX1_165> {
		static void Battlecry(Contexts::OnPlay const& context) {
			static std::vector<Cards::CardId> choices{
				Cards::ID_EX1_165a,
				Cards::ID_EX1_165b
			};
			Cards::CardId choice = context.manipulate_.UserChooseOne(choices);
			state::CardRef new_ref;
			if (choice == choices[0]) {
				new_ref = Manipulate(context).OnBoardMinion(context.card_ref_).Transform(Cards::ID_EX1_165t1);
			}
			else {
				new_ref = Manipulate(context).OnBoardMinion(context.card_ref_).Transform(Cards::ID_EX1_165t2);
			}
			if (!new_ref.IsValid()) return;
			*context.new_card_ref = new_ref;
		}
	};

	struct Card_NEW1_008 : public MinionCardBase<Card_NEW1_008> {
		Card_NEW1_008() {
			static std::vector<Cards::CardId> choices{
				Cards::ID_NEW1_008a,
				Cards::ID_NEW1_008b
			};
			onplay_handler.SetChooseOneOptionsCallback([](Contexts::GetChooseOneOptions & context) {
				return context.manipulate_.UserChooseOne(choices);
			});
			onplay_handler.SetSpecifyTargetCallback([](Contexts::SpecifiedTargetGetter & context) {
				Cards::CardId choice = context.GetChooseOneChoice();
				if (choice == Cards::kInvalidCardId) {
					// get default target rule to check card is playable or not
				}
				else if (choice == choices[0]) {
				}
				else {
					assert(choice == choices[1]);
					context.SetOptionalBattlecryTargets(context.player_);
				}
			});
			onplay_handler.SetOnPlayCallback([](FlowControl::onplay::context::OnPlay const& context) {
				if (context.manipulate_.GetSavedUserChoice() == choices[0]) {
					context.manipulate_.Player(context.player_).DrawCard();
				}
				else {
					assert(context.manipulate_.GetSavedUserChoice() == choices[1]);
					state::CardRef target = context.GetTarget();
					if (!target.IsValid()) return;
					context.manipulate_.OnBoardCharacter(target).Heal(context.card_ref_, 5);
				}
			});
		}
	};

	struct Card_EX1_178ae : public Enchantment<Card_EX1_178ae, MaxHP<5>> {};
	struct Card_EX1_178be : public Enchantment<Card_EX1_178be, Attack<5>> {};
	struct Card_EX1_178 : public MinionCardBase<Card_EX1_178> {
		static void Battlecry(Contexts::OnPlay const& context) {
			static std::vector<Cards::CardId> choices{
				Cards::ID_EX1_178a,
				Cards::ID_EX1_178b
			};
			Cards::CardId choice = context.manipulate_.UserChooseOne(choices);
			if (choice == choices[0]) {
				context.manipulate_.OnBoardMinion(context.card_ref_).Enchant().Add<Card_EX1_178ae>();
				context.manipulate_.OnBoardMinion(context.card_ref_).Taunt(true);
			}
			else {
				context.manipulate_.OnBoardMinion(context.card_ref_).Enchant().Add<Card_EX1_178be>();
			}
		}
	};

	struct Card_EX1_573ae : public Enchantment<Card_EX1_573ae, MaxHP<2>, Attack<2>> {};
	struct Card_EX1_573t : public MinionCardBase<Card_EX1_573t, Taunt> {};
	struct Card_EX1_573 : public MinionCardBase<Card_EX1_573> {
		static void Battlecry(Contexts::OnPlay const& context) {
			static std::vector<Cards::CardId> choices{
				Cards::ID_EX1_573a,
				Cards::ID_EX1_573b
			};
			Cards::CardId choice = context.manipulate_.UserChooseOne(choices);
			if (choice == choices[0]) {
				context.manipulate_.Board().Player(context.player_).minions_.ForEach([&](state::CardRef card_ref) {
					if (card_ref == context.card_ref_) return true;
					context.manipulate_.OnBoardMinion(card_ref).Enchant().Add<Card_EX1_573ae>();
					return true;
				});
			}
			else {
				SummonToLeft(context.manipulate_, context.card_ref_, Cards::ID_EX1_573t);
				SummonToRight(context.manipulate_, context.card_ref_, Cards::ID_EX1_573t);
			}
		}
	};
}

REGISTER_CARD(EX1_573)
REGISTER_CARD(EX1_573t)
REGISTER_CARD(EX1_178)
REGISTER_CARD(NEW1_008)
REGISTER_CARD(EX1_165t2)
REGISTER_CARD(EX1_165t1)
REGISTER_CARD(EX1_165)
REGISTER_CARD(NEW1_007)
REGISTER_CARD(EX1_164)
REGISTER_CARD(EX1_571)
REGISTER_CARD(EX1_166)
REGISTER_CARD(EX1_158)
REGISTER_CARD(EX1_570)
REGISTER_CARD(EX1_155)
REGISTER_CARD(EX1_154)
REGISTER_CARD(EX1_160)
REGISTER_CARD(EX1_578)
REGISTER_CARD(EX1_161)