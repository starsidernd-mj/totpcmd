

class Viewer {
	
	private:
	int pos = 1;
	
	public:
	
	TOTP totpGen;
	std::string totp;
	
	Viewer() {};
	
	void drawViewer() {
		clear();
		
		pos = 1;
		
		//print header
		mvprintw(pos++, 1, "Viewer");
		mvprintw(pos++, 1, "Q. Back");
		pos++;
		
		//refresh();
	}
	
	void drawEntries(std::vector<Entry*> keys) {
		// get current UTC time
		auto now = std::chrono::system_clock::now();
		std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
		uint64_t currentTime64 = static_cast<uint64_t>(currentTime);
		
		// time step
		uint64_t timeStep = 30;
		
		// calculate counter based on current time and time step
		uint64_t counter = currentTime64 / timeStep;
		
		for(Entry* i : keys) {
			totp = totpGen.generateTOTP(i->getEntry().token, counter);
			//pos+=2;
			pos++;
			//mvprintw(pos++, 1, i->getEntry().name.c_str());
			//mvprintw(pos++, 1, totp.c_str());
			
			mvprintw(pos, 1, totp.c_str());
			mvprintw(pos++, totp.length()+3, i->getEntry().name.c_str());
			mvprintw(pos, 1, i->getEntry().token.c_str());
		}
	}
	
	void subMenu(bool *running) {
		// Get user input
		int choice = getch();
		
		//disable no delay
		nodelay(stdscr, false);
		
		// Process user input
		if(choice != ERR) {
			switch(choice) {
				case 'q':
				case 'Q': {
					*running = false;
					break;
				}
			}
		}
		
		//enable no delay
		nodelay(stdscr, true);
	}
	
	int getPos() {
		return pos;
	}

};
