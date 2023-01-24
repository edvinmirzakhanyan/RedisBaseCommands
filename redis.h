#ifndef REDIS_H
#define REDIS_H

#include <iostream>
#include <vector>
#include <map>
#include <fstream>

enum Commands {
    Get = 0,
    Set,
    LPush,
    Rpush,
    HSet,
    HGet,
    LLen,

    // Command sizes
    GetSize = 2,
    SetSize = 3,
    LPushSize = 3,
    RPushSize = 3,
    HSetSize = 4,
    HGetSize = 3,
    LLenSize = 2,
};

class Redis {

public:
    Redis ();

public:
    void run();
private:
    std::string toLower(const std::string&);
    std::vector<std::string> split(const std::string& str);
    void dataSave();
    void hDataSave();
    void getCommand();

private:            
    void _set();
    std::string _get();
    void _lpush();
    void _rpush();
    int _llen();
    void _hset();
    std::string _hget();

private:
    static std::map<std::string, int> command;
    std::vector<std::string> splitedCommand;

    std::vector<std::string> splitedData;
    
    static const std::string fileName;
    static const std::string hFileName;
    
    std::map<std::string, std::string> key_value;
};

const std::string Redis::fileName = "data.txt";
const std::string Redis::hFileName = "hdata.txt";

std::map<std::string, int> Redis::command = {
    { "get", Commands::Get },
    { "set", Commands::Set },
    { "lpush", Commands::LPush },
    { "rpush", Commands::Rpush },
    { "hset", Commands::HSet },
    { "hget", Commands::HGet },
    { "llen", Commands::LLen },
};

Redis::Redis() {}

std::string Redis::toLower(const std::string& line) {
    std::string tmp;
    int size = line.length();

    for (int i = 0; i < size; ++i) {
        if (line[i] > 'A' && line[i] < 'Z') {
            tmp += line[i] + 32;
            continue;
        }
        tmp += line[i];
    }
    return tmp;
}

std::vector<std::string> Redis::split(const std::string& str) {
    int length = str.length();
    std::vector<std::string> arr;
    std::string word;

    for (int i = 0; i < length; ++i) {
        if (str[i] != ' ') { word += str[i]; continue; }
        if (word.length()) { arr.push_back(word); word = "";}
    }
    if (word.length()) { arr.push_back(word); }

    return arr;
}

void Redis::dataSave() {
    std::string saveLine;
    int size = splitedCommand.size();

    for (int i = 1; i < size; ++i) {
        saveLine += splitedCommand[i] + ' ';
    }

    std::ofstream fout(fileName, std::ios::app);

    fout << saveLine + '\n';
    fout.close();
}

void Redis::hDataSave() {
    std::string saveLine;
    int size = splitedData.size();

    for (int i = 0; i < size; ++i) {
        saveLine += splitedData[i] + ' ';
    }

    std::ofstream fout(hFileName, std::ios::app);

    fout << saveLine + '\n';
    fout.close();
}

