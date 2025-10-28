#ifndef _CFDIALGORITHMS_HPP_
#define _CFDIALGORITHMS_HPP_

#include <string>
#include <list>
#include <ranges>
#include <iostream>
#include <utility>
#include <functional>

namespace cfdi {
    using std::invoke;
    using std::forward;
    using std::ranges::begin;
    using std::ranges::end;
    using std::ranges::input_range;
    using std::string;

    template <input_range Range, typename T>
    string join(const Range& range, const string& delimiter, T&& transform)
    {
        string result { };

        auto it = begin(range);
        auto endit = end(range);        
        if (it == endit) {
            return result;
        }
        
        bool first { true };
        while (it != endit) {
            auto value { invoke(forward<T>(transform), *it) };
            if (!value.empty()) {
                if (!first) {
                    result += delimiter;
                }
                result += value;
                first = false;
            }
            ++it;
        }

        return result;
    }
}

#endif 
