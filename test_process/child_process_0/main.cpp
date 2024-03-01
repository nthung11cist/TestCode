#include <signal.h>
#include <setjmp.h>
#include <chrono>
#include <iostream>
#include <atomic>
#include <unistd.h>
#include "TestIPC.hpp"

using namespace std;
using namespace chrono;
using namespace TestIPC;

struct sigaction sigIntHandler;
volatile std::atomic<bool> program_running;
void signalHandler(int signal)
{
	std::cout << "Child process 0, PID: " << getpid() << ", signal: " << signal << std::endl;
	program_running = false;
}

int main(int argc, char **argv)
{
	program_running = true;
	sigIntHandler.sa_handler = signalHandler;
	sigfillset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);
	// sigaction(SIGSEGV, &sigIntHandler, NULL);
	sigaction(SIGILL, &sigIntHandler, NULL);
	sigaction(SIGFPE, &sigIntHandler, NULL);
	sigaction(SIGABRT, &sigIntHandler, NULL);
	sigaction(SIGFPE, &sigIntHandler, NULL);
	sigaction(SIGTERM, &sigIntHandler, NULL);
	sigaction(SIGKILL, &sigIntHandler, NULL);

	std::unique_ptr<TestShareMem> mTestShareMem = std::make_unique<TestShareMem>();
	std::unique_ptr<TestMsgQueue> mTestMsgQueue = std::make_unique<TestMsgQueue>();

	while (program_running == true)
	{
		std::cout << "--------------Child process 0, PID: " << getpid() << std::endl;

		// mTestShareMem->ReadInt();
		// mTestShareMem->ReadString();
		mTestShareMem->ReadVector();

		// mTestMsgQueue->ReceiveMsg();
		sleep(3);
	}

	return 0;
}
