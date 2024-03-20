#include <ncurses.h>
#include <string>
#include <vector>
#include <ctime>
#include <chrono>
#include <thread>
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <vector>

#include "totp.cpp"
#include "screenconfig.cpp"
#include "entry.cpp"
#include "viewer.cpp"

// Function declarations
void initialize();
void drawMainMenu();
void cleanup();
void enableNonBlockingInput();
void restoreBlockingInput();
void drawAddMenu();
int loopAddMenu(Entry *entry, bool *running);
void drawClock(int position);

std::string secretKey = "";
std::string totp = "";

std::vector<Entry*> keys;

int pos = 1;

int main() {
    // System-specific commands to set terminal size and position
    ScreenConfig* sc = new ScreenConfig();
    
    // Get screen size
    int screen_rows, screen_cols;
    sc->get_screen_resolution(&screen_rows, &screen_cols);
    
    // Calculate the rows and columns to set the window size to
    int dst_rows, dst_cols;
    sc->calculate_terminal_size(screen_rows, screen_cols, 8, 16, &dst_rows, &dst_cols);
    
    //std::cout << "Screen res " << dst_rows << ", " << dst_cols << std::endl;
    
    // Resize the terminal
    sc->resize_terminal(dst_rows, 80);
    
    // Get terminal position
    int screen_x, screen_y;
    sc->get_terminal_position(&screen_x, &screen_y);
    //sc.position_terminal();
    
    // Destory ScreenConfig object
    delete sc;
    
    initialize(); // Initialize ncurses
    
    // Enable non-blocking input
    //enableNonBlockingInput();
    nodelay(stdscr, true);

    // Draw the main menu
    drawMainMenu();
    
    // Main loop
    bool running = true;
    // bool to keep menu open
    bool add_menu_running = true;
    // bool to keep viewer open
    bool viewer_menu_running = true;
    
    
    while (running) {
        drawMainMenu(); // Draw the main menu

        // Get user input
        int choice = getch();

        // Process user input
        if(choice != ERR) {
            switch (choice) {
                case '1': {
                    Entry* en = new Entry();
                    add_menu_running = true;
                    int success = 0;
                    
                    //disable no delay
                    nodelay(stdscr, false);
                    
                    
                    while(add_menu_running) {
                        drawAddMenu();
                        success = loopAddMenu(en, &add_menu_running);
                        
                        //check if entry was added, if so add to list, else delete memory
                        pos+=4;
                        switch(success) {
                            case 0: {
                                delete en;
                                mvprintw(pos, 1, "Entry canceled");
                                refresh();
                                break;
                            }
                            case 1: {
                                keys.push_back(en);
                                mvprintw(pos, 1, "Entry added");
                                refresh();
                                break;
                            }
                            case 2: {
                                mvprintw(pos, 1, "Please enter name and token");
                                refresh();
                                // Wait for 1 second
                                std::this_thread::sleep_for(std::chrono::seconds(1));
                                break;
                            }
                            default: {
                                break;
                            }
                        }
                    }

                    //re-enable no delay for menus
                    nodelay(stdscr, true);
                    
                    break;
                }
                /*case '2': {
                    // Add your functionality for option 2 here
                    //replace time step
                    uint64_t timeStep = 30;

                    //get current time in seconds
                    uint64_t currentTime = std::time(nullptr);

                    //calculate counter based on current time and time step
                    uint64_t counter = currentTime / timeStep;
                    
                    TOTP totpGen;
                    totp = totpGen.generateTOTP(secretKey, counter);
                    mvprintw(7, 1, "TOTP: %s", totp.c_str());
                    refresh(); 
                    break;
                }*/
                case '3': {
                    viewer_menu_running = true;
                    
                    Viewer* vw = new Viewer();
                    
                    //disable no delay for menus
                    //nodelay(stdscr, false);
                    
                    while(viewer_menu_running) {
                        vw->drawViewer();
                        vw->drawEntries(keys);
                        
                        int clockPos = vw->getPos()+4;
                        drawClock(clockPos);
                        
                        vw->subMenu(&viewer_menu_running);
                        
                        refresh();
                        
                        // Wait for 1 second
                        //std::this_thread::sleep_for(std::chrono::seconds(1));
                    }
                    
                    //re-enable no delay for menus
                    //nodelay(stdscr, true);
                    
                    /*
                    // Add your functionality for option 2 here
                    //replace time step
                    uint64_t timeStep = 30;

                    //get current time in seconds
                    uint64_t currentTime = std::time(nullptr);

                    //calculate counter based on current time and time step
                    uint64_t counter = currentTime / timeStep;
                    
                    TOTP totpGen;
                    totp = totpGen.generateTOTP(secretKey, counter);
                    //mvprintw(7, 1, "TOTP: %s", totp.c_str());
                    refresh(); */
                    break;
                }
                case 'q':
                case 'Q':
                    running = false; // Quit the program
                    break;
            }
        }
        
        refresh(); // Refresh the screen
            
        // Wait for 1 second
        //std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    // Restore blocking input before exiting
    restoreBlockingInput();

    cleanup(); // Cleanup ncurses
    return 0;
}

// Function to enable non-blocking input
void enableNonBlockingInput() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

// Function to restore blocking input
void restoreBlockingInput() {
    struct termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    oldt.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

// init ncurses
void initialize() {
    initscr(); // Initialize ncurses
    cbreak(); // Disable line buffering
    noecho(); // Don't echo user input
    keypad(stdscr, true); // Enable keypad for arrow keys, etc.
}

// draw the main menu
void drawMainMenu() {
    clear(); // Clear the screen
    
    pos = 1;

    // Print the menu options
    mvprintw(pos++, 1, "Main Menu");
    mvprintw(pos++, 1, "1. Add entry");
    mvprintw(pos++, 1, "2. Remove entry");
    mvprintw(pos++, 1, "3. View entries"); 
    mvprintw(pos++, 1, "Q. Quit");
    pos++;
    //mvprintw(pos++, 1, "TOTP: %s", totp.c_str());
    
    // Print current time
    drawClock(++pos);

    refresh(); // Refresh the screen
    
    //usleep(100000);
}

void drawClock(int position) {
    // Get current time
    time_t now = time(0);
    tm *ltm = localtime(&now);
    
    auto nowt = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(nowt);

    std::stringstream ss;
    ss << current_time;
    std::string time_str = ss.str();
    
    // Print current time
    //mvprintw(position++, 1, "Current UTC Time: %02d:%02d:%02d", ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
    mvprintw(position++, 1, time_str.c_str());
    
    // Draw blocks based on 30 timer
    int bar_cntr;
    bar_cntr = 30 - (ltm->tm_sec % 30);

    for(int i = 1; i <= bar_cntr; i++) {
        mvprintw(position, i, "=");
    }
    
    // Wait for 1 second
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

// entry items
std::string entry_name;
std::string entry_token;
std::string entry_token_hidden;

int namepos = 0;
int tokenpos = 0;

// draw the meny for adding entries
void drawAddMenu() {
    clear();
    
    pos = 1;
    
    // Print header
    mvprintw(pos++, 1, "Add item");
    pos++;
    namepos = pos;
    mvprintw(pos++, 1, "1. Name: ");
    mvprintw(pos++, 1, "2. Issuer: ");
    tokenpos = pos;
    mvprintw(pos++, 1, "3. Token: ");
    mvprintw(pos++, 1, "4. Create");
    mvprintw(pos++, 1, "Q. Cancel");
    
    mvprintw(namepos, 16, entry_name.c_str());
    mvprintw(tokenpos, 16, entry_token_hidden.c_str());
    
    refresh();
}

// loop through the options of the entry add menu
int loopAddMenu(Entry *entry, bool *running) {
    // Get user input
    int choice = getch();

    // Process user input
    if(choice != ERR) {
        switch (choice) {
            case '1': {
                entry_name = entry->addName(namepos);
                break;
            }
            case '2': {

                break;
            }
            case '3': {
                entry_token = entry->addToken(tokenpos);
                entry_token_hidden = entry_token;
                std::fill(entry_token_hidden.begin(), entry_token_hidden.end(), '*');
                break;
            }
            case '4': {
                if(!(entry_name.empty()) && !(entry_token.empty())) {
                    *running = false;
                    entry_name.clear();
                    entry_token_hidden.clear();
                    entry_token.clear();
                    return 1;
                } else {
                    //pos+=4;
                    //mvprintw(pos, 1, "Please enter a name and token");
                    return 2;
                }
            }
            case 'q':
            case 'Q': {
                *running = false;
                entry_name.clear();
                entry_token_hidden.clear();
                entry_token.clear();

                //break;
                return 0;
            }
        }
    }
    return -1;
}

void cleanup() {
    endwin(); // Cleanup ncurses
}
