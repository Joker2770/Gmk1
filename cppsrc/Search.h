#pragma once

#include "Common.h"
#include "Board.h"
#include "Evaluation.h"
#include "NN/nn_cpp.h"
#include <vector>
#include <map>
using std::vector;

typedef float Val;
int randomSelect(const vector<float> &weight);
int randomSelect(BoardWeight weight, int count);

class MCTS
{
public:
	Val UCBC = 1.6f;
	bool add_noise = false;
	bool use_transform = false;
	float mcwin;
	int stopflag;
	clock_t starttime;
private:
	int playouts;
	NN* network;
	struct Node
	{
		Val sumv,policy;
		int cnt; int fa;
		vector<int> ch;
		int move;
		void print()
		{
			//fout << "rate:" << score / cnt << " cnt:" << cnt << " " << Coord(p) << " ch:";
			//for (auto it : ch) fout << it << ' ';
			//fout << '\n';
		}
	};
	Node *tr;
	std::map<unsigned long long, int> hash_table;
	BoardHasher boardhash;
	const int root = 0;
	int trcnt = 1, viscnt = 0;
	Board board;
	int nowcol;
	int counter = 0;

private:
	void make_move(int move);
	void unmake_move(int move);
	Val getValue();
	void expand(int cur, RawOutput &output, Board &avail);
	void simulation_back(int cur);
	void addNoise(int cur, Val alpha, Val epsilon);
	void createRoot();
	int selection(int cur);
	bool getTimeLimit(int played);

public:
	MCTS(Board &_board, int _col, NN *_network, int _playouts);
	void solve(BoardWeight &result);
	int solvePolicy(Val te, BoardWeight &policy, float &winrate);
	~MCTS()
	{
		delete[] tr;
	}
};

class Player
{
private:
	NN network;
	int cfg_playouts;
	float cfg_puct;
	bool cfg_add_noise;
	bool cfg_use_transform;
	float cfg_temprature1;
	float cfg_temprature2;
	int cfg_temprature_moves;

	BoardWeight policy;
	float winrate;
public:
	Player(string file_network, 
		int _playouts = 400,
		float _puct=1.6, 
		bool _add_noise = false, 
		bool _use_transform = true,
		float _temprature1 = 0.6,
		float _temprature2 = 0.0, 
		int _temprature_moves = 14):network(file_network)
	{
		cfg_playouts = _playouts;
		cfg_puct = _puct;
		cfg_add_noise = _add_noise;
		cfg_use_transform = _use_transform;
		cfg_temprature1=_temprature1;
		cfg_temprature2=	_temprature2;
		cfg_temprature_moves=	_temprature_moves;
	}

	Coord randomOpening(Board gameboard);

	Coord run(Board &gameboard, int nowcol);

	const BoardWeight& getlastPolicy()
	{
		return policy;
	}
	float getlastValue()
	{
		return winrate;
	}
};
