//#include "Quadtree.h"
//#include "GameObject.h"
//#include <math.h>
//
//#define MAX_LEVEL                5
//#define MAX_OBJECT_IN_REGION     4
//
//Quadtree::Quadtree(int level, RECT region) {
//	this->m_level = level;
//	this->m_region = region;
//}
//
//void Quadtree::Clear()
//{
//	// Clear all nodes
//	if (m_nodes)
//	{
//		for (int i = 0; i < 4; i++)
//		{
//			m_nodes[i]->Clear();
//			delete m_nodes[i];
//		}
//		delete[] m_nodes;
//	}
//
//	// Clear current Quadtree
//	m_objects_list->clear();
//
//	delete m_objects_list;
//	delete &m_region;
//}
//
//bool Quadtree::IsContain(CGameObject* CGameObject)
//{
//
//	float left, top, right, bottom;
//
//	CGameObject->GetBoundingBox(left, top, right, bottom);
//
//	float width = abs(right - left);
//	float height = abs(top - bottom);
//
//	return !(right > m_region.right ||
//		top < m_region.top ||
//		left < m_region.left ||
//		bottom > m_region.bottom);
//}
//
//void Quadtree::Split()
//{
//	double middle_x = m_region.left + m_region.right / 2;
//	double middle_y = m_region.top + m_region.bottom / 2;
//
//	RECT rect1 = { m_region.left ,m_region.top , middle_x, middle_y };
//	RECT rect2 = { middle_x ,m_region.top , m_region.right, middle_y };
//	RECT rect3 = { m_region.left, middle_y, middle_x, m_region.bottom };
//	RECT rect4 = { middle_x, middle_x, m_region.right, m_region.bottom };
//
//	//rec1 : top = top, left=left,right=middle_x, bot=middle_y
//	//rec1 : top = top, left=left,right=middle_x, bot=middle_y
//	m_nodes = new Quadtree * [4];
//
//	m_nodes[0] = new Quadtree(m_level + 1, rect1);
//	m_nodes[1] = new Quadtree(m_level + 1, rect2);
//	m_nodes[2] = new Quadtree(m_level + 1, rect3);
//	m_nodes[3] = new Quadtree(m_level + 1, rect4);
//}
//
//void Quadtree::Insert(CGameObject* entity)
//{
//	// Insert CGameObject into corresponding nodes
//	if (m_nodes)
//	{
//		if (m_nodes[0]->IsContain(entity))
//			m_nodes[0]->Insert(entity);
//		if (m_nodes[1]->IsContain(entity))
//			m_nodes[1]->Insert(entity);
//		if (m_nodes[2]->IsContain(entity))
//			m_nodes[2]->Insert(entity);
//		if (m_nodes[3]->IsContain(entity))
//			m_nodes[3]->Insert(entity);
//
//		return; // Return here to ignore rest.
//	}
//
//	// Insert entity into current quadtree
//	if (this->IsContain(entity))
//		m_objects_list->push_back(entity);
//
//	// Split and move all objects in list into it’s corresponding nodes
//	if (m_objects_list->size() > MAX_OBJECT_IN_REGION && m_level < MAX_LEVEL)
//	{
//		Split();
//
//		while (!m_objects_list->empty())
//		{
//			if (m_nodes[0]->IsContain(m_objects_list->back()))
//				m_nodes[0]->Insert(m_objects_list->back());
//			if (m_nodes[1]->IsContain(m_objects_list->back()))
//				m_nodes[1]->Insert(m_objects_list->back());
//			if (m_nodes[2]->IsContain(m_objects_list->back()))
//				m_nodes[2]->Insert(m_objects_list->back());
//			if (m_nodes[3]->IsContain(m_objects_list->back()))
//				m_nodes[3]->Insert(m_objects_list->back());
//
//			m_objects_list->pop_back();
//		}
//	}
//}