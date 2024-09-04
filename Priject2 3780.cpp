#include <iostream>
#include <fstream>
#include <string>

#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#include <random>
#include <vector>
#include <chrono>



using namespace std;


string getHash(const string &input)
{
        EVP_MD_CTX* mdctx;
        const EVP_MD* md;
        unsigned char md_value[EVP_MAX_MD_SIZE];
        unsigned int md_len;

        OpenSSL_add_all_digests();  // Initialize OpenSSL's digest algorithms

        md = EVP_get_digestbyname("sha256");
        if (!md) {
            std::cerr << "SHA-256 not found." << std::endl;
            return "";
        }

        mdctx = EVP_MD_CTX_create();
        EVP_DigestInit_ex(mdctx, md, NULL);
        EVP_DigestUpdate(mdctx, input.c_str(), input.size());
        EVP_DigestFinal_ex(mdctx, md_value, &md_len);
        EVP_MD_CTX_destroy(mdctx);

        std::string hashed;
        for (unsigned int i = 0; i < md_len; i++) {
            char buf[3];
            snprintf(buf, sizeof(buf), "%02x", md_value[i]);
            hashed += buf;
        }

        return hashed;
    
}


string getUsername()
{
    string username;

    bool validInput = false;

    while (validInput == false)
    {
        validInput = true;

        cout << "Enter a username: ";
        getline(cin, username);

        for (char i : username)
        {
            if (!isalpha(i)) // if the username contains numbers, symbols bad input
            {
                validInput = false;
                cout << "username cannot contains numbers or symbols" << endl;
                break;
            }
        }

        if (username.length() > 10)
        {
            validInput = false;

            cout << " length cannot be more than 10" << endl;
            break;
        }

    }

    return username;
};

string getPassword(int maxLength)
{
    string password;

    bool validInput = false;

    while (validInput == false)
    {
        validInput = true;

        cout << "Enter a Password: ";
        getline(cin, password);

        for (char i : password)
        {
            if (!isalpha(i)) // if the username contains numbers, symbols bad input
            {
                validInput = false;
                cout << "Your password cannot contain any numbers or symbols" << endl;
                break;
            }
        }

        if (password.length() > maxLength)
        {
            validInput = false;

            cout << " length cannot be more than " + maxLength << endl;
            break;
        }

    }
    return password;
};

int getSalt() {
    unsigned char random_byte;

    if (RAND_bytes(&random_byte, 1) == 1) {
        return static_cast<int>(random_byte);
    }
    else {
        return -1;
    }
}


void createAccount(string username, string password)
{
    ofstream file;
    ofstream file2;
    ofstream file3;
    string line;
    string hash = getHash(password);
    int salt = getSalt();
    string hashSalt = getHash((password + to_string(salt)));

    file.open("plaintext_password.txt", ios::app);
    file2.open("hashed_password.txt", ios::app);
    file3.open("salted_password.txt", ios::app);


    if (file) {
        file << username + " " + password << "\n";
        cout << "Account created in plaintext_password.txt" << endl;
    }

    if (file2) {
        file2 << username + " " + hash << "\n";
        cout << "Account created in hashed_password.txt" << endl;
    }

    if (file3) {
        file3 << username + " " + to_string(salt) + " " + hashSalt << "\n";
        cout << "Account created in salted_password.txt" << endl;
    }


    file.close();
    file2.close();
    file3.close();
};

void verifyLogin(string username, string password, string fileName)
{
    ifstream file;
    string databaseUsername;
    string databasePassword;
    string databaseSalt;
    string line;
    string temp;
    int accountFound = 0;

    temp = password;

    if (fileName == "hashed_password.txt")
    {
        temp = getHash(password);
    }


    file.open(fileName);

    if (file)
    {
        while (getline(file, line))
        {

            if(fileName == "salted_password.txt")
            {   
                size_t spacePos1 = line.find(' ');
                if (spacePos1 != std::string::npos) {
                    databaseUsername = line.substr(0, spacePos1); // Extract the first word

                    size_t spacePos2 = line.find(' ', spacePos1 + 1);
                    if (spacePos2 != std::string::npos) {
                        databaseSalt = line.substr(spacePos1 + 1, spacePos2 - spacePos1 - 1); // Extract the second word

                        // Find the third word starting after the second space
                        size_t spacePos3 = line.find(' ', spacePos2 + 1);
                        if (spacePos3 != std::string::npos) {
                            databasePassword = line.substr(spacePos2 + 1, spacePos3 - spacePos2 - 1); // Extract the third word
                           
                        }
                        else {
                            databasePassword = line.substr(spacePos2 + 1); // There's no space after the second word
                        }
         
                    }
                   temp = getHash((password + databaseSalt));

                }
                
            }
            else {
                size_t spacePos = line.find(' ');
                if (spacePos != string::npos)
                {
                    databaseUsername = line.substr(0, spacePos); // Extract the first word
                    databasePassword = line.substr(spacePos + 1);

                }

            }

            if (databaseUsername == username)
            {
                accountFound = 1;

                if (databasePassword == temp)
                {
                    cout << "Successful match in: " + fileName << endl;
                    break;
                }

                else cout << "incorrect password: " + fileName << endl;
                break;

            }
        }
    }
    else
    {
        cout << "File does not exist";
    }

    if (!accountFound)
    {
        cout << "You do not have an account, sign up.";
       
    }
    file.close();
};

