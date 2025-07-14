#include <iostream>
#include "headers/gameplay.hpp"

/*I'm gonna try to make the best tick tack toe i can with the little i know about c++
(among that knowledge, is a lack of graphic libraries, so terminal by now)

I wanna do it with AI (Worst case only with random moves), file saved stadistics,
replays and the best graphics a terminal can give me haha*/

//620 Lines, left to do: Stadistics

int main()
{
    srand(time(0)); //Seed for random numbers

    while (true)
    {
        clearConsole();

        std::cout << "Welcome to THE BEST TICK TACK TOE THAT A BROKE STUDENT CAN MAKE" << std::endl << std::endl
            << "1 - Start Game \n2 - Stadistics \n3 - Credits \n0 - Exit\n\n";

        short input = getNumber<short>(3, "Choose an option: ");

        switch (input)
        {
        case 1:
            preGame();
            break;
        case 2:
            //Stadistics menu
            break;
        case 3:
            clearConsole();
            std::cout << "Made entirely by Jaludex (Viva Venezuela Carajo).\n(With lots of help of DeepSeek, not in coding, but learning concepts of c++)\n\n";
            getNumber<short>(0, "0 - Go back: ");
            break;
        case 0:
            clearConsole();
            std::cout << "Thanks you so much for playing my game.\nTips on: I haven't setted up my paypal yet\n\n";
            std::this_thread::sleep_for(std::chrono::seconds(3));
            clearConsole();
            return 0;
        }
    }
    
    return 0;
}