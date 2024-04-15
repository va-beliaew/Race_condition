#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <random>
#include <sstream>

using namespace std::chrono_literals;

int n = 5;
std::atomic<int> stop(0);
std::atomic<int> num_of_thr(0);
std::atomic<double*> data(new double [n]{0});
std::atomic<size_t*> id(new size_t[n]{0});
std::atomic<int*> progr(new int[n]{0});

void process(int n) {
	
	num_of_thr.store(num_of_thr.load() + 1);
	int num = num_of_thr.load();
	std::stringstream s;
	s << std::this_thread::get_id();
	s >> id[num - 1];
	auto start = std::chrono::steady_clock::now();
	for (int i = 0; i < 20; ++i) {
		std::this_thread::sleep_for(std::chrono::milliseconds(n));
		progr[num - 1] = i;
	}
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> dif = end - start;
	data[num - 1] = dif.count();
	stop.store(stop.load() + 1);
}

std::string line(int n) {
	std::string ch;
	for (int i = 0; i < 20; ++i) {
		ch.push_back(' ');
	}
	for (int i = 0; i < n; ++i) {
		ch[i] = 219;
	}
	return ch;
}

void print() {
	while (1) {
		system("cls");
		std::cout << "N  " << "id \t" << "load\t\t\t" << "time\n\n";
		for (int i = 0; i < num_of_thr; ++i) {
			std::cout << i + 1 << " " << id[i] << "\t" << line(progr[i]) << "\t" << data[i] << std::endl;
			
		}
		if (n != stop.load()) {
			std::this_thread::sleep_for(300ms);
			
		}
		else {
			break;
		}
	}
}

int main() {
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(200, 600);
	std::thread t1(print);
	
	std::thread* th = new std::thread[n];
	for (int i = 0; i < n; ++i) {
		th[i] = std::thread(process, (dist(rng)));
	}
	for (int i = 0; i < n; ++i) {
		th[i].join();
	}
	t1.join();

	return 0;
	}