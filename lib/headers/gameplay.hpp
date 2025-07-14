#ifndef GAMEPLAY_HPP
#define GAMEPLAY_HPP

#include "data.hpp"
#include <thread>
#include <random>

//Abstract
class Player
{
    protected:
        char letter;
    public:
        virtual void hisTurn(GameBoard& actualBoard) = 0;
        Player(char l):letter(l){}
};

//Real
class RealPlayer : public Player
{
    public:
        virtual void hisTurn(GameBoard& actualBoard) override;
        RealPlayer(char l):Player(l){}
};

//Random moves
class CPUEasy : public Player
{
    public:
        virtual void hisTurn(GameBoard& actualBoard) override;
        CPUEasy(char l):Player(l){}
};


//It will try and play on the square who would make someone wins, winning or blocking the opponent, 
//then goes to center, corner, side (those last two by random)
class CPUHard : public Player
{
    public:
        virtual void hisTurn(GameBoard& actualBoard) override;
        CPUHard(char l):Player(l){}
};

void gameplay(Player& P1, Player& P2, Gamelog& this_game);
void preGame();



#endif