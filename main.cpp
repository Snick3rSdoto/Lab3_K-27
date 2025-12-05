#include <iostream>
#include <vector>
#include <thread>
#include <barrier>
#include <syncstream>
#include <chrono>

void f(char group, int id) {
    std::osyncstream(std::cout) << "З набору " << group
                                << " виконано дію " << id << "."
                                << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void worker(int thread_idx, std::barrier<>& sync_point) {

    if (thread_idx == 0) {
        f('a', 1); f('a', 2); f('a', 3); f('a', 4);
        f('b', 1);
    }
    else if (thread_idx == 1) {
        f('b', 2); f('b', 3); f('b', 4); f('b', 5); f('b', 6);
    }
    else if (thread_idx == 2) {
        f('b', 7); f('b', 8); f('c', 1); f('c', 2);
    }
    else if (thread_idx == 3) {
        f('c', 3); f('c', 4); f('c', 5); f('c', 6);
    }

    sync_point.arrive_and_wait();
	
    if (thread_idx == 0) {
        f('d', 1); f('d', 2); f('d', 3); f('d', 4); f('d', 5);
    }
    else if (thread_idx == 1) {
        f('d', 6); f('e', 1); f('e', 2); f('e', 3); f('e', 4);
    }
    else if (thread_idx == 2) {
        f('e', 5); f('e', 6); f('e', 7); f('f', 1); f('f', 2);
    }
    else if (thread_idx == 3) {
        f('f', 3); f('f', 4); f('f', 5); f('f', 6); f('f', 7);
    }

    sync_point.arrive_and_wait();

    if (thread_idx == 0) {
        f('g', 1); f('g', 2); f('g', 3); f('g', 4); f('h', 1);
    }
    else if (thread_idx == 1) {
        f('h', 2); f('h', 3); f('h', 4); f('h', 5); f('h', 6);
    }
    else if (thread_idx == 2) {
        f('h', 7); f('i', 1); f('i', 2); f('i', 3);
    }
    else if (thread_idx == 3) {
        f('g', 5); f('g', 6); f('i', 4); f('i', 5);
    }

    sync_point.arrive_and_wait();

    if (thread_idx == 0) {
        f('j', 1); f('j', 2); f('j', 3); f('j', 4);
    }
    else if (thread_idx == 1) {
        f('j', 5); f('j', 6); f('j', 7);
    }
    else if (thread_idx == 2) {
        f('j', 8); f('j', 9); f('k', 1);
    }
    else if (thread_idx == 3) {
        f('k', 2); f('k', 3); f('k', 4);
    }

}

int main() {
    std::cout << "Обчислення розпочато." << std::endl;

    const int nt = 4;
    std::barrier sync_point(nt);

    std::vector<std::jthread> threads;
    threads.reserve(nt);

    for (int i = 0; i < nt; ++i) {
        threads.emplace_back(worker, i, std::ref(sync_point));
    }

    // jthread автоматично join-иться в деструкторі,
    // clear() запустить деструктори елементів вектора
    threads.clear();

    std::cout << "Обчислення завершено." << std::endl;

    return 0;
}

