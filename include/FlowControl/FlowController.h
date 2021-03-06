#pragma once

#include "state/State.h"
#include "state/targetor/Targets-impl.h"
#include "state/detail/InvokeCallback-impl.h"

#include "FlowControl/FlowContext.h"
#include "FlowControl/FlowContext-impl.h"
#include "FlowControl/Manipulate.h"

#include "FlowControl/aura/Handler-impl.h"
#include "FlowControl/onplay/Handler-impl.h"
#include "FlowControl/enchantment/Handler-impl.h"
#include "FlowControl/Manipulators/PlayerManipulator-impl.h"
#include "FlowControl/Manipulators/HeroManipulator-impl.h"
#include "FlowControl/Manipulators/HeroPowerManipulator-impl.h"
#include "FlowControl/Manipulators/BoardManipulator-impl.h"
#include "FlowControl/Manipulators/CardManipulator-impl.h"
#include "FlowControl/Manipulators/OnBoardMinionManipulator-impl.h"
#include "FlowControl/Manipulators/MinionManipulator-impl.h"
#include "FlowControl/Manipulators/SecretManipulator-impl.h"
#include "FlowControl/Manipulators/Helpers/EnchantmentHelper-impl.h"
#include "FlowControl/Manipulators/Helpers/DamageHelper-impl.h"

#include "FlowControl/detail/Resolver.h"

namespace FlowControl
{
	class FlowController
	{
	public:
		FlowController(state::State & state, FlowContext & flow_context)
			: state_(state), flow_context_(flow_context)
		{
		}

	public: // main flow
		Result PlayCard(int hand_idx);
		Result Attack(state::CardRef attacker);
		Result HeroPower();
		Result EndTurn();

	private:
		state::CardRef GetDefender(state::CardRef attacker);

	private:
		Result Resolve();

		void PlayCardInternal(state::CardRef card_ref);
		void CostCrystal(int amount);
		void CostHealth(int amount);

		template <state::CardType> bool PlayCardPhase(state::CardRef card_ref);
		template <state::CardType> bool PlayCardPhaseInternal(state::CardRef card_ref);

		bool PlayMinionCardPhase(state::CardRef card_ref);
		bool PlayWeaponCardPhase(state::CardRef card_ref);
		bool PlayHeroPowerCardPhase(state::CardRef card_ref);
		bool PlaySpellCardPhase(state::CardRef card_ref);
		
		void AttackInternal(state::CardRef attacker);
		bool AttackPhase(state::CardRef attacker);

		void EndTurnInternal();
		void EndTurnPhase();
		void StartTurnPhase();
		void DrawCardPhase();

		void HeroPowerInternal();
		bool HeroPowerPhase();
		int GetMaxHeroPowerUseThisTurn();

		bool SetResult(Result result);
		bool SetInvalid() {
			return SetResult(Result::kResultInvalid);
		}

	private:
		state::State & state_;
		FlowContext & flow_context_;
	};
}