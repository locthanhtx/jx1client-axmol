#ifndef KLinkArray_H
#define	KLinkArray_H

class KLinkNode
{
public:
	int32_t nPrev;
	int32_t	nNext;
public:
	KLinkNode() { nPrev = nNext = 0; }
};

class  KLinkArray
{
private:
	KLinkNode*	pNodeL;
	int32_t nCount;
public:
	KLinkArray() { pNodeL = NULL; nCount = 0;}
	~KLinkArray();
	int32_t GetCount() { return nCount; }
	KLinkNode * GetNode(int32_t nIdx){return &pNodeL[nIdx];};
	void			Init(int32_t nSize);
	void			Remove(int32_t nIdx);
	void			Insert(int32_t nIdx);
	int32_t				GetNext(int32_t nIdx) const {
		if(pNodeL) return pNodeL[nIdx].nNext; 
		else return 0;
	}
	int32_t	GetPrev(int32_t nIdx) const {
		if(pNodeL) return pNodeL[nIdx].nPrev; 
		else return 0;
	}
};
#endif
