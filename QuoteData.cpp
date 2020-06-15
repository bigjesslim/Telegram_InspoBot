#include <string>
#include <iostream>
#include <random>
#include <fstream>
#include <vector>
#include <utility> // std::pair
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream
#include "QuoteData.hpp"
#include "Functions.hpp"

//QuoteTopic Functions
QuoteTopic:: QuoteTopic(std::string const topic): Topic(topic){
    quotelist = generate_quotelist(topic);
}

std::string QuoteTopic:: generate_quote() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, quotelist.size()-1);
    return quotelist[distr(gen)];
}

std::vector<std::string> QuoteTopic::generate_quotelist(std::string const topic) {
    std::ifstream QuotesFile("quote_database.txt", std::ios::in | std::ios::binary);

    if (!QuotesFile.is_open())
        throw std::runtime_error("Could not open file");

    std::string line, word;

    if (QuotesFile.good()) {
        while (getline(QuotesFile, line, '\r')) {
            std::istringstream s(line);
            getline(s, word, '/');
            if (word == topic) {
                while (getline(s, word, '/')) {
                    quotelist.push_back(word);
                }
                break;
            }
        }
    }
    return quotelist;
}

int QuoteTopic::match_keywords(std::string const& sentence){
    //storing related keywords into a temporary vector
    std::ifstream KeywordsFile("topic_keywords.csv", std::ios::in | std::ios::binary);
    if (!KeywordsFile.is_open())
        throw std::runtime_error("Could not open file");
    std::string line, key;
    std::vector<std::string> keywords;
    if (KeywordsFile.good()) {
        while (getline(KeywordsFile, line, '\r')) {
            std::istringstream s(line);
            getline(s, key, ',');
            if (key == Topic) {
                while (getline(s, key, ',')) {
                    keywords.push_back(key);
                }
                break;
            }
        }
    }

    //counting the number of matched keywords
    int count = 0;
    std::string word;
    std::stringstream ss(sentence);
    while(ss>>word) {
        //if present, removes commas from strings
        word.erase(std::remove(word.begin(), word.end(), ','), word.end());
        for (const auto& e : keywords) {
            if (e == word)
                count++;
        }
    }
    return count;
}
