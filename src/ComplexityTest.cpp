#include "FixedHashMap.hpp"
#include <chrono>
#include <vector>
#include <algorithm>
#include <iomanip>
// http://stackoverflow.com/a/32241080
typedef std::chrono::high_resolution_clock Clock;


std::string random_string( size_t length )
{
    auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}

void printInfo(int n, int t, std::string units, int factor) {
    std::cout << std::setw(6) << std::setfill(' ') << n << ": "
              << std::setw(12) << t
              << " " << units
              << "; t/n = "
              << std::setw(8)
              << t/static_cast<double>(n) << " " << units
              << "; t/n/load = "
              << factor * t/static_cast<double>(n) << " " << units
              << std::endl;
}

int main() {
    std::vector<int> nums{80, 160, 320, 640};
    int max = 640000;

    std::cout << "Running with map size = {";
    for (unsigned int i = 0; i < nums.size(); ++i)
        std::cout << nums[i] << ((i < nums.size()-1) ? "," : "");
    std::cout << "}x1000" << std::endl << std::endl;

    std::vector<FixedHashMap<int>> maps;
    std::vector<std::pair<std::string,int*>> vals;
    for (int i = 0; i < max; ++i) {
        vals.push_back(std::make_pair(random_string(16), new int(i)));
    }
    for (unsigned int i = 0; i < nums.size(); ++i) {
        maps.push_back(FixedHashMap<int>(nums[i]*1000));
    }

    std::vector<int> factors{1, 2, 10};
    for (auto const& f : factors) {
        std::cout << "Add operations until load = " << static_cast<float>(1)/f << ":" << std::endl;
        for (unsigned int i = 0; i < nums.size(); ++i) {
            int n = nums[i] * 1000;
            FixedHashMap<int>& map = maps[i];

            auto t1 = Clock::now();
            for (int j = 0; j < n/f; ++j) {
                if (map.set(vals[j].first, vals[j].second) == false) throw;
            }
            auto t2 = Clock::now();
            printInfo(n, std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count(), "ns", f);
        }

        std::cout << "Get operations with load = " << static_cast<float>(1)/f << ":" << std::endl;
        for (unsigned int i = 0; i < nums.size(); ++i) {
            int n = nums[i] * 1000;
            FixedHashMap<int>& map = maps[i];

            auto t1 = Clock::now();
            for (int j = 0; j < n/f; ++j) {
                if(map.get(vals[j].first) != vals[j].second) throw;
            }
            auto t2 = Clock::now();
            printInfo(n, std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count(), "ns",f);

        }

        std::cout << "Delete operations with load = " << static_cast<float>(1)/f << ":" << std::endl;
        for (unsigned int i = 0; i < nums.size(); ++i) {
            int n = nums[i] * 1000;
            FixedHashMap<int>& map = maps[i];

            auto t1 = Clock::now();
            for (int j = 0; j < n/f; ++j) {
                if(map.remove(vals[j].first) != vals[j].second) throw;
            }
            auto t2 = Clock::now();
            printInfo(n, std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count(), "ns",f);
        }

        std::cout << std::endl;
    }

    for (int i = 0; i < max; ++i) {
        delete vals[i].second;
    }

    return 0;
}
