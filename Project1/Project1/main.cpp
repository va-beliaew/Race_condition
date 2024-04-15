#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>



using namespace std::chrono_literals;


void queue(const char* type, std::atomic<int>& man, const int limit_man, std::atomic<int>& current_man) {
	if (type == "client") {
		while (limit_man > man.load()) {
			std::this_thread::sleep_for(std::chrono::seconds(1));
			man.store(man.load() + 1);
			current_man.store(current_man.load() + 1);
			std::cout << "\nclient_queue " << current_man.load() << std::endl;
			std::cout << "Total_comin: " << man.load();
		}
		std::cout << "\nlimit!\n";
	}
	else if (type == "cashier") {
		int to_do = 0;
		while (to_do <= limit_man) {
			if (current_man.load() >= 0) {
				std::this_thread::sleep_for(std::chrono::seconds(2));
				current_man.store(current_man.load() - 1);
				std::cout << "\ntotal_opered " << to_do << std::endl;
				++to_do;
			}
		}
	}
}

int main() {
	std::atomic<int> man = 0;
	std::atomic<int> current_man = 0;
	const int limit_man = 50;
	std::thread t1(queue, "client", std::ref(man), limit_man, std::ref(current_man));
	std::thread t2(queue, "cashier", std::ref(man), limit_man, std::ref(current_man));
	t1.join();
	t2.join();
	std::cout << "\nend";

	return 0;
}