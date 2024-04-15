#include <mutex>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;


class Data {
	int a = 0;
	double b = 0;
	std::string str;
public:
	std::mutex m;
	Data() {};
	Data(int a_, double b_, std::string str_) : a(a_), b(b_), str(str_) {};
	Data& operator= (const Data& data) {
		this->a = data.a;
		this->b = data.b;
		this->str = data.str;
		return *this;
	}
	void print() const {
		std::cout << a << '\t' << b << '\t' << str << std::endl;
	}
};


void swap_lock(Data& d1, Data& d2) {
	Data tmp;
	d1.m.lock();
	d2.m.lock();
	tmp = d1;
	d1 = d2;
	d2 = tmp;
	d1.m.unlock();
	d2.m.unlock();
	
}

void swap_scoped(Data& d1, Data& d2) {
	Data tmp;
	std::scoped_lock sc (d1.m, d2.m);
	tmp = d1;
	d1 = d2;
	d2 = tmp;
	
}

void swap_unique(Data& d1, Data& d2) {
	Data tmp;
	std::unique_lock<std::mutex> ul1(d1.m );
	std::unique_lock<std::mutex> ul2 (d2.m );
	tmp = d1;
	d1 = d2;
	ul1.unlock();
	d2 = tmp;

}

int main() {
	Data d1{ 1, 2, "First" };
	Data d2{ 10, 20, "Second" };
	d1.print();
	d2.print();
	std::thread t( swap_lock, std::ref(d1), std::ref(d2) );
	//std::thread t(swap_scoped, std::ref(d1), std::ref(d2));
	//std::thread t(swap_unique, std::ref(d1), std::ref(d2));
	std::this_thread::sleep_for(2ms);
	d1.print();
	d2.print();
	t.join();
	return 0;
}