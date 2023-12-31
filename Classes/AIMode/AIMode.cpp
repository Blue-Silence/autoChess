/******************************************************/
/*                  �ļ�����AIMode.cpp                */
/*                  ���ܣ�AI��ҵ���Ϣ                */
/******************************************************/

#include "AIMode.h"

//***************//
//               //
//  �ӿں���ʵ�� //
//               //
//***************//
vector<PlayerInfo>* AIMode::getPlayersVectorInfo()
{
    return &players;
}


//***************//
//               //
//  ���ܺ���ʵ�� //
//               //
//***************//

// ���캯��ʵ��
AIMode::AIMode(int aiCount)
{
    // ����AI��������PlayerInfo����
    // players[1...aiCount]����1��AI,2��AI...
    players.emplace_back(*PlayerInfo::create());

    totalAINum = aiCount;
    liveAINum = aiCount;

    for (int i = 1; i <= aiCount; ++i)
    {
        players.emplace_back(*PlayerInfo::create());
        players[i].setCoinNum(500);
        initializeGame(i);
    }
}

// �����Ѫ����
void AIMode::randomBloodLose(int AIInBattle)
{
    turn++;
    for (int i = 1; i <= totalAINum; ++i)
    {
        if (i == AIInBattle || !players[i].isAlive())
        {
            continue;
        }
        
       // �۳�һ����Ѫ��
        players[i].DecreaseLifeValue(players[i].getLevel() + i % 5);
        if (!players[i].isAlive())
            liveAINum--;
    }
    
}

// ������ʵ�ֿ������캯��
AIMode::AIMode(const AIMode& other) : players(other.players)
{
    // ����������Ա����
    turn = other.turn;

    // ���� chessBoard ����
    for (int i = 0; i < BOARDMAXR; ++i)
    {
        for (int j = 0; j < BOARDMAXC; ++j) 
        {
            chessBoard[i][j] = other.chessBoard[i][j];
        }
    }
}

// ������ʵ�ָ�ֵ�����
AIMode& AIMode::operator=(const AIMode& other)
{
    if (this != &other) 
    {
        // ���� players
        players = other.players;

        // ����������Ա����
        turn = other.turn;

        // ���� chessBoard ����
        for (int i = 0; i < BOARDMAXR; ++i)
        {
            for (int j = 0; j < BOARDMAXC; ++j) 
            {
                chessBoard[i][j] = other.chessBoard[i][j];
            }
        }
    }
    return *this;
}


// �������ȡ ��ΧminNum - maxNum
int AIMode::getRandomNumber(int minNum, int maxNum)
{
    // ��̬����ȷ��ֻ�ں����״ε���ʱ���г�ʼ��
    static random_device rd;  // ���ڻ������
    static mt19937 gen(rd()); // ��׼��mersenne_twister_engine������
    uniform_int_distribution<> dis(minNum, maxNum); // ����һ����minNum��maxNum�ķֲ�

    return dis(gen); // ���ɲ����������
}