void  loginAccount(string username, string password)
{

    verifyLogin(username, password, "plaintext_password.txt");
    verifyLogin(username, password, "hashed_password.txt");
    verifyLogin(username, password, "salted_password.txt");

};

string generateRandomString(int length) {
    const string charset = "abcdefghijklmnopqrstuvwxyz";
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distribution(0, charset.length() - 1);
    string result;

    for (int i = 0; i < length; i++) {
        result += charset[distribution(gen)];
    }

    return result;
}


void generateSampleAccounts(int min, int max,int maxAccounts) {
    ofstream file1;
    ofstream file2;
    ofstream file3;

    random_device rd; // obtain a random number from hardware
    mt19937 gen(rd()); // seed the generator
    uniform_int_distribution<> distr(min, max); // define the range

    file1.open("plaintext_password.txt", ios::app);
    file2.open("hashed_password.txt", ios::app);
    file3.open("salted_password.txt", ios::app);

    for (int i = 0; i < maxAccounts; i++) {
        cout << "generating accounts: " << i << endl;

        int passwordSize = distr(gen);
        int usernameSize = distr(gen);

        string username = generateRandomString(usernameSize);
        string password = generateRandomString(passwordSize);

        if (file1) {
            file1 << username + " " + password << "\n";
            cout << "Account created in plaintext_password.txt" << endl;
        }

        if (file2) {
            string hash = getHash(password);
            file2 << username + " " + hash << "\n";
            cout << "Account created in hashed_password.txt" << endl;

        }

        if (file3) {
            int salt = getSalt();
            string hashSalt = getHash((password + to_string(salt)));
            file3 << username + " " + to_string(salt) + " " + hashSalt << "\n";
            cout << "Account created in salted_password.txt" << endl << endl;
        }

    }

    file1.close();
    file2.close();
    file3.close();
};


void crackPasswords(string password, string fileName,int salt) {
    
        ifstream file;
        string databaseUsername;
        string databasePassword;
        string databaseSalt;
        string line;
        string temp;
        int accountFound = 0;

        temp = password;

        if (fileName == "hashed_password.txt")
        {
            temp = getHash(password);
        }


        file.open(fileName);

        if (file)
        {
            while (getline(file, line))
            {

                if (fileName == "salted_password.txt")
                {
                    size_t spacePos1 = line.find(' ');
                    if (spacePos1 != std::string::npos) {
                        databaseUsername = line.substr(0, spacePos1); // Extract the first word

                        size_t spacePos2 = line.find(' ', spacePos1 + 1);
                        if (spacePos2 != std::string::npos) {
                            databaseSalt = line.substr(spacePos1 + 1, spacePos2 - spacePos1 - 1); // Extract the second word

                            // Find the third word starting after the second space
                            size_t spacePos3 = line.find(' ', spacePos2 + 1);
                            if (spacePos3 != std::string::npos) {
                                databasePassword = line.substr(spacePos2 + 1, spacePos3 - spacePos2 - 1); // Extract the third word

                            }
                            else {
                                databasePassword = line.substr(spacePos2 + 1); // There's no space after the second word
                            }

                        }
                        temp = getHash((password + (string)to_string(salt)));
                        

                    }

                }
                else {
                    size_t spacePos = line.find(' ');
                    if (spacePos != string::npos)
                    {
                        databaseUsername = line.substr(0, spacePos); // Extract the first word
                        databasePassword = line.substr(spacePos + 1);

                    }

                }

                if (databasePassword == temp)
                {
                    if (fileName == "hashed_password.txt") {
                        cout << "Successful match in: " + fileName << endl;
                        cout << "User Name: " << databaseUsername << endl << "Password: " << password << endl << "Hashed: " << databasePassword << endl;
                        break;
                    }

                    if (fileName == "salted_password.txt") {
                        cout << "Successful match in: " + fileName << endl;
                        cout << "User Name: " << databaseUsername << endl << "Password: " << password << endl << "Salt: " << salt << endl << "Salted + Hashed: " << databasePassword << endl;
                        break;
                    }

                    else {
                        cout << "Successful match in: " + fileName << endl;
                        cout << "User Name: " << databaseUsername << endl << "Password: " << databasePassword << endl;
                        break;
                    }


                }
            }
        }
        else
        {
            cout << "File does not exist";
        }

        file.close();




    
};

