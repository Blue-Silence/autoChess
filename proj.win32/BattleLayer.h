/******************************************************/
/*                  文件名：BattleLayer.h             */
/*                  功能：战斗逻辑头文件              */
/*                  作者：郑伟丞                      */
/******************************************************/

#pragma once
#include <atomic>
#include"PlayerInfo.h"
#include"ChessBoard.h"
#include "audio/include/SimpleAudioEngine.h"

class BattleLayer :public Layer
{
public:

	// 初始化函数
	bool init();

	// 更新函数，用于每帧更新战斗状态
	void update(float delta);

	// 静态创建函数，用于创建BattleLayer实例
	static BattleLayer* create(PlayerInfo* playerME, PlayerInfo* playerOPP, bool* isInBattle, bool* canBuyChess);

	// 战斗前棋子初始化
	void chessInitBeforeBattle(shared_ptr<Chess> chess, const bool &isME);

	// 计算棋子间的曼哈顿距离
	double getDistance(const ChessCoordinate* start, const ChessCoordinate* end);

	// 寻找敌人
	shared_ptr<Chess> findEnemy(shared_ptr<Chess> damageMaker, PlayerInfo* enemy);

	// 寻路至敌人位置
	bool findPathToEnemy(shared_ptr<Chess> damageMaker, shared_ptr<Chess> targetEnemy);

	// 移动棋子
	void moveChess(shared_ptr<Chess> movingChess, const Vec2 &targetPosition);

	// 执行攻击动作
	void doAttack(shared_ptr<Chess> damageMaker, shared_ptr<Chess> targetEnemy);

	// 检查指定位置是否可通行
	inline bool isAvailable(const int &row,const int &col);

	// 执行战斗全流程
	void playGame(shared_ptr<Chess> chess, PlayerInfo* opp);

	// 判断胜负
	void detectWinner();

	// 显示结束游戏的动画（胜利、失败或平局）
	void BattleLayer::showEndGameAnimation(const string& gameResult);

	// 共用的战后处理逻辑
	void BattleLayer::processAttackResult(shared_ptr<Chess> damageMaker, shared_ptr<Chess> targetEnemy,
								const int &targetRow, const int &targetCol, Sprite* targetChessImage,const bool &useSkill);

	// 播放相应的音效
	void BattleLayer::playSkillSound(shared_ptr<Chess> damageMaker);

	// 技能共用动画逻辑
	void remoteAttackAnimation(shared_ptr<Chess> damageMaker, shared_ptr<Chess> targetEnemy,const bool &useSkill);

	// AI 测试函数
	void AItest();

private:

	// 私有构造函数
	BattleLayer(PlayerInfo* playerOfMe, PlayerInfo* playerOfOpp, bool* inBattle, bool* BuyChess);
	
	// 游戏结束标志
	bool gameOver = false;

	// 游戏中的棋盘状态
	std::atomic<int> boardInGame[BOARDMAXR][BOARDMAXC] = { 0 };

	// 玩家信息
	PlayerInfo* playerME;

	// 对手信息
	PlayerInfo* playerOPP;

	// 是否处于战斗状态
	bool* isInBattle;

	// 是否可以购买棋子
	bool* canBuyChess;

	
};

// A*寻路算法中使用的路径节点类
class pathNode
{
public:
	// 构造函数
	pathNode(int x, int y, pathNode* parent = nullptr);

	// 计算F值
	void calculateFCost();

	// 节点位置
	int x, y;

	// G值，H值和F值
	int gCost, hCost, fCost;

	// 父节点
	pathNode* parent;
};