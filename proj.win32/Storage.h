/*
���ܣ����Ӵ洢����Ϣ
���ߣ�Hu Junwei
*/
#pragma once

#ifndef _STORAGE_H_
#define _STORAGE_H_

#include <vector>
#include "ChessPiece.h"

using std::vector;

class Storage :public Ref
{
private:
	vector<tank>tankVector;
	vector<shooter>shooterVector;
	vector<mage>mageVector;

	// ����������ƴ洢��
	virtual bool init();

	// xxxMemorize���ǽ��뿨�� xxxDelete���Ǵӿ�����ɾ��
	void tankMemorize(tank obj);
	void tankDelete(tank* ptr);
	void shooterMemorize(shooter obj);
	void shooterDelete(shooter* ptr);
	void mageMemorize(mage obj);
	void mageDelete(mage* ptr);
};


#endif

