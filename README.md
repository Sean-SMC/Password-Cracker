# CS 3780 - Project 2

**Author:** Sean Clewis  
**Course:** CS 3780  
**Project:** Cryptographic Hash Functions & Pseudo Random Number Generator  

## Project Overview
This project focuses on implementing cryptographic hash functions and a pseudo-random number generator using the OpenSSL library in C++. The project includes tasks such as creating accounts, logging into accounts, generating accounts, and brute-forcing passwords using varying password lengths.

The project was developed in Visual Studio, with the OpenSSL library installed via vcpkg. It demonstrates both salted and non-salted password hashing mechanisms, allowing for comparisons between security and performance when brute-forcing passwords.

## Features
- **Account Creation**: Create new accounts with usernames and passwords stored in text files.
- **Account Login**: Log into existing accounts by verifying passwords.
- **Account Generation**: Generate multiple accounts and store them in text files.
- **Password Cracking**: Brute-force passwords with specified lengths, including both hashed and salted variations.

## File Structure
- **Source Code**: The main program is written in C++.
- **Text Files**: The project includes three pre-populated text files with over 1,000 accounts of varying lengths.
  - Format: `username`, `salt` (if salted), `password`
  - Non-salted files do not include the salt column.
- **Images**: A couple of images are included in the project for demonstration purposes.

## How to Use
1. **Running the Program**: 
   - Upon running the program, you will be prompted with a set of on-screen commands.
   - Use the corresponding number to select an operation (e.g., logging in, creating accounts, generating accounts, or cracking passwords).
2. **Cracking Accounts**:
   - When cracking passwords, you can specify the minimum and maximum password lengths to attempt.

## Performance Results
- **Password Cracking (Non-Salted)**:
  - Password lengths of 4 or lower are quickly cracked.
  - For lengths of 5 or more, the cracking process becomes time-consuming due to memory and hardware limitations.
  - The program logs the time taken and the number of passwords checked.
  - Approximate speed: 15,000 passwords in 90 seconds for a password length of 5.

- **Password Cracking (Salted)**:
  - The salted version is significantly slower, processing about 20 passwords in 90 seconds for a length of 5.
  - Using a smaller password length (e.g., 3) is feasible but still much slower compared to the non-salted version.
  - Approximate speed: 0.2 passwords per second for a password length of 5.

## Insights
- The salted version offers significantly more security than the non-salted version, albeit at the cost of performance.
- For the salted version, a password length of 10 characters would result in an infeasible brute-force time (approximately 8.155 x 10^9 days).

This highlights the importance of using strong passwords and secure password storage practices, such as salting and hashing.

## Installation
1. **Install OpenSSL**: Ensure you have OpenSSL installed using vcpkg.
2. **Compile and Run**: Compile the project in Visual Studio and run the executable.

## Future Improvements
- **Optimize Memory Usage**: Export generated passwords to a file rather than storing all combinations in memory to handle longer password lengths.
- **Performance Enhancements**: Improve the efficiency of the password generation and cracking process, particularly for the salted version.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
