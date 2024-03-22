

class SaveSystem {

	private:
	
	std::string saveFilePath = "/etc/totpcmd/keys";
	//std::string saveFilePath = "/home/dev/devGithub/totpcmd/build/keys";
		
	public:
	
	std::fstream file;
	
	SaveSystem() {};
	
	int openFile() {
		file.open(saveFilePath, std::ios::in | std::ios::out | std::ios::app);
		//file.open(saveFilePath, std::ios::in | std::ios::out);
		if(!file.is_open()) {
			std::cerr << "Could not open key file: " << saveFilePath << std::endl;
			return 1;
		} else {
			return 0;
		}
	}
	
	int closeFile() {
		if(!file.is_open()) {
			std::cerr << "Key file already closed" << std::endl;
			return 1;
		} else {
			//std::cout << "Closing file" << std::endl;
			file.close();
		}
		return 0;
	}
	
	void testWrite(std::string input) {
		file << input << std::endl;
	}
	
	void addEntry(Entry *entry) {
		//printw("Trying to add entry here\n");
		//std::cout << "Adding entry to file" << std::endl;
		if(file.is_open()) {
			//printw("entered write function here\n");
			//file.seekg(0);
			//std::cout << "Name: " << entry->getEntry().name << std::endl;
			//std::cout << "Token: " << entry->getEntry().token << std::endl;
			
			std::string name = entry->getEntry().name;
			std::string token = entry->getEntry().token;
			
			printw(name.c_str());
			printw(token.c_str());
			
			file << name.c_str() << "," << token.c_str() << std::endl;
			//std::cout << entry->getEntry().name << "," << entry->getEntry().token << std::endl;
			//std::cout << "Added file" << std::endl;
			//std::this_thread::sleep_for(std::chrono::seconds(5));
		} else {
			printw("File not open, cannot write\n");
			std::cerr << "File not open, cannot write" << std::endl;
		}
	}
	
	void removeEntry() {
		
	}
	
	std::vector<std::string> readEntry() {
		std::vector<std::string> lines;
		
		if(file.is_open()) {
			std::string line;
			while(std::getline(file, line)) {
				if(!line.empty()) {
					lines.push_back(line);
				}
			}
		} else {
			std::cerr << "Failed to read file, file not open: " << saveFilePath << std::endl;
		}
		
		//file.seekg(0);
		file.clear();
		
		return lines;
	}
	
	std::vector<std::string> splitString(const std::string& s, char delimiter) {
		std::vector<std::string> tokens;
		std::string token;
		std::istringstream tokenStream(s);
		while (std::getline(tokenStream, token, delimiter)) {
			tokens.push_back(token);
		}
		return tokens;
	}
	
};
