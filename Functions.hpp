//
// Created by Jessica Halim on 10/6/20.
//

#ifndef INSPOBOT_FUNCTIONS_HPP
#define INSPOBOT_FUNCTIONS_HPP

std::vector<std::shared_ptr<QuoteTopic>> store_QuoteTopics();
int which_topic(std::string const& sentence, std::vector<std::shared_ptr<QuoteTopic>> AllTopics);

bool check_yes(std::string const& word);
bool check_no(std::string const& word);

bool compare_char (char const& a, char const& b);
bool compare_str (std::string const& a, std::string const& b);
bool operator==(std::string const& a, std::string const& b);
bool operator!=(std::string const& a, std::string const& b);


#endif //INSPOBOT_FUNCTIONS_HPP
