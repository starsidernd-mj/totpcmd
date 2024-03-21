

class SaveSystem {

	private:
	
	std::string saveFilePath = "/etc/totpcmd/keys";
		
	public:
	
	std::fstream file;
	
	SaveSystem() {};
	
	int openFile() {
		file.open(saveFilePath, std::ios::in | std::ios::out);
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
			file.close();
		}
		return 0;
	}
	
	void addEntry() {
		
	}
	
	void removeEntry() {
		
	}
	
	std::vector<std::string> readEntry() {
		std::vector<std::string> lines;
		
		if(file.is_open()) {
			std::string line;
			while(std::getline(file, line)) {
				lines.push_back(line);
			}
		} else {
			std::cerr << "Failed to read file, file not open: " << saveFilePath << std::endl;
		}
		
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