void Redis::run() {
    std::string input;
    std::getline(std::cin, input);

    splitedCommand = split(input);

    try {
        getCommand();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void Redis::getCommand() {

    if (!splitedCommand.size()) { 
        throw std::invalid_argument("Wrong command");
    }

    // splitedCommand[0] is current command
    std::string currentCommand = toLower(splitedCommand[0]);

    if (!command.contains(currentCommand)) {
        throw std::invalid_argument("Wrong command");
    }

    if (command[currentCommand] == Commands::Set) {
       try { 
            _set();
       }
       catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
       }
    }
    else if (command[currentCommand] == Commands::Get) {
        try {
             std::cout << _get();
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        } 
    }
    else if (command[currentCommand] == Commands::LPush) {
        try {
             _lpush();
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        } 
    }
    else if (command[currentCommand] == Commands::Rpush) {
        try {
            _rpush();
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }     
    }
    else if (command[currentCommand] == Commands::HSet) {
        try {
            _hset();
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }     
    }
    else if (command[currentCommand] == Commands::HGet) {
        try {
            std::cout << _hget();
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }   
    }
    else if (command[currentCommand] == Commands::LLen) {
        try {
            std::cout << _llen();
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }     
    }
}

void Redis::_set() {
    if (splitedCommand.size() != Commands::SetSize) {
        throw std::invalid_argument("Wrong command");
    }

    dataSave();
}

std::string Redis::_get() {

    if (splitedCommand.size() != Commands::GetSize) {
        return "Wrong command"; 
    }
    std::ifstream fin(fileName);
    std::string key;
    std::string value;

    // splitedCommand[1] is inputed key
    std::string inputedKey = splitedCommand[1];
    bool dataFound = false;

    while (!fin.eof()) {
        fin >> key;

        if (key == inputedKey) {
            dataFound = true;
            std::getline(fin, value);
            key_value[key] = value;
            continue;
        }
        std::getline(fin, value);
    }

    fin.close();
    if (dataFound) {
        return key_value[inputedKey].substr(1); // here substr is for ignoring white space
    }
    throw std::invalid_argument("nil");
}

void Redis::_lpush() {
    if (splitedCommand.size() != Commands::LPushSize) {
       throw std::invalid_argument("Wrong command");
    }
    
    std::ifstream fin(fileName);
    std::string key;
    std::string value;

    // splitedCommand[1] is inputed key
    std::string inputedKey = splitedCommand[1];
    bool dataFound = false;

    while (!fin.eof()) {
        fin >> key;

        if (key == inputedKey) {
            dataFound = true;
            std::getline(fin, value);
            key_value[key] = value;
            continue;
        }
        std::getline(fin, value);
    }
    fin.close();

    if (dataFound) {        
        // splitedCommand[2] lpush value
        key_value[inputedKey] = splitedCommand[2] + key_value[inputedKey];

        std::ofstream fout(fileName, std::ios::app);
        fout << inputedKey + ' ' + key_value[inputedKey] + '\n';
        fout.close();
    }
    else {
        throw std::invalid_argument("nil");
    }
}

void Redis::_rpush() {
    if (splitedCommand.size() != Commands::RPushSize) {
       throw std::invalid_argument("Wrong command");
    }
    
    std::ifstream fin(fileName);
    std::string key;
    std::string value;

    // splitedCommand[1] is inputed key
    std::string inputedKey = splitedCommand[1];
    bool dataFound = false;

    while (!fin.eof()) {
        fin >> key;

        if (key == inputedKey) {
            dataFound = true;
            std::getline(fin, value);
            key_value[key] = value;
            continue;
        }
        std::getline(fin, value);
    }
    fin.close();

    if (dataFound) {        
        // splitedCommand[2] is rpush value
        key_value[inputedKey] = key_value[inputedKey] + splitedCommand[2];

        std::ofstream fout(fileName, std::ios::app);
        fout << inputedKey + key_value[inputedKey] + ' ' + '\n';
        fout.close();
    }
    else {
        throw std::invalid_argument("nil");
    }
}

int Redis::_llen() {
    if (splitedCommand.size() != Commands::LLenSize) {
       throw std::invalid_argument("Wrong command");
    }
    
    std::ifstream fin(fileName);
    std::string key;
    std::string value;

    // splitedCommand[1] is inputed key
    std::string inputedKey = splitedCommand[1];
    bool dataFound = false;

    while (!fin.eof()) {
        fin >> key;

        if (key == inputedKey) {
            dataFound = true;
            std::getline(fin, value);
            key_value[key] = value;
            continue;
        }
        std::getline(fin, value);
    }
    fin.close();


    if(dataFound) {
        return split(key_value[inputedKey]).size();
    }
    throw std::invalid_argument("nil");
}

void Redis::_hset() {
    if (splitedCommand.size() != Commands::HSetSize) {
       throw std::invalid_argument("Wrong command");
    }

    std::ifstream fin(hFileName);
    std::string key;
    std::string value;

    // splitedCommand[1] is inputed key
    std::string inputedKey = splitedCommand[1];
    bool dataFound = false;

    while (!fin.eof()) {
        fin >> key;

        if (key == inputedKey) {
            dataFound = true;
            std::getline(fin, value);
            key_value[key] = value;
            continue;
        }
        std::getline(fin, value);
    }
    fin.close();
    
    if (dataFound) {
        splitedData = split(key_value[inputedKey]);
        int size = splitedData.size();

        for (int i = 0; i < size; i += 2) {
            // splitedCommand[2] is key which value will be setted
            if (splitedData[i] == splitedCommand[2]) {

                // splitedCommand[3] is value that will be setted
                splitedData[i + 1] = splitedCommand[3];
                splitedData.insert(splitedData.begin(), inputedKey);
                hDataSave();
                return;
            }
        }
        splitedData.insert(splitedData.begin(), inputedKey);
        splitedData.insert(splitedData.end(), { splitedCommand[2], splitedCommand[3]});
        hDataSave();
        return;
    }
    splitedData.insert(splitedData.end(), { splitedCommand[1],      // key
                                            splitedCommand[2],      // value
                                            splitedCommand[3]});    // data
    hDataSave();
}

std::string Redis::_hget() {
    if (splitedCommand.size() != Commands::HGetSize) {
       throw std::invalid_argument("Wrong command");
    }

    std::ifstream fin(hFileName);
    std::string key;
    std::string value;

    // splitedCommand[1] is inputed key
    std::string inputedKey = splitedCommand[1];
    bool dataFound = false;

    while (!fin.eof()) {
        fin >> key;

        if (key == inputedKey) {
            dataFound = true;
            std::getline(fin, value);
            key_value[key] = value;
            continue;
        }
        std::getline(fin, value);
    }
    fin.close();
    
    if (dataFound) {
        splitedData = split(key_value[inputedKey]);
        int size = splitedData.size();

        for (int i = 0; i < size; i += 2) {
            // splitedCommand[2] is the key which value we are searching
            if (splitedData[i] == splitedCommand[2]) {
                return splitedData[i + 1]; // value exists
            }
        }
    }
    throw std::invalid_argument("nil");
}

#endif // REDIS_H

