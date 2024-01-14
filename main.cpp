#include <iostream>
#include <string>
#include <vector>
#include <list>

using namespace std;

template <typename T>
struct is_string
{
    static const bool value = false;
};

template <>
struct is_string<string> 
{
    static const bool value = true;
};

template <typename T> 
auto print_ip(T ip) -> decltype(T() == 0)
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

template <typename T> 
auto print_ip(T ip) -> decltype(ip.begin())
{
    for (auto it = ip.begin(); it != ip.end(); it++){
        cout << *it;
        if (!is_string<T>::value && std::distance<decltype(it)>(it, ip.end()) > 1) {
	    cout << ".";
        }
    }
    cout << endl;
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
