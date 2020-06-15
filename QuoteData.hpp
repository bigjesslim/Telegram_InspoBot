//
// Created by Jessica Halim on 6/6/20.
//

#ifndef CSV_TESTING_QUOTEDATA_HPP
#define CSV_TESTING_QUOTEDATA_HPP


struct QuoteTopic{
    explicit QuoteTopic (std::string const topic);
    ~QuoteTopic() = default;
    std::string generate_quote();
    std::vector<std::string> quotelist;
    int match_keywords (std::string const& sentence);
    std::string const Topic;
private:
    std::vector<std::string> generate_quotelist(std::string const topic);
};

#endif //CSV_TESTING_QUOTEDATA_HPP
