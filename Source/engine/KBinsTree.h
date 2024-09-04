/* ����������
* 
*/

#ifndef BINARY_SEARCH_TREE_CLASS
#define BINARY_SEARCH_TREE_CLASS

#include <stdlib.h>
#include "KBinTreeNode.h"

template <class T>
class BinSTree
{
    private:
        // ָ����������ǰ����ָ��
        TreeNode<T> *root;
        TreeNode<T> *current;
        
        // �������������
        int32_t size;
      
        // ���ڸ��ƹ��캯������ֵ�����
        TreeNode<T> *CopyTree(TreeNode<T> *t);
        
        // ����������������ֵ������� ClearList ����
        void DeleteTree(TreeNode<T> *t);

	    

        // �ں��� Find �� Delete ��������λ��㼰��˫�������е�λ��
        TreeNode<T> *FindNode(const T& item, TreeNode<T>* & parent) const;

    public:
        // ���캯������������
        BinSTree(void);
        BinSTree(const BinSTree<T>& tree);
        ~BinSTree(void);
        
        // ��ֵ�����
        BinSTree<T>& operator= (const BinSTree<T>& rhs);
		
        
        // ��׼�ı�����
        bool Find(T& item);
        bool Insert(const T& item);
        void Delete(const T& item);
        void ClearList(void);
        bool ListEmpty(void) const;
        int32_t ListSize(void) const;
		int32_t	BiTreeDepth(TreeNode<T> *t);
        // �������ⷽ��
        void Update(const T& item);
        TreeNode<T> *GetRoot(void) const;
};

// ������ t ��ʹ��洢�ڵ�ǰ������
template <class T>
TreeNode<T> *BinSTree<T>::CopyTree(TreeNode<T> *t)
{
    TreeNode<T> *newlptr, *newrptr, *newNode;
   
    // �������֧Ϊ�գ����� NULL
    if (t == NULL)
        return NULL;
        
    // ������ t �������������������� newlptr
    if (t->left != NULL) 
        newlptr = CopyTree(t->left);
    else
        newlptr = NULL;
 
    // ������ t �������������������� newrptr
    if (t->right != NULL) 
        newrptr = CopyTree(t->right);
    else
        newrptr = NULL;
 
    // Ϊ��ǰ��������洢������������ֵ��ָ����������������������ָ��
    newNode = new TreeNode<T>(t->data, newlptr, newrptr);
    return newNode;
}

// ɾ����ǰ����洢����
template <class T>
void BinSTree<T>::DeleteTree(TreeNode<T> *t)
{
    if (t != NULL)
    {
        DeleteTree(t->left);
        DeleteTree(t->right);
        delete t;
		t=NULL;
    }
}

// ������������������ҵ����򷵻ؽ���ַ��ָ����˫�׵�ָ�룻���򣬷��� NULL
template <class T>
TreeNode<T> *BinSTree<T>::FindNode(const T&item, TreeNode<T>* &parent) const
{   
    // ��ָ�� t �Ӹ���ʼ������

	if (root==NULL)
	{
	//	printf("--[���ҵ��ڵ���:root=NULL] -- \n");
		return NULL;
	}

    TreeNode<T> *t = root;	//�Ӹ��ڵ㿪ʼ��

    //����˫��Ϊ NULL
    parent = NULL;

	int32_t i=0;
    // ������Ϊ�գ���ѭ������
    while(t != NULL)
    {
        // ���ҵ���ֵ�����˳�
		//printf("--[��ʼ���ҽڵ���:%d|%d] -- \n",i,item);

        if (item == t->data)
		{
			//printf("--[���ҵ��ڵ���] -- \n");
            break;
		}
        else 
        {
            // �޸�˫��ָ�룬���Ƶ���������������
			//printf("--[���ҵ��ڵ���:%d] -- \n",i);
            parent = t;		 //���ϸ��ڵ� ��ֵ��˫�׽ڵ㣨����Ͻڵ���� ��˫�׽ڵ㣩

            if (item < t->data)
                t = t->left; //����߿�ʼ��
            else 
                t = t->right;//���ұ߿�ʼ��
			++i;
        }
    }
    
    // ����ָ�����ָ�룻��û�ҵ����򷵻� NULL
    return t;
}

