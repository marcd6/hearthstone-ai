#pragma warning( disable : 4100 4267)

#include <unordered_set>
#include <cassert>
#include <functional>
#include <iostream>
#include <memory>
#include <type_traits>
#include "State/State.h"
#include "FlowControl/FlowController.h"

using namespace state;

static Cards::RawCard GetHero(PlayerIdentifier player)
{
	Cards::RawCard raw_card;
	raw_card.card_id = 8;
	raw_card.card_type = kCardTypeHero;
	raw_card.enchanted_states.zone = kCardZonePlay;
	raw_card.enchanted_states.max_hp = 30;
	raw_card.enchanted_states.player = player;
	return raw_card;
}

static void CheckZoneAndPosition(const state::State & state, CardRef ref, PlayerIdentifier player, CardZone zone, int pos)
{
	auto & item = state.mgr.Get(ref);
	assert(item.GetPlayerIdentifier() == player);
	assert(item.GetZone() == zone);
	assert(item.GetZonePosition() == pos);
}

struct Enchantment1
{
	static constexpr EnchantmentTiers tier = kEnchantmentTier1;

	Cards::Enchantments::ApplyFunctor apply_functor;
};
struct Enchantment2
{
	static constexpr EnchantmentTiers tier = kEnchantmentTier2;

	Cards::Enchantments::ApplyFunctor apply_functor;
};

static void test1()
{
	using namespace FlowControl;
	state::State state;
	FlowContext flow_context;

	Cards::RawCard c1;
	c1.card_type = kCardTypeMinion;
	c1.card_id = 1;
	c1.enchanted_states.player = kPlayerFirst;
	c1.enchanted_states.zone = kCardZoneDeck;
	c1.enchanted_states.cost = 5;
	c1.zone_position = 0;
	CardRef r1 = state.mgr.PushBack(state, flow_context, Cards::Card(c1));
	CheckZoneAndPosition(state, r1, kPlayerFirst, kCardZoneDeck, 0);

	c1.enchanted_states.zone = kCardZoneGraveyard;
	CheckZoneAndPosition(state, r1, kPlayerFirst, kCardZoneDeck, 0);

	Manipulate(state, flow_context).Minion(r1).Zone().ChangeTo<kCardZoneHand>(kPlayerFirst);
	CheckZoneAndPosition(state, r1, kPlayerFirst, kCardZoneHand, 0);

	auto state2 = state;
	CheckZoneAndPosition(state, r1, kPlayerFirst, kCardZoneHand, 0);
	CheckZoneAndPosition(state2, r1, kPlayerFirst, kCardZoneHand, 0);

	Manipulate(state2, flow_context).Minion(r1).Zone().ChangeTo<kCardZonePlay>(kPlayerFirst, 0);
	CheckZoneAndPosition(state, r1, kPlayerFirst, kCardZoneHand, 0);
	CheckZoneAndPosition(state2, r1, kPlayerFirst, kCardZonePlay, 0);

	Manipulate(state2, flow_context).Card(r1).Cost(9);
	assert(state.mgr.Get(r1).GetCost() == 5);
	assert(state2.mgr.Get(r1).GetCost() == 9);

	CardRef r2 = state2.mgr.PushBack(state2, flow_context, Cards::Card(c1));
	CheckZoneAndPosition(state2, r2, kPlayerFirst, kCardZoneGraveyard, 0);
	Manipulate(state2, flow_context).Minion(r2).Zone().ChangeTo<kCardZonePlay>(kPlayerFirst, 0);
	CheckZoneAndPosition(state2, r2, kPlayerFirst, kCardZonePlay, 0);
	CheckZoneAndPosition(state2, r1, kPlayerFirst, kCardZonePlay, 1);

	CardRef r3 = state2.mgr.PushBack(state2, flow_context, Cards::Card(c1));
	Manipulate(state2, flow_context).Minion(r3).Zone().ChangeTo<kCardZonePlay>(kPlayerFirst, 2);
	CheckZoneAndPosition(state2, r2, kPlayerFirst, kCardZonePlay, 0);
	CheckZoneAndPosition(state2, r1, kPlayerFirst, kCardZonePlay, 1);
	CheckZoneAndPosition(state2, r3, kPlayerFirst, kCardZonePlay, 2);

	CardRef r4 = state2.mgr.PushBack(state2, flow_context, Cards::Card(c1));
	Manipulate(state2, flow_context).Minion(r4).Zone().ChangeTo<kCardZonePlay>(kPlayerFirst, 1);
	CheckZoneAndPosition(state2, r2, kPlayerFirst, kCardZonePlay, 0);
	CheckZoneAndPosition(state2, r4, kPlayerFirst, kCardZonePlay, 1);
	CheckZoneAndPosition(state2, r1, kPlayerFirst, kCardZonePlay, 2);
	CheckZoneAndPosition(state2, r3, kPlayerFirst, kCardZonePlay, 3);

	// steal minion
	Manipulate(state2, flow_context).Minion(r1).Zone().ChangeTo<kCardZonePlay>(kPlayerSecond, 0);
	CheckZoneAndPosition(state2, r2, kPlayerFirst, kCardZonePlay, 0);
	CheckZoneAndPosition(state2, r4, kPlayerFirst, kCardZonePlay, 1);
	CheckZoneAndPosition(state2, r3, kPlayerFirst, kCardZonePlay, 2);
	CheckZoneAndPosition(state2, r1, kPlayerSecond, kCardZonePlay, 0);

	// steal minion
	Manipulate(state2, flow_context).Minion(r3).Zone().ChangeTo<kCardZonePlay>(kPlayerSecond, 0);
	CheckZoneAndPosition(state2, r2, kPlayerFirst, kCardZonePlay, 0);
	CheckZoneAndPosition(state2, r4, kPlayerFirst, kCardZonePlay, 1);
	CheckZoneAndPosition(state2, r3, kPlayerSecond, kCardZonePlay, 0);
	CheckZoneAndPosition(state2, r1, kPlayerSecond, kCardZonePlay, 1);

	// send to graveyard
	Manipulate(state2, flow_context).Minion(r1).Zone().ChangeTo<kCardZoneGraveyard>(kPlayerFirst);
	CheckZoneAndPosition(state2, r2, kPlayerFirst, kCardZonePlay, 0);
	CheckZoneAndPosition(state2, r4, kPlayerFirst, kCardZonePlay, 1);
	CheckZoneAndPosition(state2, r3, kPlayerSecond, kCardZonePlay, 0);
	CheckZoneAndPosition(state2, r1, kPlayerFirst, kCardZoneGraveyard, 0);

	// send to another player's graveyard
	Manipulate(state2, flow_context).Minion(r2).Zone().ChangeTo<kCardZoneGraveyard>(kPlayerFirst);
	CheckZoneAndPosition(state2, r4, kPlayerFirst, kCardZonePlay, 0);
	CheckZoneAndPosition(state2, r3, kPlayerSecond, kCardZonePlay, 0);
	CheckZoneAndPosition(state2, r1, kPlayerFirst, kCardZoneGraveyard, 0);
	CheckZoneAndPosition(state2, r2, kPlayerFirst, kCardZoneGraveyard, 1);

	Enchantment1 enchant1{ [](Cards::Card & card) {
		card.SetCost(card.GetCost() + 1);
	} };
	Enchantment2 enchant2{ [](Cards::Card & card) {
		card.SetCost(card.GetCost() * 2);
	} };

	auto manipulator = Manipulate(state, flow_context).Minion(r1);
	auto ref1 = manipulator.Enchant().Add(enchant1);
	auto ref2 = manipulator.Enchant().Add(enchant2);
	auto ref3 = manipulator.Enchant().Add(enchant1);
	auto ref4 = manipulator.Enchant().Add(enchant2);

	auto state3 = state;

	Manipulate(state3, flow_context).Minion(r1).Enchant().Remove<Enchantment2>(ref2);
}

