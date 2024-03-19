

class Entry {
	public:
	
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
		
		std::string name;
		name.assign(issuer);
		return name;
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
		//secretKey.assign(password);
		std::string result(password);
		return result;
	}
};
