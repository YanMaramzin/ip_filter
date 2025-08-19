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
    std::vector<std::vector<std::string> > ip_pool;
    for(std::string line; std::getline(std::cin, line);)
    {
        std::vector<std::string> v = split(line, "\t");
        ip_pool.push_back(split(v.at(0), "."));
    }

    std::vector<std::string> ips = {{"1.1.1.1"}, {"46.2.1.1"}, {"1.10.1.1"}, {"1.46.1.1"}};

    std::sort(ips.begin(), ips.end(), [](const std::string &s1, const std::string &s2) {
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

    std::cout << "test";
    printIp(ips);
    std::cout << std::endl;
    printIp(filter("^46\\..*", ips));
    std::cout << std::endl;
    printIp(filter(".*46.*", ips));

    return 0;
}