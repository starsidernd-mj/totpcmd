#include <iostream>
#include <iomanip>
#include <cstring>
#include <ctime>
#include <cmath>
#include <openssl/hmac.h>
#include <openssl/sha.h>

std::string generateTOTP(const std::string& key, size_t digits = 6) {
	std::cout << "Generating TOTP" << std::endl;
	// Replace 'your_time_step' with the time step (usually 30 seconds)
    uint64_t timeStep = 30;
	
	// Get current time as seconds since the Unix epoch
    uint64_t currentTime = std::time(nullptr);
	
    // Calculate the counter based on the current time and time step
    uint64_t counter = currentTime / timeStep;

    // Prepare data for HMAC
    std::string data(reinterpret_cast<char*>(&counter), sizeof(counter));

    // Compute HMAC-SHA1
    unsigned char hash[SHA_DIGEST_LENGTH];
    HMAC(EVP_sha1(), key.c_str(), key.size(), reinterpret_cast<unsigned char*>(data.data()), data.size(), hash, nullptr);

    // Get last nibble of the digest to determine the starting offset
    size_t offset = hash[SHA_DIGEST_LENGTH - 1] & 0xF;

    // Extract 4 bytes from the hash, starting from the determined offset
    uint32_t truncatedHash = (hash[offset] & 0x7F) << 24 | (hash[offset + 1] & 0xFF) << 16 | (hash[offset + 2] & 0xFF) << 8 | (hash[offset + 3] & 0xFF);

    // Apply modulo to get a number that fits in 'digits' decimal places
    truncatedHash %= static_cast<uint32_t>(std::pow(10, digits));

    // Convert to string with leading zeros if necessary
    std::ostringstream result;
    result << std::setw(digits) << std::setfill('0') << truncatedHash;
    return result.str();
}
