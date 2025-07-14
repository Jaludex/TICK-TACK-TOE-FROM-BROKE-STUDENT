#ifndef DATA_HPP
#define DATA_HPP

#include "basics.hpp"
#include <chrono>
#include <vector>
#include <ctime>
#include <array>


class GameBoard
{
    private:
        std::array<char, 9> board; //My initial plan was to use a matrix, but this seems simpler, the only
                                   //disadvantage is that to check for a winner, i've got to do it manually

    public:
        std::array<char, 9> getBoard() { return board; }
        char getAt(short pos) const { return this->board.at(pos); }

        bool isFull() const
        {
            for (size_t i = 0; i < 9; i++)
            {
                if (this->board.at(i) == ' ') { return false; }
            }
            
            return true;
        }

        bool playAt(char icon, int pos)
        {
            if (board.at(pos) == ' ')
            {
                this->board.at(pos) = icon;
                return true;
            }
            
            return false;
        }

        void print()
        {
            std::cout << " " << board.at(0) << " | " << board.at(1) << " | " << board.at(2) << " \n"
                      << " " << board.at(3) << " | " << board.at(4) << " | " << board.at(5) << " \n"
                      << " " << board.at(6) << " | " << board.at(7) << " | " << board.at(8) << " \n\n";
        }

        short checkWinner() //0 for no winner yet, 1 for P1, 2 for P2, -1 for tie.
        {
            size_t i;    
        
            for (i = 0; i < 3; i++) //Vertical winner check
            {
                if ((board.at(i) != ' ') && 
                    ((board.at(i) == board.at(i + 3)) && 
                    (board.at(i) == board.at(i + 6)))) 
                {
                    if (board.at(i) == 'X') { return 1; }
                    else if (board.at(i) == 'O') { return 2; }
                }
            }
        
            for (i = 0; i <= 6; i += 3) //Horizontal winner check
            {
                if ((board.at(i) != ' ') && 
                    ((board.at(i) == board.at(i + 1)) &&
                    (board.at(i) == board.at(i + 2))))
                {   
                    if (board.at(i) == 'X') { return 1; }
                    else if (board.at(i) == 'O') { return 2; }
                } 
            }
        
            char last = board.at(4); //Diagonal Winner check - The last two checks have this boy in common
            if (last != ' ')
            {
                if ((board.at(0) == last) && (last == board.at(8)))
                {
                    if (last == 'X') { return 1; }
                    else if (last == 'O') { return 2; }
                }
                else if ((board.at(2) == last) && (last == board.at(6)))
                {
                    if (last == 'X') { return 1; }
                    else if (last == 'O') { return 2; }
                }
            }
        
            if (this->isFull()) //Tie check - If the board is full and there are no winners
            {
                return -1;
            }
        
            return 0;
        }

        GameBoard()
        {
            board.fill(' ');
        }

};

class Gamelog 
{
    private:
        time_t date;
        std::chrono::seconds duration;
        bool start_P1;
        short mode;
        bool win_P1;
        std::vector<GameBoard> boards;

    public:
        //Getters
        time_t getDate() { return this->date; }
        std::chrono::seconds getDuration() { return this->duration; }
        bool getStartP1() { return this->start_P1; }
        short getMode() { return this->mode; }
        bool getWinP1() { return this->win_P1; }
        std::vector<GameBoard> getBoard() { return this->boards; }

        //Setters
        void setDate(time_t newDate) { this->date = newDate; }
        void setDuration(std::chrono::seconds newDuration) { this->duration = newDuration; }
        void setStartP1(bool newStartP1) { this->start_P1 = newStartP1; }
        void setWinP1(bool newWin) { this->win_P1 = newWin; }
        void setMode(short newMode) { this->mode = newMode; }
        void setBoards(const std::vector<GameBoard>& boards) { this->boards = boards; }

        void pushBoard(const GameBoard& board) { this->boards.push_back(board); } 

        void winnerScreen()
        {
            clearConsole();
            this->boards.back().print();

            std::string opponent;
            std::string P1 = "P1";
            switch (this->mode)
            {
            case 1:
                opponent = "P2";
                break;
            case 2:
                opponent = "CPU (Easy)";
                break;
            case 3:
                opponent = "CPU (Easy)";
                P1 = opponent;
                break;
            case 4:
                opponent = "CPU (Hard)";
                break;
            case 5:
                opponent = "CPU (Hard)";
                P1 = opponent;
                break;
            default:
                break;
            }

            std::string result;
            switch (this->boards.back().checkWinner())
            {
            case -1:
                result = "Tie, " + P1 + " and " + opponent + " Are at the same level!!\n\n";
                break;
            
            case 1:
                result = "Winner!! " + P1 + " wins over " + opponent + "!!!";
                break;
            case 2:
                result = "Winner!! " + opponent + " wins over " + P1 + "!!!";
                break;
            default:
                break;
            }

            std::cout << std::endl << result << std::endl << std::endl;

            getNumber<short>(0, "0 - Go Back: ");
        }





        //BORRAR SI NO TIENE USO
        //Contructor completo
        Gamelog(time_t date, std::chrono::seconds duration, bool start, short mode, bool win, std::vector<GameBoard> boards)
        {
            this->date = date;
            this->duration = duration;
            this->start_P1 = start;
            this->mode = mode;
            this->win_P1 = win;
            this->boards = boards;
        }

        //El constructor cencillo, ya luego ire preguntado datos y setteandolos en el objeto
        Gamelog() : boards(1)
        {
            this->start_P1 = rand() % 2; //Lleva automaticamente el 0 a false y el 1 a true
            this->date = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        }


};

#endif