// ���캯������ʼ�� root��current Ϊ�գ�size Ϊ 0
template <class T>
BinSTree<T>::BinSTree(void):root(NULL), current(NULL), size(0)
{}

// ���ƹ��캯��
template <class T>
BinSTree<T>::BinSTree(const BinSTree<T>& tree)
{
    // �� tree ���Ƶ���ǰ���󣬷��� current �� size
    root = CopyTree(tree.root);
    current = root;
    size = tree.size;
}

// ��������
template <class T>
BinSTree<T>::~BinSTree(void)
{
    ClearList();
}

// ɾ�����е����н��
template <class T>
void BinSTree<T>::ClearList(void)
{
    DeleteTree(root);
    root = current = NULL;
    size = 0;
}

// ��ֵ�����
template <class T>
BinSTree<T>& BinSTree<T>::operator= (const BinSTree<T>& rhs)
{
    // ���ܽ������Ƶ�����
    if (this == &rhs)
        return *this;
        
    // �����ǰ�������������Ƶ���ǰ����
    ClearList();
    root = CopyTree(rhs.root);
    
    // �� current ָ��ָ�� root ���������� size ֵ
    current = root;
    size = rhs.size;
    
    // ���ص�ǰ�����ָ��
    return *this;
}

// ���������� item�����ҵ����򽫽�����ݸ��� item
template <class T>
bool BinSTree<T>::Find(T& item)
{
    // ʹ�� FindNode������Ҫ parent ����
    TreeNode<T> *parent=NULL;
	//printf("--[���ҽڵ���A] -- \n");

    // ���������� item����ƥ��Ľ�㸳�� current
    current = FindNode(item, parent);
   // printf("--[���ҽڵ���B] -- \n");
    // ���ҵ��������ݸ��� item ������ True
    if (current != NULL)
    {
        item = current->data;
        return TRUE;
    }
    else
    	// ������û�ҵ� item������ False
        return FALSE;
}

// ָʾ���Ƿ�Ϊ��
template <class T>
bool BinSTree<T>::ListEmpty(void) const
{
    return (size == 0);
}

// �������е����������
template <class T>
int32_t BinSTree<T>::ListSize(void) const
{
    return size;
}

// ���������в����������Ԫ���ظ������������Ԫ��
template <class T>
bool BinSTree<T>::Insert(const T& item)
{
    // t Ϊ���������еĵ�ǰ��㣬parent Ϊǰһ���
    TreeNode<T> *parent = NULL;
	//TreeNode<T> *newNode =NULL;
	current = FindNode(item, parent);
    
	if (current != NULL) //��������� ֱ�Ӹ�ֵ
	{
		current->data = item;
		return true;
	}   
	else
	{
	    TreeNode<T> *newNode = new TreeNode<T>(item,NULL,NULL);	 //�����ܶ������
		// �� parent Ϊ NULL��������Ϊ��������
		if (parent == NULL)
		{
			// �����µ�Ҷ�ӽ��
			root = newNode;
		}
		// �� item < parent->data��������Ϊ���Ӳ���        
		else if (item < parent->data)                   
			parent->left = newNode;
        
		else
			// �� item >= parent->data����Ϊ�Һ��Ӳ���     
			parent->right = newNode;
        
		// current ��ֵΪ�½��ĵ�ַ���� size �� 1
		current = newNode;
		size++;
	}
	return false;

}

