#include <iostream>
#include <string>
#include <unordered_map>
#include <stack>
#include <cassert>

class calculator {
    std::unordered_map<char, int> values;
    std::unordered_map<int, char> letters;
    std::stack<char> operators;
    std::stack<long long> numbers;


    long long parse_rome(const std::string &number) {
        long long result = 0;
        for (unsigned i = 0; i < number.size(); ++i) {
            int value = values[number[i]];
            if (i == number.size() - 1) {
                result += value;
            } else {
                if (value < values[number[i + 1]]) {
                    value *= -1;
                }
                result += value;
            }
        }

        return result;
    }

    std::string parse_arab(long long number) {
        std::string result;
        if (number == 0) {
            return "Z";
        }

        int d = 1000;
        while (d > 0) {
            int x = number / d;
            if (d == 1000) {
                for (int i = 0; i < x; ++i) {
                    result += letters[d];
                }
            } else {
                switch (x) {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                        for (int i = 0; i < x; ++i) {
                            result += letters[d];
                        }
                        break;
                    case 4:
                        result += letters[d];
                        result += letters[5 * d];
                        break;
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                        result += letters[5 * d];
                        for (int i = 0; i < x - 5; ++i) {
                            result += letters[d];
                        }
                        break;
                    case 9:
                        result += letters[d];
                        result += letters[10 * d];
                        break;
                    default:
                        assert(0);
                }
            }
            number -= x * d;
            d /= 10;
        }

        return result;
    }


public:

    std::string calc(const std::string &s) {
        while (!operators.empty()) {
            operators.pop();
        }
        while (!numbers.empty()) {
            numbers.pop();
        }

        for (unsigned i = 0; i < s.size(); i++) {
            if (s[i] == ' ') {
                continue;
            }
            if (values.find(s[i]) != values.end()) {
                std::string number_str;
                while (values.find(s[i]) != values.end()) {
                    number_str += s[i];
                    i++;
                }
                long long number = parse_rome(number_str);
                if (!operators.empty() && (operators.top() == '/' || operators.top() == '*')) {
                    long long tmp = numbers.top();
                    numbers.pop();
                    if (operators.top() == '*') {
                        numbers.push(tmp * number);
                    } else {
                        numbers.push(tmp / number);
                    }
                    operators.pop();
                    if (!operators.empty() && operators.top() == '-') {
                        operators.pop();
                        operators.push('+');
                        long long x = numbers.top();
                        numbers.pop();
                        numbers.push(-x);
                    }
                } else {
                    if (!operators.empty() && operators.top() == '-') {
                        operators.pop();
                        operators.push('+');
                        number = -number;
                    }
                    numbers.push(number);
                }
                i--;
            }


            else {
                if (s[i] == ')') {
                    while (operators.top() != '(') {
                        long long last = numbers.top();
                        numbers.pop();
                        long long prelast = numbers.top();
                        numbers.pop();
                        if (operators.top() == '+') {
                            numbers.push(prelast + last);
                        } else {
                            numbers.push(prelast - last);
                        }
                        operators.pop();
                    }
                    operators.pop();
                    if (!operators.empty()) {

                        if (operators.top() == '*') {
                            long long last = numbers.top();
                            numbers.pop();
                            long long prelast = numbers.top();
                            numbers.pop();
                            numbers.push(last * prelast);
                            operators.pop();
                        }

                        if (!operators.empty() && operators.top() == '/') {
                            long long last = numbers.top();
                            numbers.pop();
                            long long prelast = numbers.top();
                            numbers.pop();
                            numbers.push(prelast / last);
                            operators.pop();
                        }

                        if (!operators.empty() && operators.top() == '-') {
                            operators.pop();
                            operators.push('+');
                            long long x = numbers.top();
                            numbers.pop();
                            numbers.push(-x);
                        }
                    }
                }

                else {
                    operators.push(s[i]);
                }
            }
        }
        while (!operators.empty()) {
            long long last = numbers.top();
            numbers.pop();
            long long prelast = numbers.top();
            numbers.pop();
            if (operators.top() == '+') {
                numbers.push(prelast + last);
            }
            operators.pop();
        }
        long long result = numbers.top();
        numbers.pop();
        return parse_arab(result);
    }

    calculator() {
        values['Z'] = 0, letters[0] = 'Z';
        values['I'] = 1, letters[1] = 'I';
        values['V'] = 5, letters[5] = 'V';
        values['X'] = 10, letters[10] = 'X';
        values['L'] = 50, letters[50] = 'L';
        values['C'] = 100, letters[100] = 'C';
        values['D'] = 500, letters[500] = 'D';
        values['M'] = 1000, letters[1000] = 'M';
    }
};

int main() {
    calculator calc;
    std::string input;
    while(std::getline(std::cin, input)) {
        std::cout << calc.calc(input) << std::endl;
    }
}
