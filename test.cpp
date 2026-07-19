// 6/10/2023
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#define ANKERL_NANOBENCH_IMPLEMENT
#include "nanobench.h"
#include "fakeit.hpp"
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <set>
#include <limits>
#include <thread>
#include <variant>
#include "nlohmann/json.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <bitset>
#include <exception>

int fact(int n) {
    if (n == 0) {
        return 1;
    }
    return n <= 1 ? n : fact(n - 1) * n;
}

int fibonacci(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    int a = 0, b = 1;
    for (int i = 2; i <= n; ++i) {
        int temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

struct SomeInterface {
    virtual int foo(int) = 0;
    virtual int bar(std::string) = 0;
};

template<class T>
inline const T& max(const T& a, const T& b)
{
    return a < b ? b : a;
}

class Q {
public:
    typedef int SubType;
};

template <class T>
class MyClass {
private:
    T value;
public:
    // copy ctor with implicit type conversion
    template<class U>
    MyClass(const MyClass<U>& x) {
        value = x.getValue();
    }

    MyClass() {
    }

    template<class X>
    void assign(const MyClass<X>& x) {
        value = x.getValue();
    }

    T getValue() const {
        return value;
    }
};

enum class MyVisible {XTrue = 1, XFalse=2};

TEST_CASE("testing the factorial function") {
    CHECK(fact(0) == 1); // should fail
    CHECK(fact(1) == 1);
    CHECK(fact(2) == 2);
    CHECK(fact(3) == 6);
    CHECK(fact(10) == 3628800);
}

TEST_CASE("testing the fibonacci function") {
    CHECK(fibonacci(0) == 0);
    CHECK(fibonacci(1) == 1);
    CHECK(fibonacci(2) == 1);
    CHECK(fibonacci(3) == 2);
    CHECK(fibonacci(4) == 3);
    CHECK(fibonacci(5) == 5);
    CHECK(fibonacci(6) == 8);
    CHECK(fibonacci(7) == 13);
    CHECK(fibonacci(8) == 21);
    CHECK(fibonacci(9) == 34);
    CHECK(fibonacci(10) == 55);
    CHECK(fibonacci(20) == 6765);
}

TEST_CASE("bench") {
    double d = 1.0;
    ankerl::nanobench::Bench().run("some double ops", [&] {
            d += 1.0 / d;
            if (d > 5.0) {
            d -= 5.0;
            }
            ankerl::nanobench::doNotOptimizeAway(d);
            });
}

TEST_CASE("fake") {
    fakeit::Mock<SomeInterface> mock;
    fakeit::When(Method(mock,foo)).Return(1); // Method mock.foo will return 1 once.
    SomeInterface &i = mock.get();
    std::cout << i.foo(0);
    REQUIRE(i.foo(0) == 1);
}

int transmogrify(int x) {
    return 2 * x;
}

TEST_CASE("item30") {
    std::vector<int> values{1,2,3};
    std::vector<int> results;
    results.reserve(3);

    std::transform(values.begin(), values.end(),
            std::back_inserter(results), transmogrify);
    std::vector<int> out{2,4,6};
    REQUIRE(results == out);
    std::cout << results.size() << "\n";
}

TEST_CASE("reserve") {
    std::vector<int> values{1,2,3};
    std::vector<int> results;

    SUBCASE("back_inserter") {
        results.reserve(3);
        std::transform(values.begin(), values.end(),
                std::back_inserter(results), transmogrify);
    }

    SUBCASE("resize") {
        results.resize(3);
        std::transform(values.begin(), values.end(),
                results.begin(), transmogrify);
    }

    std::cout << "size: " << results.size() << "\n";
    for (auto& item: results) {
        std::cout << item << "\n";
    }
}

inline bool doublegreater(double d1, double d2) {
    return d1 > d2;
}

TEST_CASE("sort") {
    std::vector<double> v{4.1,2.3, 5.8};

    SUBCASE("greater") {
        std::sort(v.begin(), v.end(), std::greater<double>());
    }

    SUBCASE("doublegreater") {
        std::sort(v.begin(), v.end(), doublegreater);
    }

    for (auto& item: v) {
        std::cout << item << " ";
    }
    std::cout << "\n";
}

// struct StringSize:
//     public std::unary_function<std::string, std::string::size_type>
// {
//     std::string::size_type operator()(const std::string& s) const
//     {
//         return s.size();
//     }
// };

// template<typename FPType>
// FPType average(FPType val1, FPType val2) {
//     return (val1 + val2) / 2;
// }


// template<typename FPType>
// struct Average:
//     public std::binary_function<FPType, FPType, FPType> {
//     FPType operator()(FPType val1, FPType val2) const
//     {
//         return average(val1, val2);
//     }
// };

template<typename InputIter1,
         typename InputIter2>
void writeaverages(InputIter1 begin1,
                   InputIter1 end1,
                   InputIter2 begin2,
                   std::ostream& s)
{
    /* std::transform(begin1, end1, begin2, */
    /*         std::ostream_iterator<typename iterator_traits<InputIter1>::value_type>(s, "\n"), */
    /*         average<typename iterator_traits<InputIter1>::value_type>); */
    /* std::transform(begin1, end1, begin2, */
    /*         std::ostream_iterator<typename iterator_traits<InputIter1>::value_type>(s, "\n"), */
    /*         Average<typename iterator_traits<InputIter1::value_type>()); */
}

// TEST_CASE("mem_fun_ref") {
//     std::set<std::string> s{"abc", "ef"};

//     SUBCASE("mem_fun_ref") {
//         std::transform(s.begin(), s.end(),
//                 std::ostream_iterator<std::string::size_type>(std::cout, "\n"),
//                 std::mem_fun_ref(&std::string::size));
//     }

//     SUBCASE("unary_function") {
//         std::transform(s.begin(), s.end(),
//                 std::ostream_iterator<std::string::size_type>(std::cout, "\n"),
//                 StringSize());
//     }
// }

TEST_CASE("max") {
    int a = 3;
    int b = 4;
    int result = max(a, b);
    REQUIRE(result == 4);
}

void throw_logic_error()
{
    std::string err_msg("error");
    throw std::logic_error(err_msg);
}

TEST_CASE("logic_error") {
    REQUIRE_THROWS_AS(throw_logic_error(), std::logic_error);
}

TEST_CASE("MyClass") {
    MyClass<double> d;
    MyClass<int> i;

    d.assign(d);
    d.assign(i);

    MyClass<double> xd;
    MyClass<double> xd2(xd);
    MyClass<int> xi(xd);
}

TEST_CASE("explicit_init") {
    int i1;
    int i2 = int();
    CHECK(i1 != 0);
    CHECK(i2 == 0);
}

TEST_CASE("limits") {
    std::cout << std::boolalpha;

    std::cout << "max(short): " << std::numeric_limits<short>::max() <<
        std::endl;
    std::cout << "max(int): " << std::numeric_limits<int>::max() << std::endl;
    std::cout << "max(long): " << std::numeric_limits<long>::max() << std::endl;
    std::cout << "max(float): " << std::numeric_limits<float>::max() <<
        std::endl;
    std::cout << "max(double): " << std::numeric_limits<double>::max()
        << std::endl;
    std::cout << "max(long double): " << std::numeric_limits<long double>::max()
        << std::endl;
    std::cout << "is_signed(char): " << std::numeric_limits<char>::is_signed <<
        std::endl;
    std::cout << "is_specialized(string): "
        << std::numeric_limits<std::string>::is_specialized << std::endl;
}

bool int_ptr_less(int* a, int* b)
{
    return *a < *b;
}

TEST_CASE("max") {
    int x = 17;
    int y = 42;
    int* px = &x;
    int* py = &y;
    int *pmax;

    pmax = std::max(px, py, int_ptr_less);
    REQUIRE(*pmax == y);
}

TEST_CASE("max2") {
    int x = 3;
    long y = 4;

    int result = std::max<long>(x, y);
    REQUIRE(result == y);
}

TEST_CASE("enum") {
   MyVisible v = MyVisible::XTrue;

    std::cout << "v: " << (int)v << std::endl;
}

TEST_CASE("atomic") {
    int y = 0;
    std::atomic<int> x(0);
    ankerl::nanobench::Bench().run("compare_exchange_strong", [&] {
            x.compare_exchange_strong(y, 0);
            });
}

TEST_CASE("tutorial_fast_v2") {
    uint64_t x = 1;
    ankerl::nanobench::Bench().run("++x", [&]() {
        ankerl::nanobench::doNotOptimizeAway(x += 1);
    });
}

TEST_CASE("tutorial_slow_v1") {
    ankerl::nanobench::Bench().run("sleep 100ms, auto", [&] {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    });
}

TEST_CASE("tutorial_slow_v2") {
    ankerl::nanobench::Bench().epochs(3).run("sleep 100ms", [&] {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    });
}

constexpr double square(double x)
{
    return x * x;
}

TEST_CASE("init") {
    double d2{2.3};
    std::vector<int> v{1,2,3,4,5,6};
    auto b = true;
    auto bb{true};

    constexpr double max1 = 1.4 * square(17);
    printf("%f %d %d %f\n", d2, b, bb, max1);
}

void print()
{
    int v[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    for (auto& x: v)
        std::cout << x << "\n";

    for (auto& x: {10, 21, 32, 43, 54, 65})
        std::cout << x << "\n";
}

TEST_CASE("range") {
    print();
}

// A Tour of C++ (13)
int count_x(const char* p, char x)
{
    if (p == nullptr)
        return 0;

    int count = 0;
    while (*p) {
        if (*p == x)
            ++count;
        ++p;
    }
    return count;
}

TEST_CASE("nullptr") {
    double *pd = nullptr;

    char str[] = "hello world";
    auto count = count_x(str, 'l');
    REQUIRE(count == 3);
    printf("%p\n", pd);
}

TEST_CASE("var_in_if") {
    std::vector<int> v{1,2,3};

    if (auto n = v.size()) {
        std::cout << "n: " << n << "\n";
    }
}

TEST_CASE("variant1") {
    std::variant<int, float> v, w;
    v = 42;

    int i = std::get<int>(v);
    REQUIRE(i == 42);

    w = v;
    try {
        std::get<float>(w);
    } catch (const std::bad_variant_access& ex) {
        std::cout << ex.what() << "\n";
    }
}

enum  class Color {red, blue, green};

TEST_CASE("enum") {
    Color color = Color::red;
    printf("color %d\n", (int)color);
}

class A
{
public:
    A() {}
};

class B
{
public:
    explicit B(int x=0, bool b=true):x_(x), b_(b)
    {

    }

    int x() { return x_; }
private:
    int x_;
    int b_;
};

class C
{
public:
    explicit C(int x):x_(x)
    {

    }
private:
    int x_;
};

void do_something(B obj)
{
    printf("obj.x %d\n", obj.x());
}

TEST_CASE("explicit") {
    B bobj1;
    B bobj2(28);

    do_something(B(28));
    //do_something(28);
}

class GamePlayer
{
private:
    enum { NumTurns = 5 };
    int scors[NumTurns];
};

using json = nlohmann::json;
TEST_CASE("json") {
    
    auto j = json::parse(R"({"leaf": {"happy": true, "pi": 3.141}})");
    std::string s = j.dump();
    std::cout << "json " << s << "\n";
    std::cout << "happ: " << j["leaf"]["happy"] << "\n";
}

TEST_CASE("json_create") {
    json j;

    j["pi"] = 3.141;
    j["happy"] = true;
    j["name"] = "Niels";
    j["nothing"] = nullptr;
    j["answer"]["everything"] = 42;
    j["list"] = {1, 0, 2};
    j["object"] = {{"currency", "USD"}, {"value", 42.99}};
    
    std::cout << j.dump() << "\n";
}

TEST_CASE("json_create1") {
    json j2 = {
        {"pi", 3.141},
        {"happy", true},
        {"name", "Niels"},
        {"nothing", nullptr},
        {"answer", {
                       "everything", 42
                   }},
        {"list", {1, 0, 2}},
        {"object", {
                       {"currency", "USD"},
                       {"value", 42.99}
                   }}
        
    };
    std::cout << j2["object"]["value"] << "\n";
    std::cout << j2["nothing"] << "\n";

    std::ofstream out_file("s.json");
    out_file << j2.dump();
}

TEST_CASE("img") {
    int width, height, channels;

    unsigned char* img = stbi_load("lena.png", &width, &height, &channels, 0);
    std::cout << "'width: " << width << " height: " << height << " channel: "
        << channels << "\n";
    REQUIRE(img != nullptr);
    stbi_image_free(img);
}

TEST_CASE("bitshift") {
    unsigned short short1 = 4;
    std::bitset<16> bitset1{short1};
    std::cout << bitset1 << std::endl;

    unsigned short short2 = static_cast<unsigned short>(static_cast<unsigned long long>(short1) << 48);
    std::bitset<16> bitset2{short2};
    std::cout << bitset2 << std::endl;

    unsigned long a = static_cast<unsigned long>(short1) << 48;
    std::bitset<64> b{a};
    std::cout << b << std::endl;
}

// verify method invocation
TEST_CASE("fake_verify") {
    fakeit::Mock<SomeInterface> mock;

    fakeit::When(Method(mock, foo)).Return(0);
    SomeInterface& i = mock.get();
    i.foo(1);
    fakeit::Verify(Method(mock, foo));
    fakeit::Verify(Method(mock, foo).Using(2));
}

struct ISome {
    virtual int foo(int) = 0;
    virtual int bar(int, int) = 0;
    virtual int baz(int*, int&) = 0;
};

TEST_CASE("fakeit_multiple_return1") {
    fakeit::Mock<ISome> mock;
    fakeit::When(Method(mock, foo)).Return(1);

    fakeit::When(Method(mock, foo)).Return(1,2,3);
    
    ISome& i = mock.get();
    std::cout << i.foo(0) << "\n";
    std::cout << i.foo(0) << "\n";
    std::cout << i.foo(0) << "\n";
}

TEST_CASE("fakeit_multiple_return2") {
    fakeit::Mock<ISome> mock;
    fakeit::When(Method(mock, foo)).Return(1);

    fakeit::When(Method(mock, foo)).Return(1).Return(2).Return(3);
    
    ISome& i = mock.get();
    std::cout << i.foo(0) << "\n";
    std::cout << i.foo(0) << "\n";
    std::cout << i.foo(0) << "\n";
}

TEST_CASE("fakeit_always_return") {
    fakeit::Mock<ISome> mock;
    fakeit::When(Method(mock, foo)).Return(1);

    fakeit::When(Method(mock, foo)).AlwaysReturn(1);
    
    ISome& i = mock.get();
    std::cout << i.foo(0) << "\n";
    std::cout << i.foo(0) << "\n";
    std::cout << i.foo(0) << "\n";
}

TEST_CASE("fakeit_foo1_100") {
    fakeit::Mock<ISome> mock;
    fakeit::When(Method(mock, foo).Using(1)).Return(100);

    ISome& i = mock.get();
    std::cout << i.foo(1) << "\n";
}

TEST_CASE("fakeit_foo1_100_a") {
    fakeit::Mock<ISome> mock;
    fakeit::When(Method(mock, foo)).AlwaysReturn(0);
    fakeit::When(Method(mock, foo).Using(1)).AlwaysReturn(100);

    ISome& i = mock.get();
    std::cout << i.foo(4) << "\n";
    std::cout << i.foo(1) << "\n";
    REQUIRE(i.foo(5) == 0);
    REQUIRE(i.foo(1) == 100);
}

TEST_CASE("fakeit_exception") {
    fakeit::Mock<ISome> mock;
    fakeit::When(Method(mock, foo)).Throw(std::exception());
    
    ISome& i = mock.get();
    REQUIRE_THROWS_AS(i.foo(1), std::exception);
}

TEST_CASE("fakeit_output_param") {
    fakeit::Mock<ISome> mock;

    // i.baz(2,3)->1
    fakeit::When(Method(mock, baz)).ReturnAndSet(1, 2, 3);
    ISome& i = mock.get();

    int a, b;
    int c = i.baz(&a, b);
    REQUIRE(a == 2);
    REQUIRE(b == 3);
    REQUIRE(c == 1);
}

TEST_CASE("buffer_overflow") {
    int *p = new int[10];
    p[0] = 0;
    int a = p[0];

    int b = a + 3;
    printf("b %d\n", b);
    delete[] p;
}

TEST_CASE("test") {
    std::cout << "test\n";
}