// ��ʼ����Ϸ����
void AIMode::initializeGame(int index)
{
    // �������ֳ�ʼ�����ݣ����������ȷ��
    int randomNum = getRandomNumber(1, 3);

    shared_ptr<Chess> chess1, chess2, chess3;
   
    ChessCoordinate* newPos = new ChessCoordinate;

    switch (randomNum)
    {
        case 1:
            players[index].putChessInBattleArea(make_shared<tank>(ZHANGFEI));
            players[index].putChessInBattleArea(make_shared<shooter>(DIRENJIE));
            players[index].putChessInBattleArea(make_shared<mage>(DIAOCHAN));

            chess1 = (*players[index].getBattleAreaChesses())[0];
            chess2 = (*players[index].getBattleAreaChesses())[1];
            chess3 = (*players[index].getBattleAreaChesses())[2];

            chess1->setChessCoordinateByType(Vec2(3, 3), CoordinateType::chessBoardCoordinates);
            CoordinateConvert(CoordinateType::screenCoordinates, Vec2(3, 3), newPos);
            chess1->setChessCoordinateByType(Vec2(newPos->getX(), newPos->getY()), CoordinateType::screenCoordinates);
            

            chess2->setChessCoordinateByType(Vec2(3, 4), CoordinateType::chessBoardCoordinates);
            CoordinateConvert(CoordinateType::screenCoordinates, Vec2(3, 4), newPos);
            chess2->setChessCoordinateByType(Vec2(newPos->getX(), newPos->getY()), CoordinateType::screenCoordinates);

            chess3->setChessCoordinateByType(Vec2(2, 3), CoordinateType::chessBoardCoordinates);
            CoordinateConvert(CoordinateType::screenCoordinates, Vec2(2, 3), newPos);
            chess3->setChessCoordinateByType(Vec2(newPos->getX(), newPos->getY()), CoordinateType::screenCoordinates);

            chessBoard[3][3] = chessBoard[3][4] = chessBoard[2][3] = 1;

            delete newPos;
            break;
        case 2:
            players[index].putChessInBattleArea(make_shared<tank>(ZHANGFEI));
            players[index].putChessInBattleArea(make_shared<shooter>(HOUYI));
            players[index].putChessInBattleArea(make_shared<mage>(DAJI));

            chess1 = (*players[index].getBattleAreaChesses())[0];
            chess2 = (*players[index].getBattleAreaChesses())[1];
            chess3 = (*players[index].getBattleAreaChesses())[2];

            chess1->setChessCoordinateByType(Vec2(4, 3), CoordinateType::chessBoardCoordinates);
            CoordinateConvert(CoordinateType::screenCoordinates, Vec2(4, 3), newPos);
            chess1->setChessCoordinateByType(Vec2(newPos->getX(), newPos->getY()), CoordinateType::screenCoordinates);

            chess2->setChessCoordinateByType(Vec2(3, 4), CoordinateType::chessBoardCoordinates);
            CoordinateConvert(CoordinateType::screenCoordinates, Vec2(3, 4), newPos);
            chess2->setChessCoordinateByType(Vec2(newPos->getX(), newPos->getY()), CoordinateType::screenCoordinates);

            chess3->setChessCoordinateByType(Vec2(3, 3), CoordinateType::chessBoardCoordinates);
            CoordinateConvert(CoordinateType::screenCoordinates, Vec2(3, 3), newPos);
            chess3->setChessCoordinateByType(Vec2(newPos->getX(), newPos->getY()), CoordinateType::screenCoordinates);

            chessBoard[4][3] = chessBoard[3][4] = chessBoard[3][3] = 1;

            delete newPos;
            break;
        case 3:
            players[index].putChessInBattleArea(make_shared<tank>(XIANGYU));
            players[index].putChessInBattleArea(make_shared<shooter>(HOUYI));
            players[index].putChessInBattleArea(make_shared<mage>(DAJI));

            chess1 = (*players[index].getBattleAreaChesses())[0];
            chess2 = (*players[index].getBattleAreaChesses())[1];
            chess3 = (*players[index].getBattleAreaChesses())[2];

            chess1->setChessCoordinateByType(Vec2(5, 3), CoordinateType::chessBoardCoordinates);
            CoordinateConvert(CoordinateType::screenCoordinates, Vec2(5, 3), newPos);
            chess1->setChessCoordinateByType(Vec2(newPos->getX(), newPos->getY()), CoordinateType::screenCoordinates);

            chess2->setChessCoordinateByType(Vec2(4, 4), CoordinateType::chessBoardCoordinates);
            CoordinateConvert(CoordinateType::screenCoordinates, Vec2(4, 4), newPos);
            chess2->setChessCoordinateByType(Vec2(newPos->getX(), newPos->getY()), CoordinateType::screenCoordinates);

            chess3->setChessCoordinateByType(Vec2(3, 3), CoordinateType::chessBoardCoordinates);
            CoordinateConvert(CoordinateType::screenCoordinates, Vec2(3, 3), newPos);
            chess3->setChessCoordinateByType(Vec2(newPos->getX(), newPos->getY()), CoordinateType::screenCoordinates);

            chessBoard[5][3] = chessBoard[4][4] = chessBoard[3][3] = 1;

            delete newPos;
            break;
        default:
            players[index].putChessInBattleArea(make_shared<tank>(XIANGYU));
            players[index].putChessInBattleArea(make_shared<shooter>(HOUYI));
            players[index].putChessInBattleArea(make_shared<mage>(DAJI));

            chess1 = (*players[index].getBattleAreaChesses())[0];
            chess2 = (*players[index].getBattleAreaChesses())[1];
            chess3 = (*players[index].getBattleAreaChesses())[2];

            chess1->setChessCoordinateByType(Vec2(4, 3), CoordinateType::chessBoardCoordinates);
            CoordinateConvert(CoordinateType::screenCoordinates, Vec2(4, 3), newPos);
            chess1->setChessCoordinateByType(Vec2(newPos->getX(), newPos->getY()), CoordinateType::screenCoordinates); 

            chess2->setChessCoordinateByType(Vec2(4, 4), CoordinateType::chessBoardCoordinates);
            CoordinateConvert(CoordinateType::screenCoordinates, Vec2(4, 4), newPos);
            chess2->setChessCoordinateByType(Vec2(newPos->getX(), newPos->getY()), CoordinateType::screenCoordinates);

            chess3->setChessCoordinateByType(Vec2(3, 3), CoordinateType::chessBoardCoordinates);
            CoordinateConvert(CoordinateType::screenCoordinates, Vec2(3, 3), newPos);
            chess3->setChessCoordinateByType(Vec2(newPos->getX(), newPos->getY()), CoordinateType::screenCoordinates);

            chessBoard[4][3] = chessBoard[4][4] = chessBoard[3][3] = 1;

            delete newPos;
            break;
    }

}

