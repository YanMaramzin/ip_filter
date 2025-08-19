#include <algorithm>
#include <charconv>
#include <iostream>
#include <regex>
#include <vector>

// Программа из стандартного ввода читает данные.
// Данные хранятся построчно. Каждая строка состоит из трех полей, разделенных одним символом табуляции, и завершается символом конца строки.

// **Формат строки:**
//
// text1 \\t text2 \\t text3
//
// Поля text2 и text3 игнорируются. Поле text1 имеет следующую структуру (ip4 address):
//
// n1.n2.n3.n4 где n1..4 – целое число от 0 до 255.
//
// Требуется загрузить список ip-адресов в память и отсортировать их в обратном лексикографическом порядке. Пример лексикографической сортировки (по первому числу, затем по второму и так далее):
//
// 1\.1.1.1 1.2.1.1 1.10.1.1
//
// Соответственно, обратная:
//
// 1\.10.1.1 1.2.1.1 1.1.1.1
//
// 💡 Обратите внимание – сортировка выполняется в байтовом (численном) представлении IP-адреса, а не в строковом. Поэтому адрес, например, 1.2.1.1 меньше адреса 1.10.1.1 (ведь 2 < 10).
//
// **Далее выводим в стандартный вывод следующее:**
//
// 1. Полный список адресов после сортировки. Одна строка - один адрес.
// 2. Сразу следом список адресов, первый байт которых равен 1. Порядок сортировки не меняется. Одна строка - один адрес. Списки ничем не разделяются.
// 3. Сразу продолжается список адресов, первый байт которых равен 46, а второй 70. Порядок сортировки не меняется. Одна строка - один адрес. Списки ничем не разделяются.
// 4. Сразу продолжается список адресов, любой байт которых равен 46. Порядок сортировки не меняется. Одна строка - один адрес. Списки ничем не разделяются.
std::vector<std::string> split(const std::string& str, const std::string_view delimiter)
{
    std::vector<std::string> result;
    for (size_t start = 0, end; (end = str.find(delimiter, start)) != std::string::npos; start = end + 1)
        result.emplace_back(str.substr(start, end - start));

    result.emplace_back(str.substr(str.find_last_not_of(delimiter)));
    return result;
}

void printIp(const std::vector<std::string> &ips)
{
    for (const auto &ip : ips)
        std::cout << ip << std::endl;
}

std::vector<std::string> filter(const std::string &value, std::vector<std::string> ptr)
{
    std::vector<std::string> result;
    std::regex ipRegex(value);
    std::copy_if(ptr.begin(), ptr.end(),
                 std::back_inserter(result),
                 [ipRegex](const std::string &x) {
                    return std::regex_match(x, ipRegex);
                 });
    return result;
}

int main()
{
    std::vector<std::string> tmp {
        "113.162.145.156	111	0",
        "157.39.22.224	5	6",
        "79.180.73.190	2	1",
        "179.210.145.4	22	0"
    };
// 219.102.120.135	486	0
// 67.232.81.208	1	0
// 85.254.10.197	0	7
// 23.240.215.189	6	0
// 1.29.168.152	17	0
// 185.69.186.168	1	3}

    std::vector<std::string> ip_pool;
    for(std::string line; std::getline(std::cin, line);)
    {
        auto v = split(line, "\t");
        ip_pool.push_back(v.at(0));
    }

    std::sort(ip_pool.begin(), ip_pool.end(), [](const std::string &s1, const std::string &s2) {
        const auto tmp1 = split(s1, ".");
        const auto tmp2 = split(s2, ".");

        for (size_t i = 0; i < tmp1.size(); ++i) {
            const auto result = std::stoi(tmp1[i]);
            const auto result2 = std::stoi(tmp2[i]);
            if (result == result2)
                continue;

            return result > result2;
        }
        return false;
    });

    printIp(ip_pool);
    std::cout << std::endl;
    printIp(filter("^1\\..*", ip_pool));
    std::cout << std::endl;
    printIp(filter("^46\\.70\\..*", ip_pool));
    std::cout << std::endl;
    printIp(filter(".*46.*", ip_pool));

    return 0;
}