
#include <list>
#include <unordered_map>
#include <stdio.h>
#include "GameObject.h"

using namespace std;


class Quadtree
{
private:
	int            m_level;
	RECT* m_region;
	vector<CGameObject*>* m_objects_list = new vector<CGameObject*>();
	Quadtree** m_nodes;

	bool           IsContain(CGameObject* entity);
	void           Split();
public:
	Quadtree();
	Quadtree(int level, RECT* region);
	//~Quadtree();

	void  Clear();
	void  Insert(CGameObject* entity);
	void  Retrieve(vector<CGameObject*>* return_objects_list, CGameObject* entity);
};