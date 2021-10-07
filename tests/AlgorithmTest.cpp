
#include "Util/Algorithm.h"
#include <gtest/gtest.h>

TEST(Algorithm, sort_permutation)
{
    int life[] = { 1, 2, 0, 5, 4, 3 };
    std::vector<const char*> names = { "zero", "one", "two", "three", "four", "five" };

    const auto sort_func = [](int left, int right) {
        return left < right;
    };

    const std::vector<size_t> permutation = mono::sort_permutation(std::begin(life), std::begin(life) + 3, sort_func);
    const std::vector<const char*> sorted_names = mono::apply_permutation(permutation, names);

    for(const char* name : sorted_names)
        printf("%s\n", name);
}

TEST(Algorithm, apply_permutation_in_place_variadic)
{
    int values[] = { 1, 2, 0, 5, 4, 3 };

    const auto sort_func = [](int left, int right) {
        return left < right;
    };

    const std::vector<size_t> permutation = mono::sort_permutation(std::begin(values), std::begin(values), sort_func);

    std::vector<const char*> names = { "zero", "one", "two", "three", "four", "five" };
    std::vector<int> numbers = { 0, 1, 2, 3, 4, 5 };
    std::vector<float> floats = { 0.0f, 1.1f, 2.2f, 3.3f, 4.4f, 5.5f };

    mono::apply_permutation_in_place(permutation, names, numbers, floats);

//    for(size_t index = 0; index < permutation.size(); ++index)
//        printf("%s %u %f\n", names[index], numbers[index], floats[index]);
}
