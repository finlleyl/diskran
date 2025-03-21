#include <iostream> 
#include <vector>
#include <string>


void countSort(std::vector<std::pair<int, std::string>>& vec, int k) {
    if (vec.empty()) return;
    
    std::vector<int> count(k, 0);
    for (const auto &p : vec) {
        count[p.first]++;
    }
    
    for (int i = 1; i < k; i++) {
        count[i] += count[i - 1];
    }
    
    std::vector<std::pair<int, std::string>> output(vec.size());
    
    for (int i = vec.size() - 1; i >= 0; i--) {
        int key = vec[i].first;
        int pos = count[key] - 1;
        output[pos] = vec[i];
        count[key]--;
    }
    
    vec = std::move(output);
}


int main() {
    std::vector<std::pair<int, std::string>> vec;
    std::string line;

    while(std::getline(std::cin, line)) {
        size_t tabPos = line.find('\t');
        if (tabPos != std::string::npos) {
            int key = std::stoi(line.substr(0, tabPos));
            std::string value = line.substr(tabPos + 1);

            vec.push_back(std::make_pair(key, value));
        }
    }

    countSort(vec, 65536);

    for (auto& el : vec) {
        std::cout << el.first << '\t' << el.second << '\n';
    }
    std::flush(std::cout);

    return 0;
}