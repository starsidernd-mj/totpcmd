

class Entry {
	private:
	
	struct EntryInfo {
		std::string name;
		std::string token;
	};
	
	EntryInfo entry_d;
	
	public:
	
	Entry() : entry_d{"", ""} {};
	
	std::string addName() {
		char issuer[100];
		int i = 0;
		
		while(true) {
			int ch = getch();
			if (ch == '\n' || ch == '\r') {
				break; // End of input
			} else {
				issuer[i++] = ch;
				mvprintw(3, 16, issuer);
				refresh();
			}
		}
		issuer[i] = '\0';
		
		entry_d.name.assign(issuer);
		
		return entry_d.name;
	}
	
	std::string addToken() {
		char password[100];
		int i = 0;
		
		while(true) {
			int ch = getch();
			if (ch == '\n' || ch == '\r') {
				break; // End of input
			} else if (ch == '\b' || ch == 127) {
				// Handle backspace
				if (i > 0) {
					--i;
					mvprintw(5, 16 + i, " "); // Clear the character
					refresh();
				}
			} else {
				// Handle normal character input
				if (i < sizeof(password) - 1) {
					password[i++] = ch;
					mvprintw(5, 16 + i - 1, "*"); // Display asterisk
					refresh();
				}
			}
		}
		password[i] = '\0'; // Null-terminate the password string
		// Now you have the password in the 'password' variable

		entry_d.token.assign(password);
		return entry_d.token;
	}
};
