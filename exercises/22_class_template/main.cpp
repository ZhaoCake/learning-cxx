#include "../exercise.h"
#include <cstring>
// READ: 类模板 <https://zh.cppreference.com/w/cpp/language/class_template>

template<class T>
struct Tensor4D {
    unsigned int shape[4];
    T *data;

    Tensor4D(unsigned int const shape_[4], T const *data_) {
        unsigned int size = 1;
        // TODO: 填入正确的 shape 并计算 size
        for (int i = 0; i < 4; ++i) {
            shape[i] = shape_[i];
            size *= shape[i];
        }
        data = new T[size];
        std::memcpy(data, data_, size * sizeof(T));
    }
    ~Tensor4D() {
        delete[] data;
    }

    // 为了保持简单，禁止复制和移动
    Tensor4D(Tensor4D const &) = delete;
    Tensor4D(Tensor4D &&) noexcept = delete;

    // 这个加法需要支持“单向广播”。
    // 具体来说，`others` 可以具有与 `this` 不同的形状，形状不同的维度长度必须为 1。
    // `others` 长度为 1 但 `this` 长度不为 1 的维度将发生广播计算。
    // 例如，`this` 形状为 `[1, 2, 3, 4]`，`others` 形状为 `[1, 2, 1, 4]`，
    // 则 `this` 与 `others` 相加时，3 个形状为 `[1, 2, 1, 4]` 的子张量各自与 `others` 对应项相加。
    Tensor4D &operator+=(Tensor4D const &others) {
        // TODO: 实现单向广播的加法
        // 规则：对每个维度 d：others.shape[d] 必须等于 shape[d] 或者为 1。
        // 若为 1，则该维度发生广播：others 在该维度的索引恒为 0。
        // 广播不改变内存，因为不是存储内容变了，而是索引思路变了。

        for (int d = 0; d < 4; ++d) {
            ASSERT(others.shape[d] == 1 || others.shape[d] == shape[d],
                   "Shape mismatch: others.shape[d] must be 1 or equal to this.shape[d]");
        }

        const unsigned int this_strides[4]{
            shape[1] * shape[2] * shape[3],
            shape[2] * shape[3],
            shape[3],
            1,
        };
        const unsigned int other_strides[4]{
            others.shape[1] * others.shape[2] * others.shape[3],
            others.shape[2] * others.shape[3],
            others.shape[3],
            1,
        };
        const unsigned int this_size = shape[0] * shape[1] * shape[2] * shape[3];

        for (unsigned int offset = 0; offset < this_size; ++offset) {
            const unsigned int i0 = offset / this_strides[0];
            const unsigned int r0 = offset % this_strides[0];
            const unsigned int i1 = r0 / this_strides[1];
            const unsigned int r1 = r0 % this_strides[1];
            const unsigned int i2 = r1 / this_strides[2];
            const unsigned int i3 = r1 % this_strides[2];

            const unsigned int o0 = (others.shape[0] == 1) ? 0u : i0;
            const unsigned int o1 = (others.shape[1] == 1) ? 0u : i1;
            const unsigned int o2 = (others.shape[2] == 1) ? 0u : i2;
            const unsigned int o3 = (others.shape[3] == 1) ? 0u : i3;
            const unsigned int other_offset = o0 * other_strides[0] + o1 * other_strides[1] +
                                              o2 * other_strides[2] + o3 * other_strides[3];

            data[offset] += others.data[other_offset];
        }
        return *this;
    }
};

// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    {
        unsigned int shape[]{1, 2, 3, 4};
        // clang-format off
        int data[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        auto t0 = Tensor4D(shape, data);
        auto t1 = Tensor4D(shape, data);
        t0 += t1;
        for (auto i = 0u; i < sizeof(data) / sizeof(*data); ++i) {
            ASSERT(t0.data[i] == data[i] * 2, "Tensor doubled by plus its self.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        float d0[]{
            1, 1, 1, 1,
            2, 2, 2, 2,
            3, 3, 3, 3,

            4, 4, 4, 4,
            5, 5, 5, 5,
            6, 6, 6, 6};
        // clang-format on
        unsigned int s1[]{1, 2, 3, 1};
        // clang-format off
        float d1[]{
            6,
            5,
            4,

            3,
            2,
            1};
        // clang-format on

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == 7.f, "Every element of t0 should be 7 after adding t1 to it.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        double d0[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        unsigned int s1[]{1, 1, 1, 1};
        double d1[]{1};

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == d0[i] + 1, "Every element of t0 should be incremented by 1 after adding t1 to it.");
        }
    }
}