// ��ȼ����Ӷ�����Ӣ��
void AIMode::addHero()
{
    for (unsigned int  i = 1; i < players.size(); ++i)
    {
        if (players[i].getLevel() == 3 || players[i].getLevel() == 5 || players[i].getLevel() == 7 || players[i].getLevel() == 9)
        {
            int randomNum = getRandomNumber(0, 5);
            shared_ptr<Chess> chess;
            switch (randomNum)
            {
                case 0:
                case 1:
                    players[i].putChessInBattleArea(make_shared<mage>(randomNum));
                    break;
                case 2:
                case 3:
                    players[i].putChessInBattleArea(make_shared<shooter>(randomNum));
                    break;
                case 4:
                case 5:
                    players[i].putChessInBattleArea(make_shared<tank>(randomNum));
                    break;
                default:
                    return;
            }

            ChessCoordinate* newPos = new ChessCoordinate;
            chess = (*players[i].getBattleAreaChesses()).back();

            // �жϸ�λ�ÿɲ����Է�������
            int row, col;
            while (1)
            {
                col = getRandomNumber(0, 7);
                row = getRandomNumber(3, 4);
                if (chessBoard[row][col] != 0)
                {
                    continue;
                }
                else
                {
                    break;
                }
            }
            chess->setChessCoordinateByType(Vec2(col, row), CoordinateType::chessBoardCoordinates);
            CoordinateConvert(CoordinateType::screenCoordinates, Vec2(col, row), newPos);
            chess->setChessCoordinateByType(Vec2(newPos->getX(), newPos->getY()), CoordinateType::screenCoordinates);

            chessBoard[row][col] = 1;

            delete newPos;
        }
    }
    
    
}

// ������ҵȼ�(ÿ2�غ�����һ�εȼ�)
void AIMode::promoteLevel()
{
    for (unsigned int  i = 1; i < players.size(); ++i)
    {
        if (turn % 2 == 0)
        {
            players[i].ChangeLevel();
        }
    }
}

// ����Ӣ���Ǽ�(�غ���Ϊ2 5 7 10�������һ��Ӣ���Ǽ�)
void AIMode::promoteHeroStar()
{
    for (unsigned int  i = 1; i < players.size(); ++i)
    {
        int randomNum = getRandomNumber(0, players[i].getMaxBattleChessNum()-1);
        if (turn == 2 || turn == 5 || turn == 7 || turn == 10)
        {
            if ((*players[i].getBattleAreaChesses())[randomNum]->getChessLevel() == 1)
            {
                (*players[i].getBattleAreaChesses())[randomNum]->setChessLevel(2);
            }
            else if ((*players[i].getBattleAreaChesses())[randomNum]->getChessLevel() == 2)
            {
                (*players[i].getBattleAreaChesses())[randomNum]->setChessLevel(3);
            }
        }
    }
}

// ÿ�غϽ�����Ҫ���Ĺ�������
void AIMode::eachTurnOver()
{
    promoteLevel();
    addHero();
    promoteHeroStar();
}

