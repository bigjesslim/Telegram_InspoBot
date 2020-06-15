# Telegram_InspoBot
A telegram bot built in C++ to respond to user string inputs with relevant inspirational quotes.
I used the API 'tgbot' created for coding Telegram Bots using C++ here: https://github.com/reo7sp/tgbot-cpp. 
 I'm a huge amateur here, I just learnt C++ around 3 weeks ago using a short tutorial which I'm not sure is comprehensive, and I just started coding in general a year ago when I started my university degree in Computer Science.   
 
## Current Features/Functions of the Bot
The user starts the bot function using '/start' - the bot prompts the user 
and this brings us to the main interface where:
1. The program accepts a string from the user which details any troubles or any general concerns the user has regarding life
2. Parses through to check for keywords that indicate a certain genre/topic of quotes
3. If a topic is identified: a random quote from the topic will be returned

After this, the a feedback interface is initialised and the user is asked if the quote was useful
- Some form of yes would redirect the user to the main interface
- An ambiguous answer not detected to be either yes or no would prompt the user to key in only either one
- Some form of no would allow the user to choose the reason of this: Was it the correct topic and wrong quote or the wrong topic?  
Wrong Topic: leads user to an interface of inline buttons where they can choose the relevant topic.  
Wrong Quote: would simply generate another random quote of the same topic.  

## Extra features
After every quote is generated, the user is asked if they want to add it to their favourites list - which they can view using the command '\favourites'

# Feedback
I would like to ask for feedback in terms of:
1. How can my current code be improved in terms of good C++ coding and structure?
2. What kind of domains should I research and read up on to better my knowledge regarding C++?
3. What kind of features would be useful to add for users?
4. What kind of features would be useful to add for my learning with regards to C++?
5. What kind of features would be cool in a resume? (I understand that this is may seem like a superficial reason - but my resume is sorely lacking at the moment and would be crucial in helping me gain access to internships where I can further my learning.)

Thank you so much for reading through :-)
