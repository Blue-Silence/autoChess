/******************************************************/
/*                  文件名：AIMode.cpp                */
/*                  功能：AI玩家的信息                */
/******************************************************/

#include "AIMode.h"

//***************//
//               //
//  接口函数实现 //
//               //
//***************//
vector<PlayerInfo>* AIMode::getPlayersVectorInfo()
{
    return &players;
}


//***************//
//               //
//  功能函数实现 //
//               //
//***************//

// 构造函数实现
AIMode::AIMode(int aiCount)
{
    // 根据AI数量创建PlayerInfo对象
    // players[1...aiCount]代表1号AI,2号AI...
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

// 随机扣血函数
void AIMode::randomBloodLose(int AIInBattle)
{
    turn++;
    for (int i = 1; i <= totalAINum; ++i)
    {
        if (i == AIInBattle || !players[i].isAlive())
        {
            continue;
        }
        
       // 扣除一定的血量
        players[i].DecreaseLifeValue(players[i].getLevel() + i % 5);
        if (!players[i].isAlive())
            liveAINum--;
    }
    
}

// 在类外实现拷贝构造函数
AIMode::AIMode(const AIMode& other) : players(other.players)
{
    // 复制其他成员变量
    turn = other.turn;

    // 复制 chessBoard 数组
    for (int i = 0; i < BOARDMAXR; ++i)
    {
        for (int j = 0; j < BOARDMAXC; ++j) 
        {
            chessBoard[i][j] = other.chessBoard[i][j];
        }
    }
}

// 在类外实现赋值运算符
AIMode& AIMode::operator=(const AIMode& other)
{
    if (this != &other) 
    {
        // 复制 players
        players = other.players;

        // 复制其他成员变量
        turn = other.turn;

        // 复制 chessBoard 数组
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


// 随机数获取 范围minNum - maxNum
int AIMode::getRandomNumber(int minNum, int maxNum)
{
    // 静态，以确保只在函数首次调用时进行初始化
    static random_device rd;  // 用于获得种子
    static mt19937 gen(rd()); // 标准的mersenne_twister_engine生成器
    uniform_int_distribution<> dis(minNum, maxNum); // 定义一个从minNum到maxNum的分布

    return dis(gen); // 生成并返回随机数
}

// 初始化游戏阵容
void AIMode::initializeGame(int index)
{
    // 给出三种初始化阵容，根据随机数确定
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

// 随等级增加而增加英雄
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

            // 判断该位置可不可以放置棋子
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

// 提升玩家等级(每2回合提升一次等级)
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

// 提升英雄星级(回合数为2 5 7 10随机提升一个英雄星级)
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

// 每回合结束需要做的工作集成
void AIMode::eachTurnOver()
{
    promoteLevel();
    addHero();
    promoteHeroStar();
}

