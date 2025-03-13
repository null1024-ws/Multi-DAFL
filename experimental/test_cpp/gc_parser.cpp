#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <array>

struct IfRangeData {
    int index;
    int share;
    std::array<long long, 2> low;
    std::array<long long, 2> high;
    std::array<long long, 2> expr;
    int andor;
};

// 解析函数
IfRangeData parseIfRange(const std::string& input) {
    std::regex pattern(R"(\[if_range\]: index\((-?\d+)\), share\((-?\d+)\), low\((-?\d+),\s*(-?\d+)\), high\((-?\d+),\s*(-?\d+)\), expr\((-?\d+),\s*(-?\d+)\), andor\((-?\d+)\))");
    std::smatch match;

    IfRangeData data;

    if (std::regex_search(input, match, pattern)) {
        // 按顺序提取匹配到的数值
        data.index = std::stoi(match[1]);
        data.share = std::stoi(match[2]);
        data.low = {std::stoll(match[3]), std::stoll(match[4])};
        data.high = {std::stoll(match[5]), std::stoll(match[6])};
        data.expr = {std::stoll(match[7]), std::stoll(match[8])};
        data.andor = std::stoi(match[9]);
    } else {
        throw std::invalid_argument("Input string does not match the expected format.");
    }

    return data;
}

// 测试代码
int main() {
    std::string input = "[if_range]: index(10), share(10), low(1, 9223372036854775807), high(10, 14), expr(100, 20), andor(1)";
    
    std::ifstream file("/data/sda_18tdisk/morangeous/Fuzzing/multi-dafl-code/DAFL/experimental/test_cpp/result.txt");


    try {
        std::string line;
        while (getline(file, line)){
            
        IfRangeData data = parseIfRange(line);
        // 输出解析结果
        std::cout << "***************begin**************" << std::endl;
        std::cout << "Index: " << data.index << "\n";
        std::cout << "Share: " << data.share << "\n";
        std::cout << "Low: [" << data.low[0] << ", " << data.low[1] << "]\n";
        std::cout << "High: [" << data.high[0] << ", " << data.high[1] << "]\n";
        std::cout << "Expr: [" << data.expr[0] << ", " << data.expr[1] << "]\n";
        std::cout << "AndOr: " << data.andor << "\n";
        std::cout << "***************end**************" << std::endl;
        }



    } catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
