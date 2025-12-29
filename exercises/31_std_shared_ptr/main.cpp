#include "../exercise.h"
#include <memory>

#include <iostream>

// READ: `std::shared_ptr` <https://zh.cppreference.com/w/cpp/memory/shared_ptr>
// READ: `std::weak_ptr` <https://zh.cppreference.com/w/cpp/memory/weak_ptr>

// TODO: 将下列 `?` 替换为正确的值
int main(int argc, char **argv) {
    auto shared = std::make_shared<int>(10);
    std::shared_ptr<int> ptrs[]{shared, shared, shared};

    std::weak_ptr<int> observer = shared;
    std::cerr << "step0 " << observer.use_count() << "\n";
    ASSERT(observer.use_count() == 4, "");

    ptrs[0].reset();
    std::cerr << "step1 " << observer.use_count() << "\n";
    ASSERT(observer.use_count() == 3, "");

    ptrs[1] = nullptr;
    std::cerr << "step2 " << observer.use_count() << "\n";
    ASSERT(observer.use_count() == 2, "");

    ptrs[2] = std::make_shared<int>(*shared);
    std::cerr << "step3 " << observer.use_count() << "\n";
    ASSERT(observer.use_count() == 1, "");

    ptrs[0] = shared;
    ptrs[1] = shared;
    ptrs[2] = std::move(shared);
    std::cerr << "step4 " << observer.use_count() << "\n";
    ASSERT(observer.use_count() == 3, "");

    std::ignore = std::move(ptrs[0]);
    ptrs[1] = std::move(ptrs[1]);
    ptrs[1] = std::move(ptrs[2]);
    std::cerr << "step5 " << observer.use_count() << "\n";
    ASSERT(observer.use_count() == 1, "");

    shared = observer.lock();
    std::cerr << "step6 " << observer.use_count() << "\n";
    ASSERT(observer.use_count() == 2, "");

    shared = nullptr;
    for (auto &ptr : ptrs) ptr = nullptr;
    std::cerr << "step7 " << observer.use_count() << "\n";
    ASSERT(observer.use_count() == 0, "");

    shared = observer.lock();
    std::cerr << "step8 " << observer.use_count() << "\n";
    ASSERT(observer.use_count() == 0, "");

    return 0;
}
