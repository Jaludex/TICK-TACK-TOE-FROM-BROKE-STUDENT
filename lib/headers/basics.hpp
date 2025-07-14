#ifndef BASICS_HPP
#define BASICS_HPP

#include <iostream>
#include <string>
#include <limits>

template <typename T>
T getNumber(int limit, std::string message) 
{
    T input;
    
    while(true) {
        std::cout << message;
        if(!(std::cin >> input)) 
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
        } 
        else if(input < 0 || input > limit) 
        {
            if (input != 0) //with this, i can use the same function for just entering input to continue
            {
                std::cout << "Input must be between 0 and " << limit << ".\n";
            }
        } 
        else 
        {
            break;
        }
    }
    
    return input;
}

template <typename T>
T getNumber(int low_limit, int up_limit, std::string message) 
{
    T input;
    
    while(true) {
        std::cout << message;
        if(!(std::cin >> input)) 
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
        } else if(input < low_limit || input > up_limit) 
        {
                std::cout << "Input must be between "<< low_limit << " and " << up_limit << ".\n";
        } 
        else 
        {
            break;
        }
    }
    
    return input;
}

void clearConsole();

#endif