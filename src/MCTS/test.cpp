
#include <chrono>
#include <iostream>
#include <sstream>

#include "FlowControl/FlowController-impl.h"
#include "Cards/PreIndexedCards.h"
#include "MCTS/TestStateBuilder.h"
#include "UI/AIController.h"
#include "UI/CompetitionGuide.h"
#include "UI/InteractiveShell.h"

static void Initialize()
{
	std::cout << "Reading json file...";
	if (!Cards::Database::GetInstance().Initialize("cards.json")) assert(false);
	Cards::PreIndexedCards::GetInstance().Initialize();
	std::cout << " Done." << std::endl;
}

struct Configs
{
	static int threads;
};

int Configs::threads = 0;

void Run(ui::AIController * controller, int secs)
{
	auto & s = std::cout;

	auto start_board_getter = [](int seed) -> state::State {
		return TestStateBuilder().GetState(seed);
	};

	auto seed = std::random_device()();

	s << "Running for " << secs << " seconds with " << Configs::threads << " threads "
		<< "(seed = " << seed << ")" << std::endl;

	auto start = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point run_until =
		std::chrono::steady_clock::now() +
		std::chrono::seconds(secs);

	long long last_show_rest_sec = -1;
	auto continue_checker = [&]() {
		auto now = std::chrono::steady_clock::now();
		if (now > run_until) return false;

		auto rest_sec = std::chrono::duration_cast<std::chrono::seconds>(run_until - now).count();
		if (rest_sec != last_show_rest_sec) {
			s << "Rest seconds: " << rest_sec << std::endl;
			last_show_rest_sec = rest_sec;
		}
		return true;
	};

	auto start_i = controller->GetStatistic().GetSuccededIterates();
	controller->Run(continue_checker, Configs::threads, seed, start_board_getter);
	auto end_i = controller->GetStatistic().GetSuccededIterates();

	s << std::endl;
	s << "Done iterations: " << (end_i - start_i) << std::endl;
	s << "====== Statistics =====" << std::endl;
	controller->GetStatistic().GetDebugMessage();

	auto now = std::chrono::steady_clock::now();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
	auto speed = (double)(end_i - start_i) / ms * 1000;
	s << "Iterations per second: " << speed << std::endl;
	s << std::endl;
}

bool CheckRun(std::string const& cmdline, ui::AIController * controller)
{
	std::stringstream ss(cmdline);

	std::string cmd;
	ss >> cmd;

	if (cmd == "t" || cmd == "threads") {
		ss >> Configs::threads;
		return true;
	}

	if (cmd == "s" || cmd == "start") {
		int secs = 0;
		ss >> secs;
		Run(controller, secs);
		return true;
	}
	return false;
}

void TestAI()
{
	Configs::threads = 1;

	srand(0);

	int tree_samples = 10000;
	std::mt19937 rand;

	ui::AIController controller(tree_samples, rand);
	ui::InteractiveShell handler(&controller);

	while (std::cin) {
		std::string cmdline;
		std::cout << "Command: ";
		std::getline(std::cin, cmdline);

		if (cmdline == "q" || cmdline == "quit") {
			std::cout << "Good bye!" << std::endl;
			break;
		}

		if (CheckRun(cmdline, &controller)) {
			continue;
		}

		std::istringstream iss(cmdline);
		handler.DoCommand(iss, std::cout);
	}
}

int main(int argc, char *argv[])
{
	Initialize();
	TestAI();
	return 0;
}
