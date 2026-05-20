#include <iostream>
#include <type_traits>

//std::true_type — тип, представляющий true (содержит статическое value = true).
//std::false_type — тип, представляющий false (содержит статическое value = false).

namespace tl {
    // Базовая структура
    template <typename... Types>
    struct TypeList {
        static constexpr size_t size() {
            return sizeof...(Types);
        }
    };

    template <size_t Index, typename... Types>
    struct TypeAtHelper;

    template <typename T, typename... Rest>
    struct TypeAtHelper<0, T, Rest...> {
        using type = T;
    };

    template <size_t Index, typename T, typename... Rest>
    struct TypeAtHelper<Index, T, Rest...> {
        using type = typename TypeAtHelper<Index - 1, Rest...>::type;
    };

    template <size_t Index, typename TList>
    struct TypeAt;

    template <size_t Index, typename... Types>
    struct TypeAt<Index, TypeList<Types...>> {
        using type = typename TypeAtHelper<Index, Types...>::type;
    };
  
    template <typename T, typename TList>
    struct Contains;

    // Пустой список
    template <typename T>
    struct Contains<T, TypeList<>> : std::false_type {};

    // Первый тип совпадает с искомым
    template <typename T, typename... Rest>
    struct Contains<T, TypeList<T, Rest...>> : std::true_type {};

    // Первый тип не совпадает - рекурсия
    template <typename T, typename T1, typename... Rest>
    struct Contains<T, TypeList<T1, Rest...>> : Contains<T, TypeList<Rest...>> {};

    template <typename T, typename TList>
    struct IndexOf;

    // Базовый случай - тип не найден
    template <typename T>
    struct IndexOf<T, TypeList<>> {
        static_assert(sizeof(T) == 0, "Type not found in TypeList");
        static constexpr size_t value = 0;
    };

    // Первый тип совпадает
    template <typename T, typename... Rest>
    struct IndexOf<T, TypeList<T, Rest...>> {
        static constexpr size_t value = 0;
    };

    // Первый тип не совпадает - рекурсия
    template <typename T, typename T1, typename... Rest>
    struct IndexOf<T, TypeList<T1, Rest...>> {
        static constexpr size_t value = 1 + IndexOf<T, TypeList<Rest...>>::value;
    };

    // Добавление в конец
    template <typename T, typename TList>
    struct PushBack;

    template <typename T, typename... Types>
    struct PushBack<T, TypeList<Types...>> {
        using type = TypeList<Types..., T>;
    };

    // Добавление в начало
    template <typename T, typename TList>
    struct PushFront;

    template <typename T, typename... Types>
    struct PushFront<T, TypeList<Types...>> {
        using type = TypeList<T, Types...>;
    };
}

int main() {
    using myList = tl::TypeList<int, float, char, double>;

    // 1. Тест получения размера
    static_assert(myList::size() == 4, "Size test failed");

    // 2. Тест TypeAt
    static_assert(std::is_same_v<tl::TypeAt<0, myList>::type, int>, "TypeAt<0> failed");
    static_assert(std::is_same_v<tl::TypeAt<1, myList>::type, float>, "TypeAt<1> failed");
    static_assert(std::is_same_v<tl::TypeAt<2, myList>::type, char>, "TypeAt<2> failed");
    static_assert(std::is_same_v<tl::TypeAt<3, myList>::type, double>, "TypeAt<3> failed");

    // 3. Тест Contains
    static_assert(tl::Contains<int, myList>::value, "Contains<int> failed");
    static_assert(tl::Contains<float, myList>::value, "Contains<float> failed");
    static_assert(tl::Contains<char, myList>::value, "Contains<char> failed");
    static_assert(tl::Contains<double, myList>::value, "Contains<double> failed");
    static_assert(!tl::Contains<long, myList>::value, "Contains<long> should be false");

    // 4. Тест IndexOf
    static_assert(tl::IndexOf<int, myList>::value == 0, "IndexOf<int> failed");
    static_assert(tl::IndexOf<float, myList>::value == 1, "IndexOf<float> failed");
    static_assert(tl::IndexOf<char, myList>::value == 2, "IndexOf<char> failed");
    static_assert(tl::IndexOf<double, myList>::value == 3, "IndexOf<double> failed");

    // 5. Тест PushBack
    using WithString = tl::PushBack<std::string, myList>::type;
    static_assert(WithString::size() == 5, "PushBack size failed");
    static_assert(std::is_same_v<tl::TypeAt<4, WithString>::type, std::string>, "PushBack type failed");

    // 6. Тест PushFront
    using WithBool = tl::PushFront<bool, myList>::type;
    static_assert(WithBool::size() == 5, "PushFront size failed");
    static_assert(std::is_same_v<tl::TypeAt<0, WithBool>::type, bool>, "PushFront type failed");
    static_assert(std::is_same_v<tl::TypeAt<1, WithBool>::type, int>, "PushFront shift failed");

    // 7. Тест на ошибку компиляции
    // using invalid = tl::TypeAt<10, myList>::type; // Ошибка компиляции

    std::cout << "OK!" << std::endl;
    return 0;
}