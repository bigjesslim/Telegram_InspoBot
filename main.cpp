
#include <string>
#include <random>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <tgbot/tgbot.h>
#include <iostream>
#include "QuoteData.hpp"
#include "Functions.hpp"
using namespace std;
using namespace TgBot;

int main() {
    //Setting up API
    string token(getenv("TOKEN"));
    cout << "Token: " << token.c_str() << endl;

    //Telegram Bot object  created using API token
    Bot bot(token);

    //Declaring/initializing important variables
    std::vector<std::shared_ptr<QuoteTopic>> AllTopics = store_QuoteTopics();
    // flag controls the segment of the chat we are in.
    int flag = 1;
    int TopicIndex;
    std::string Return_Quote;
    std::vector<string> favourites;

    //Start page of the bot
    bot.getEvents().onCommand("start", [&bot, &flag](const Message::Ptr& message) {
        bot.getApi().sendMessage(message->chat->id,
                "Hello, I'm InspoBot. I trade in your worries with a good inspirational quote to help you start the day right. "
                "Think of me as a friend and type in whatever's been worrying you lately."
                "\n \nYou can also key in the command '/topics' to choose from our list of quote topics." );
        flag = 1;
    });

    //To look at and store favourites
    bot.getEvents().onCommand("favourites", [&bot, &favourites](const Message::Ptr& message){
        if (favourites.empty())
            bot.getApi().sendMessage(message->chat->id, "Whoops, looks like you haven't saved any favourites yet. \n"
                                                        "First, find a quote by typing in what's been bothering you, or using the command"
                                                        " '/topics' to choose from our list of quote topics.");
        else{
            for (int n=0;n<favourites.size();n++){
                bot.getApi().sendMessage(message->chat->id, favourites[n] + "\n \n");
            }
        }
    });

    //Creating Inline Keyboards: (Formatting and Respective Queries sent)
    // 1. Quote Keyboard Configurations
    InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
    vector<InlineKeyboardButton::Ptr> topic_row;
    //Creating Buttons
    for(int n=0; n<AllTopics.size(); n++){
        topic_row.clear();
        InlineKeyboardButton::Ptr KeyboardButton (new InlineKeyboardButton);
        KeyboardButton->text = AllTopics[n]->Topic;
        KeyboardButton->callbackData = to_string(n);
        topic_row.push_back(KeyboardButton);
        keyboard->inlineKeyboard.push_back(topic_row);
    }

    //Manual method of access
    bot.getEvents().onCommand("topics", [&bot, &keyboard] (const Message::Ptr& message) {
        bot.getApi().sendMessage(message->chat->id, "Buttons."
                ,false, 0, keyboard, "Markdown");
    });

    //2. Choice Keyboard Configurations
    InlineKeyboardMarkup::Ptr choice(new InlineKeyboardMarkup);
    vector<InlineKeyboardButton::Ptr> choice_row;
    //Creating Buttons
    InlineKeyboardButton::Ptr KeyboardButton_1 (new InlineKeyboardButton);
    KeyboardButton_1->text = "Wrong Subject";
    KeyboardButton_1->callbackData = "Change Subject";
    choice_row.push_back(KeyboardButton_1);

    InlineKeyboardButton::Ptr KeyboardButton_2 (new InlineKeyboardButton);
    KeyboardButton_2->text = "Wrong Quote";
    KeyboardButton_2->callbackData = "Change Quote";
    choice_row.push_back(KeyboardButton_2);

    choice->inlineKeyboard.push_back(choice_row);

    // Actions to be executed depending on what queries are sent
    bot.getEvents().onCallbackQuery([&bot, &AllTopics, &flag, &TopicIndex, &keyboard, &Return_Quote] (const CallbackQuery::Ptr& query) {
        if (query->data.find("Change")!=std::string::npos){
            if (query->data=="Change Subject") {
                bot.getApi().sendMessage(query->message->chat->id,
                                         "Oh no. Here's a few buttons you can choose between to indicate your area of concern instead.",
                                         false, 0, keyboard, "Markdown");
            }
            else{
                Return_Quote = AllTopics[TopicIndex]->generate_quote();
                bot.getApi().sendMessage(query->message->chat->id, "Here's another quote just for you: ");
                bot.getApi().sendMessage(query->message->chat->id, Return_Quote + "\n");
                bot.getApi().sendMessage(query->message->chat->id, "Was the quote useful for you?");
                flag = 2;
            }
        }
        else {
            Return_Quote = AllTopics[stoi(query->data)]->generate_quote();
            bot.getApi().sendMessage(query->message->chat->id, "Here's a quote just for you: ");
            bot.getApi().sendMessage(query->message->chat->id, Return_Quote + "\n");
            bot.getApi().sendMessage(query->message->chat->id, "Was this quote useful for you?");
            flag = 2;

        }
    });

    //Main Telegram Logic
    bot.getEvents().onAnyMessage([&bot, &AllTopics, &flag, &choice, &TopicIndex, &favourites, &Return_Quote](const Message::Ptr &message) {
        printf("User wrote %s\n", message->text.c_str());
        if (StringTools::startsWith(message->text, "/start") || (message->text == "/end") ||
            (message->text == "/topics")|| (message->text == "/favourites")) {
            return;
        }
        //Main Chat
        if (flag == 1) {
            TopicIndex = which_topic(message->text, AllTopics);
            if (TopicIndex == -1) {
                bot.getApi().sendMessage(message->chat->id,
                                         "Sorry, I couldn't quite understand you. Would you like to try again?");
                return;
            } else {
                Return_Quote = AllTopics[TopicIndex]->generate_quote();
                bot.getApi().sendMessage(message->chat->id, "Here's a quote just for you: ");
                bot.getApi().sendMessage(message->chat->id, Return_Quote + "\n");
                bot.getApi().sendMessage(message->chat->id, "Was the quote useful for you?");
                flag = 2;
                return;
            }
        }
        //Feedback Chat
        if (flag == 2) {
            if (check_yes(message->text)) {
                bot.getApi().sendMessage(message->chat->id,
                                         "That's great! Would you like to store the quote in your list of favourites?");
                flag = 3;
                return;
            } else if (check_no(message->text)) {
                bot.getApi().sendMessage(message->chat->id, "Oh no. Why so?", false, 0, choice, "Markdown");
                return;
            } else
                bot.getApi().sendMessage(message->chat->id,"Well that's confusing... :0 \nHelp me out here and key in a simple 'yes' or 'no' please.");
        }
        if (flag == 3) {
            if (check_yes(message->text)) {
                favourites.push_back(Return_Quote);
                std::cout << Return_Quote << std::endl;
                bot.getApi().sendMessage(message->chat->id, "Okay stored! ;)\nYou can view your list of favourites by typing in the command '/favourites'.\n"
                                                            "In the meantime, feel free to tell me anything else that's troubling you.");
                flag = 1;
                return;
            } else if (check_no(message->text)) {
                bot.getApi().sendMessage(message->chat->id,
                                         "Alrighty then. Feel free to let me know of any more of your worries.");
                flag = 1;
                return;
            } else
                bot.getApi().sendMessage(message->chat->id,"Well that's confusing... :0 \nHelp me out here and key in a simple 'yes' or 'no' please.");
        }
    });

    //Capturing unexpected events
    signal(SIGINT, [](int s) {
        printf("SIGINT got\n");
        exit(0);
    });

    // Receiving user inputs via long poll
    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username. c_str());
        bot.getApi().deleteWebhook();
        TgLongPoll LongPoll (bot);
        int poll_on=1;
        while (poll_on) {
            printf("Long poll started\n");
            LongPoll.start();
            // Command to end polling
            bot.getEvents().onCommand("end", [&poll_on](const Message::Ptr& message){ poll_on = 0; });
        }
    }

    catch (exception& e) {
        printf("error: %s\n", e.what()); }
    return 0;
}
