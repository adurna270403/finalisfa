#include <iostream>
#include <string>
#include <memory>
#include <array>
#include <cstdio>
#include <stdexcept>

// Function to execute Python script and capture output
std::string executePythonScript(const std::string& prompt) {
    // Escape quotes in the prompt to avoid command injection
    std::string escapedPrompt = prompt;
    // Replace double quotes with escaped double quotes
    size_t pos = 0;
    while ((pos = escapedPrompt.find("\"", pos)) != std::string::npos) {
        escapedPrompt.replace(pos, 1, "\\\"");
        pos += 2;
    }
    
    // Create the command to execute the Python script with the prompt as an argument
    #ifdef _WIN32
    std::string command = "python test.py \"" + escapedPrompt + "\"";
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(command.c_str(), "r"), _pclose);
    #else
    std::string command = "python3 call_api.py \"" + escapedPrompt + "\"";
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    #endif
    
    if (!pipe) {
        throw std::runtime_error("Failed to execute Python script");
    }
    
    // Read the output from the Python script
    std::array<char, 4096> buffer;
    std::string result;
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    
    return result;
}

int main() {
    try {
        std::cout << "==== Gemini API Interface ====" << std::endl;
        std::cout << "Type your prompt (or 'exit' to quit):" << std::endl;
        
        std::string prompt;
        while (true) {
            std::cout << "\n> ";
            std::getline(std::cin, prompt);
            
            if (prompt == "exit" || prompt == "quit") {
                break;
            }
            
            if (prompt.empty()) {
                continue;
            }
            
            std::cout << "\nSending to Gemini API..." << std::endl;
            
            // Execute Python script and get response
            std::string response = executePythonScript(prompt);
            
            std::cout << "\n--- Response from Gemini ---" << std::endl;
            std::cout << response << std::endl;
            std::cout << "----------------------------" << std::endl;
        }
        
        std::cout << "Goodbye!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}