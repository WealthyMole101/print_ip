#include <iostream>
#include <string>
#include <vector>
#include <list>

using namespace std;

// is_container from https://stackoverflow.com/questions/9407367/determine-if-a-type-is-an-stl-container-at-compile-time

template <typename T>
struct has_begin_end
{
    template<typename C> static char (&f(typename std::enable_if<
      std::is_same<decltype(static_cast<typename C::const_iterator (C::*)() const>(&C::begin)),
      typename C::const_iterator(C::*)() const>::value, void>::type*))[1];

    template<typename C> static char (&f(...))[2];

    template<typename C> static char (&g(typename std::enable_if<
      std::is_same<decltype(static_cast<typename C::const_iterator (C::*)() const>(&C::end)),
      typename C::const_iterator(C::*)() const>::value, void>::type*))[1];

    template<typename C> static char (&g(...))[2];

    static bool const beg_value = sizeof(f<T>(0)) == 1;
    static bool const end_value = sizeof(g<T>(0)) == 1;
};

template<typename T> 
struct is_container : std::integral_constant<bool, has_begin_end<T>::beg_value && has_begin_end<T>::end_value> 
{ };

template <typename T, enable_if_t<(is_container<T>::value && !std::is_same_v<T, std::string>), T>...>
void print_ip(T ip)
{
    for (auto it = ip.begin(); it != ip.end(); it++){
        cout << *it;
        if (std::distance<decltype(it)>(it, ip.end()) > 1) {
	    cout << ".";
        }
    }
    cout << endl;
}

template <typename T, enable_if_t<is_integral_v<T>, T>...>
void print_ip(T ip)
{
    int size = sizeof(ip);

    for (int i = 0; i < size; i++){
	cout << std::to_string((reinterpret_cast<uint8_t*>(&ip))[size - i - 1]);

	if (i < (size - 1)) {
	    cout << ".";
	}
    }

    cout << endl;
}

template <typename T, enable_if_t<is_same_v<T, std::string>, T>...>
void print_ip(T ip)
{
    cout << ip << endl;
}

int main(int, char **) 
{
    print_ip( int8_t{-1} ); // 255
    print_ip( int16_t{0} ); // 0.0
    print_ip( int32_t{2130706433} ); // 127.0.0.1
    print_ip( int64_t{8875824491850138409} );// 123.45.67.89.101.112.131.41
    print_ip( std::string{"Hello, World!"} ); // Hello, World!
    print_ip( std::vector<int>{100, 200, 300, 400} ); // 100.200.300.400
    print_ip( std::list<short>{400, 300, 200, 100} ); // 400.300.200.100
    //print_ip( std::make_tuple(123, 456, 789, 0) ); // 123.456.789.0

    return 0;
}