class AuraHelper
{
public:
	static std::unordered_set<state::CardRef> GetEligibles(state::State & state, state::CardRef card_ref, const state::Cards::Card & card)
	{
		return{ card_ref };
	}

	static Cards::Enchantments::ApplyFunctor CreateEnchantmentFor(state::State & state, state::CardRef target)
	{
		return [](Cards::Card & card) {
			card.SetCost(card.GetCost() - 1);
		};
	}
};

static void test2()
{
	using namespace FlowControl;
	FlowContext flow_context;

	state::State state;

	Cards::RawCard c1;
	c1.card_type = kCardTypeMinion;
	c1.card_id = 1;
	c1.enchanted_states.zone = kCardZoneDeck;
	c1.enchanted_states.cost = 5;
	c1.zone_position = 0;
	CardRef r1 = state.mgr.PushBack(state, flow_context, Cards::Card(c1));

	Cards::RawCard c2;
	c2.card_type = kCardTypeMinion;
	c2.card_id = 2;
	c2.enchanted_states.zone = kCardZoneDeck;
	c2.enchanted_states.cost = 5;
	c2.zone_position = 0;
	CardRef r2 = state.mgr.PushBack(state, flow_context, Cards::Card(c2));

	Cards::RawCard c3;
	c3.card_type = kCardTypeMinion;
	c3.card_id = 3;
	c3.enchanted_states.zone = kCardZoneDeck;
	c3.enchanted_states.cost = 9;
	c3.zone_position = 0;
	CardRef r3 = state.mgr.PushBack(state, flow_context, Cards::Card(c3));

	typedef AuraHelper ClientAuraHelper;
	Manipulate(state, flow_context).Minion(r3).Aura().Update<ClientAuraHelper>();
	Manipulate(state, flow_context).Minion(r3).Aura().Update<ClientAuraHelper>();

	auto state2 = state;

	Manipulate(state2, flow_context).Minion(r3).Aura().Update<ClientAuraHelper>();
}

static void test3()
{
	FlowControl::FlowContext flow_context;

	Cards::Card hero1(GetHero(kPlayerFirst));
	Cards::Card hero2(GetHero(kPlayerSecond));

	state::State state;
	
	CardRef r1 = state.mgr.PushBack(state, flow_context, hero1);
	CardRef r2 = state.mgr.PushBack(state, flow_context, hero2);
}

int main()
{
	test1();
	test2();
	test3();

	return 0;
}