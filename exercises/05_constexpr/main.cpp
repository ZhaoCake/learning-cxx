#include "../exercise.h"

constexpr unsigned long long fibonacci(int i) {
    switch (i) {
        case 0:
            return 0;
        case 1:
            return 1;
        default:
            return fibonacci(i - 1) + fibonacci(i - 2);
    }
}

int main(int argc, char **argv) {
    constexpr auto FIB20 = fibonacci(20);
    ASSERT(FIB20 == 6765, "fibonacci(20) should be 6765");
    std::cout << "fibonacci(20) = " << FIB20 << std::endl;

    // TODO: 观察错误信息，修改一处，使代码编译运行
    // PS: 编译运行，但是不一定能算出结果……
    constexpr auto ANS_N = 30;
    auto ANS = fibonacci(ANS_N); // 这个递归太大，编译器算不出来。所以改成非编译器白能量就可以了。
    // 那么我这里还有一个问题。这个fibonacci函数携程constexpr的作用是什么呢？
    // 这个是允许其在编译期被计算。但并非是强制的。可以用于编译期常量初始化。不满足编译期条件时，会退化为运行期调用。
    std::cout << "fibonacci(" << ANS_N << ") = " << ANS << std::endl;

    return 0;
}
