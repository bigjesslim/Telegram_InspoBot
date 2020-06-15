#include <string>
#include <iostream>
#include <random>
#include <fstream>
#include <vector>
#include <utility> // std::pair
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream
#include <boost/algorithm/string.hpp>
#include "QuoteData.hpp"
#include "Functions.hpp"

std::vector<std::shared_ptr<QuoteTopic>> store_QuoteTopics(){
    std::vector<std::shared_ptr<QuoteTopic>> AllTopics;
    std::ifstream QuoteFile("quote_database.txt", std::ios::in | std::ios::binary);

    if (!QuoteFile.is_open())
        throw std::runtime_error("Could not open file");
    std::string line, word;

    if (QuoteFile.good()) {
        while (getline(QuoteFile, line, '\r')) {
            std::istringstream s(line);
            getline(s, word, '/');
            AllTopics.push_back(std::shared_ptr<QuoteTopic>(new QuoteTopic(word)));
        }
    }
    return AllTopics;
}

int which_topic(std::string const& sentence, std::vector<std::shared_ptr<QuoteTopic>> AllTopics){
    int index = -1;
    int NumKeywords = 0;
    for(int n=0;n<AllTopics.size();n++){
        if (AllTopics[n]->match_keywords(sentence)>NumKeywords) {
            index = n;
            NumKeywords = AllTopics[index]->match_keywords(sentence);
        }
        else if ((AllTopics[n]->match_keywords(sentence)==NumKeywords)&&(NumKeywords!=0)){
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distr(1,2);
            index = (distr(gen) == 1)? index : n;
            NumKeywords = AllTopics[index]->match_keywords(sentence);
        }
    }
    return index;
}



//General Use Functions
bool check_yes(std::string const& word){
    std::string yes_words = "yep yes yeap sure of course indeed affirmative absolutely yup yah yeh yeet";
    if (yes_words.find(word) != std::string::npos)
        return true;
    else{
        std::string lower_word = boost::to_lower_copy(word);
        std::cout << lower_word << std::endl;
        return yes_words.find(lower_word) != std::string::npos;
    }
}

bool check_no(std::string const& word){
    std::string no_words = "no nope negative not at all nah no way naw not really absolutely not of course not";
    if (no_words.find(word) != std::string::npos)
        return true;
    else{
        std::string lower_word = boost::to_lower_copy(word);
        std::cout << lower_word << std::endl;
        return (no_words.find(lower_word) != std::string::npos);
    }
}

bool compare_char (char const& a, char const& b){
    return (a == b) || (std::toupper(a) == std::toupper(b));
}

bool compare_str (std::string const& a, std::string const& b){
    return ((a.size() == b.size())&&(std::equal(a.begin(), a.end(),b.begin(), &compare_char)));
}

bool operator==(std::string const& a, std::string const& b) {
    return compare_str(a, b);
}

bool operator !=(std::string const& a, std::string const& b) {
    return !(compare_str(a, b));
}