int passwordMin() {
    int min;
    string input;

    while (true) {
        cout << "Enter minimum password size: ";
        getline(cin, input);
        if (cin.fail() || input.find_first_not_of("0123456789") != string::npos) {
            cout << "Invalid input. Please enter a valid integer." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            min = stoi(input);
            break;
        }
    }
    return min;
}
int passwordMax() {
    int max;
    string input;

    while (true) {
        cout << "Enter maximum password size: ";
        getline(cin, input);
        if (cin.fail() || input.find_first_not_of("0123456789") != string::npos) {
            cout << "Invalid input. Please enter a valid integer." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            max = stoi(input);
            break;
        }

    }
    return max;

}

int accountsMax() {
    int max;
    string input;

    while (true) {
        cout << "Enter the accounts to generate: ";
        getline(cin, input);
        if (cin.fail() || input.find_first_not_of("0123456789") != string::npos) {
            cout << "Invalid input. Please enter a valid integer." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            max = stoi(input);
            break;
        }

    }
    return max;

}

void generatePasswordsHelper(vector<string>& strings, const string& chars, string& combination, int index, int length) {
    if (index == length) {
        strings.push_back(combination);
        return;
    }

    for (char c : chars) {
        combination[index] = c;
        generatePasswordsHelper(strings, chars, combination, index + 1, length);
    }
}


void generatePasswords(int min, int max, string fileName) {
    bool invalidInput = true;
    int salt=1;
    int counter = 1;
  

    cout << endl << "Generating passwords" << endl << endl;
    vector<string> strings;
    
    for (int length = min; length <= max; length++) {
        // Generate all possible combinations of characters within the given length
        string chars = "abcdefghijklmnopqrstuvwxyz";
        string combination(length, ' ');
        generatePasswordsHelper(strings, chars, combination, 0, length);
    }

    auto startTime = std::chrono::high_resolution_clock::now();
    cout << "attempting to crack " << fileName << endl << endl;

    if (fileName == "salted_password.txt") {
        for (const string& str : strings) {
            for (int i = 1; i < 257; i++)
            {
                crackPasswords(str, fileName, salt);
                salt++;
            }
            salt = 1;
            counter++;
            if (counter % 10 == 0) {
                cout << "******************************" << endl;
                cout << "Attempting to crack: " << counter << " passwords tried" << endl;
                auto endTime = chrono::high_resolution_clock::now();
                chrono::duration<double> elapsed = endTime - startTime;
                cout << "Elapsed time: " << elapsed.count() << " seconds" << endl;
                cout << "******************************" << endl;
            }

        }
    }
    else {
        for (const string& str : strings) {
            crackPasswords(str, fileName, salt);

            counter++;

            if (counter % (1000 ) == 0) {
                cout << "******************************" << endl;
                cout << "Attempting to crack: " << counter << " passwords tried" << endl;
                auto endTime = chrono::high_resolution_clock::now();
                chrono::duration<double> elapsed = endTime - startTime;
                cout << "Elapsed time: " << elapsed.count() << " seconds" << endl;
                cout << "******************************" << endl;

            }
        }
    }
    cout << "******************************" << endl;
    cout << fileName << " Cracking Completed" << endl;
    auto endTime = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = endTime - startTime;
    cout << "Elapsed time: " << elapsed.count() << " seconds" << endl;
    cout << "******************************" << endl;
}

int main(int argc, char const* argv[])
{
    bool inMenu = true;
    string choice;
    string choice2;
    cout << "Hello, enter the corresponding value followed by enter to do an operation." << endl;

    while (inMenu)
    {
        cout <<endl << "Create Account: 1" << endl;
        cout << "Login Account: 2" << endl;
        cout << "Generate Accounts: 3" << endl;
        cout << "Crack Passwords: 4" << endl;
        cout << "Exit: 5" << endl;

        cout << endl << "Choice: ";

        getline(cin, choice);

        if (choice == "1")
        {
            string username = getUsername();
            string password = getPassword(10);
            createAccount(username, password);

        }
        else if (choice == "2")
        {
            string username = getUsername();
            string password = getPassword(10);

            loginAccount(username, password);
            inMenu = false;
            break;
        }
        else if (choice == "3")
        {
            generateSampleAccounts(passwordMin(), passwordMax(), accountsMax()); // min, max, number of accounts
        }
        else if (choice == "4")
        {
            cout << endl << "Select which file to crack" << endl;
            cout << "plaintext: 1" << endl;
            cout << "hashed: 2" << endl;
            cout << "salted_hashed: 3" << endl;
            cout << endl << "Choice: ";
            while (true)
            {
                getline(cin, choice2);
                if (choice2 == "1")
                {
                    generatePasswords(passwordMin(), passwordMax(), "plaintext_password.txt");
                    break;
                }
                else if (choice2 == "2")
                {
                    generatePasswords(passwordMin(), passwordMax(), "hashed_password.txt");
                    break;
                }
                else if (choice2 == "3")
                {
                    generatePasswords(passwordMin(), passwordMax(), "salted_password.txt");
                    break;
                }
                else {
                    cin.clear();
                    cout << "Invalid inout try again";
                }
           
            }
        }
        else if (choice == "5")
        {
            exit(0);
        }

        else
        {
            cout << "Error: Invalid input. Try Again.";
        }
    }



    

    return 0;
}