// ��� item �����У�����ɾ��
template <class T>
void BinSTree<T>::Delete(const T& item)
{
    // DNodePtr = ָ��ɾ����� D ��ָ��
    // PNodePtr = ָ����� D ��˫�׽ڵ� P ��ָ��
    // RNodePtr = ָ���滻 D �Ľ�� R ��ָ��
    TreeNode<T> *DNodePtr, *PNodePtr, *RNodePtr;
    
    // ��������ֵΪ item �Ľ�㣬������ý���˫�׽���ָ��
	if ((DNodePtr = FindNode (item, PNodePtr)) == NULL)
        return;
    
    // ��� D ��һ��ָ��Ϊ NULL�����滻���Ϊ����һ֦��ĳһ���
    if (DNodePtr->right == NULL)
        RNodePtr = DNodePtr->left;
    else if (DNodePtr->left == NULL)
        RNodePtr = DNodePtr->right;
        
    // DNodePtr ������ָ�����Ϊ NULL
    else
    {
        // Ѱ�Ҳ�ж�� D ���滻��㡣�ӽ�� D ����������ʼ��������ֵС�� D ������ֵ��
        // ���ֵ�����ý������жϿ�
        
        // PofRNodePtr = ָ���滻���˫�׵�ָ��
        TreeNode<T> *PofRNodePtr = DNodePtr;
        
        // ��һ�ֿ��ܵ��滻Ϊ D ������
        RNodePtr = DNodePtr->left;
    
        // �� D �����ӵ����������������������ֵ������¼��ǰ��㼰��˫�׽���
        // ָ�룬������ǽ��ҵ��滻���
        while(RNodePtr->right != NULL)
        {
            PofRNodePtr = RNodePtr;
            RNodePtr = RNodePtr->right;
        }
        
        if (PofRNodePtr == DNodePtr)
            // ��ɾ����������Ϊ�滻��㣬�� D ������������ R
            RNodePtr->right = DNodePtr->right;
        else
        {
            // �������������ƶ���һ����㣬������ɾ���滻��㣬����������������˫��
            PofRNodePtr->right = RNodePtr->left;
            
            // ���滻������ DNodePtr
            RNodePtr->left = DNodePtr->left;
            RNodePtr->right = DNodePtr->right;
        }
    }

    // ��ɵ�˫�׽������ӡ�ɾ������㣬�����¸���ֵ
    if (PNodePtr == NULL)
        root = RNodePtr;
        
    // �� R ���� P ����ȷһ֦��
    else if (DNodePtr->data < PNodePtr->data)
        PNodePtr->left = RNodePtr;
    else
        PNodePtr->right = RNodePtr;
        
    // �ͷű�ɾ����ڴ沢�����Ĵ�С�� 1
    delete DNodePtr;
	DNodePtr=NULL;
    size--;
}

// ����ǰ����Ѷ���������ֵ���������ֵ��ȣ��򽫽��ֵ���� item�����򣬽� item ���뵽����
template <class T>
void BinSTree<T>::Update(const T& item)
{   
    if (current != NULL && current->data == item)
            current->data = item;
    else
        Insert(item);
}

// ���ظ����ĵ�ַ
template <class T>
TreeNode<T> *BinSTree<T>::GetRoot(void) const
{
    return root;
}


/* ��ʼ����: ������T���ڡ��������: ����T����� */
template <class T>
int32_t BinSTree<T>::BiTreeDepth(TreeNode<T> *t)
{//�ݹ鷽��
	int32_t i,j;

    if(!t)
		return 0;

	if(t->left)
		i=BiTreeDepth(t->left);   // ���������
	else
		i=0;

	if(t->right)
		j=BiTreeDepth(t->right);  // ���������
	else
		j=0;

	return i>j?i+1:j+1;
}




#endif  // BINARY_SEARCH_TREE_CLASS


/*
Node* search(Node* root, int32_t key)
{ // Base Cases: ��������ֵΪ���ڵ�ֵ 
	if (root == NULL || root->key == key)
		return root; 
	// ָ��ֵ���ڸ�ֵ 
	if (root->key < key)
		return search(root->right, key); // ָ��ֵС�ڸ�ֲ 
	return search(root->left, key);
 }

 ��һ����1���ڵ㣬����2^0
 �ڶ�����2���ڵ㣬����2^1
 ������������
��n����2^(n-1)����㣬Ȼ����ݵȱ�������͹�ʽ���ܼ�����ܵĽ�����ĿΪ2^n - 1��
���Ե��߶���n��ʱ���ܽ��ĸ�����2^n - 1��Ȼ�����ö�������������߶Ⱥ��ܽ�����Ĺ�ϵ��
*/

