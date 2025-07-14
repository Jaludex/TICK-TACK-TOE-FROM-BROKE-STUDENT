#include <headers/gameplay.hpp>

void gameplay(Player& p1, Player& p2, Gamelog& this_game)
{
    GameBoard this_board;

    auto start_point = std::chrono::high_resolution_clock::now();

    short result = 0;
    short turn(0);
    bool p1_turn(this_game.getStartP1()); //With this, i manage whos turn comes next and who starts

    do
    {   
        clearConsole();
        std::cout << "Turn " << turn + 1 << ":" << std::endl;
        this_board.print();

        if (p1_turn)
        {
            p1.hisTurn(this_board);
        }
        else
        {
            p2.hisTurn(this_board);
        }

        p1_turn = !p1_turn;
        turn++;

        this_game.pushBoard(this_board);

        result = this_board.checkWinner();

    } while(!result); //When result stops being 0, the game ends

    auto end_point = std::chrono::high_resolution_clock::now();

    this_game.setDuration(std::chrono::duration_cast<std::chrono::seconds>(end_point - start_point));

    result == 1 ? this_game.setWinP1(true) : this_game.setWinP1(false);

}

void preGame()
{
    //User chooses his gamemode
    clearConsole();
    std::cout << "Now tell me, how do you wanna play?\n\n" <<
                 "1 - 2 Player mode\n2 - Against Easy CPU\n3 - CPU vs CPU Easy\n4 - Againts Hard CPU\n5 - CPU vs CPU Hard\n\n0 - Go back\n\n";
    
    short input = getNumber<short>(5, "Choose an option: ");

    Gamelog this_game;

    switch (input)
    {
    case 1:
        {
        RealPlayer p1('X');
        RealPlayer p2('O');

        gameplay(p1, p2, this_game);
        break;
        }
    case 2:
    {
        RealPlayer p1('X');
        CPUEasy cpu('O');

        gameplay(p1, cpu, this_game);
        break;
    }
    case 3:
    {
        CPUEasy cpu1('X');
        CPUEasy cpu2('O');

        gameplay(cpu1, cpu2, this_game);
        break;
    }
    case 4:
    {
        RealPlayer p1('X');
        CPUHard cpu2('O');

        gameplay(p1, cpu2, this_game);
        break;
    }
    case 5:
    {
        CPUHard cpu1('X');
        CPUHard cpu2('O');

        gameplay(cpu1, cpu2, this_game);
        break;
    }
    case 0:
        return;
    default:
        return;
    }

    //Here is the aftergame

    this_game.setMode(input);

    this_game.winnerScreen();
    //Push log into stadistics

    
}

void RealPlayer::hisTurn(GameBoard& board)
{
    short pos;
    do
    {
        std::cout << this->letter;
        pos = getNumber<short>(1 , 9, " - Where do you want to play? (1-9): ");
    } while (!board.playAt(this->letter, pos - 1));
}

void CPUEasy::hisTurn(GameBoard& board) //An easy only moves random
{
    short pos;
    do
    {
        pos = rand() % 9;
    } while (!board.playAt(this->letter, pos));

    std::this_thread::sleep_for(std::chrono::seconds(1));
}


//GOOOOOOOSH IMPLEMENTING THIS WAS A NIGHMARE

//Find the first empty cell that would make player win
int winningMove(const std::array<char,9>& cells, char player) {
    //All win lines
    static const int winLines[8][3] = {
        {0,1,2},{3,4,5},{6,7,8}, //Rows
        {0,3,6},{1,4,7},{2,5,8}, //Columns
        {0,4,8},{2,4,6}          //Diagonals
    };
    for (const auto& line : winLines) {
        int countPlayer=0, countEmpty=0, emptyIdx=-1;
        for (int idx : line) {
            if (cells[idx] == player) countPlayer++;
            else if (cells[idx] == ' ') { countEmpty++; emptyIdx = idx; }
        }
        if (countPlayer==2 && countEmpty==1) return emptyIdx;
    }
    return -1;
}

void CPUHard::hisTurn(GameBoard& board) {
    auto cells = board.getBoard();
    char cpu = this->letter;
    char opp = (cpu == 'X') ? 'O' : 'X';

    int move = -1;


    //Win if possible
    move = winningMove(cells, cpu);
    if (move != -1 && board.playAt(cpu, move)) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return;
    }
    
    //Block if opponent could win
    move = winningMove(cells, opp);
    if (move != -1 && board.playAt(cpu, move)) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return;
    }

    //Take center if available
    if (cells[4] == ' ' && board.playAt(cpu, 4)) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return;
    }

    //Pick a random available corner
    std::vector<int> corners = {0,2,6,8};
    std::vector<int> availCorners;
    for(int idx : corners)
        if(cells[idx] == ' ') availCorners.push_back(idx);
    if (!availCorners.empty()) {
        move = availCorners[rand()%availCorners.size()];
        if (board.playAt(cpu, move)) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            return;
        }
    }

    //Pick random side
    std::vector<int> sides = {1,3,5,7};
    std::vector<int> availSides;
    for(int idx : sides)
        if(cells[idx] == ' ') availSides.push_back(idx);
    if (!availSides.empty()) {
        move = availSides[rand()%availSides.size()];
        board.playAt(cpu, move);
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
}

