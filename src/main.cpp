#include <iostream>
#include <vector>
#include <string>

#include "datasets.h"
#include "tokenizer.h"

#define DEFAULT_DICTIONARY "dictionaries/2^16.tok"

struct ParsedArgs
{
        int command;
        std::vector<std::string> args;
};

ParsedArgs parseArgs(int argc, char** argv)
{
        ParsedArgs parsedArgs;
        parsedArgs.command = -1;
        parsedArgs.args = {};
        if (argc < 2)
        {
                std::cerr << "Wrong number of arguments.\n";
                std::cerr << " Run " << argv[0] << " -h for help.\n";
                return parsedArgs;
        }

        std::vector<std::string> args;

        for (int i = 1; i < argc; i++)
        {
                args.push_back(argv[i]);
        }

        parsedArgs.args = args;
        if (args[0] == "-h" || args[0] == "--help")
        {
                std::cout << "Usage: " << argv[0] << " [OPTION]...\n";
                std::cout << "\n";
                std::cout << "Options:\n";
                std::cout << "  -h, --help\n\tDisplay this help and exit.\n\n";
                std::cout << "  -p <target>, --prepare <target>\n\tPrepare <target> review data.\n\t\tAvailable targets: amazon. !DATASET HAS TO BE IN `resources/data.csv`!\n\n";
                std::cout << "  -t <dataset_path> <output_path>, --tokenize <dataset_path> <output_path>\n\tTokenize <dataset_path> and save to <output_path>.\n\n";
                std::cout << "  -i <message>, --tokenize-input <message>\n\tTokenize <message> with default dictionary.\n\n";
                std::cout << "  -i <message> <dictionary_path>, --tokenize-input <message> <dictionary_path>\n\tTokenize <message> with <dictionary_path>.\n\n";
                std::cout << "  -d <token1> <token2> ... <tokenN>, --detokenize <token1> <token2> ... <tokenN>\n\tDetokenize <token1> <token2> ... <tokenN> with default dictionary.\n\n";
                std::cout << "  -d <dictionary_path> <token1> <token2> ... <tokenN>, --detokenize <dictionary_path> <token1> <token2> ... <tokenN>\n\tDetokenize <token1> <token2> ... <tokenN> with <dictionary_path>.\n";
                parsedArgs.command = 0;
                return parsedArgs;
        }
        else if (args[0] == "-p" || args[0] == "--prepare")
        {
                if (args.size() < 2)
                {
                        std::cerr << "Wrong number of arguments.\n";
                        std::cerr << " Run " << argv[0] << " -h for help.\n";
                        return parsedArgs;
                }
                if (args[1] == "amazon")
                {
                        parsedArgs.command = 1;
                }
                else
                {
                        std::cerr << "Unknown target.\n";
                        return parsedArgs;
                }
        }
        else if (args[0] == "-t" || args[0] == "--tokenize")
        {
                if (args.size() < 3)
                {
                        std::cerr << "Wrong number of arguments.\n";
                        std::cerr << " Run " << argv[0] << " -h for help.\n";
                        return parsedArgs;
                }
                parsedArgs.command = 2;
        }
        else if (args[0] == "-i" || args[0] == "--tokenize-input")
        {
                if (args.size() < 2)
                {
                        std::cerr << "Wrong number of arguments.\n";
                        std::cerr << " Run " << argv[0] << " -h for help.\n";
                        return parsedArgs;
                }
                parsedArgs.command = 3;
        }
        else if (args[0] == "-d" || args[0] == "--detokenize")
        {
                if (args.size() < 2)
                {
                        std::cerr << "Wrong number of arguments.\n";
                        std::cerr << " Run " << argv[0] << " -h for help.\n";
                        return parsedArgs;
                }
                parsedArgs.command = 4;
        }
        else
        {
                std::cerr << "Unknown command.\n";
                return parsedArgs;
        }

        return parsedArgs;
}

int main(int argc, char** argv)
{
        ParsedArgs parsedArgs = parseArgs(argc, argv);
        TokenDictionary tokenDictionary;

        switch (parsedArgs.command)
        {
        case 0:
                break;
        case 1:
                prepareAmazonReviewData("resources/data.csv", "resources/data_prepared.txt");
                break;
        case 2:
                tokenizeDataset(parsedArgs.args[1], parsedArgs.args[2]);
                break;
        case 3:
                if (parsedArgs.args.size() == 2)
                {
                        tokenDictionary = loadTokenDictionary(DEFAULT_DICTIONARY);
                        std::vector<uint16_t> tokens = tokenizeString(parsedArgs.args[1], tokenDictionary);
                        for (uint16_t token : tokens)
                        {
                                std::cout << token << " ";
                        }
                        std::cout << "\n";
                }
                else if (parsedArgs.args.size() == 3)
                {
                        tokenDictionary = loadTokenDictionary(parsedArgs.args[2]);
                        std::vector<uint16_t> tokens = tokenizeString(parsedArgs.args[1], tokenDictionary);
                        for (uint16_t token : tokens)
                        {
                                std::cout << token << " ";
                        }
                        std::cout << "\n";
                }
                break;
        case 4:
                size_t start = 1;
                try
                {
                        tokenDictionary = loadTokenDictionary(parsedArgs.args[1]);
                        start = 2;
                }
                catch (...)
                {
                        tokenDictionary = loadTokenDictionary(DEFAULT_DICTIONARY);
                }
                std::vector<uint16_t> tokens;
                for (int i = start; i < parsedArgs.args.size(); i++)
                {
                        tokens.push_back(std::stoi(parsedArgs.args[i]));
                }
                std::string detokenized = detokenizeString(tokens, tokenDictionary);
                std::cout << detokenized << "\n";
                break;
        }

        return 0;
}
