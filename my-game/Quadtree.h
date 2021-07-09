#include <list>
#include <unordered_map>
#include <stdio.h>
#include "GameObject.h"

using namespace std;


class Quadtree
{
private:
	int            m_level;
	Rect* m_region = new Rect{ 0,0,0,0 };
	vector<CGameObject*>* m_objects_list = new vector<CGameObject*>();
	Quadtree** m_nodes;

	void           Split();
public:
	Quadtree();
	Quadtree(int level, Rect* region);
	//~Quadtree();

	bool  IsContain(CGameObject* entity);
	void  Clear();
	void  Insert(CGameObject* entity);
	void  Retrieve(vector<CGameObject*>* return_objects_list, CGameObject* entity);
};