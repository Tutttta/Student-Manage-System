#pragma once
// 链栈实现类

template <typename T>
class CStack
{
public:
	// 栈结点的数据结构
	typedef struct _STACKNODE
	{
		_STACKNODE() = default;
		_STACKNODE(T data) : m_data(data), m_pNext(nullptr) {}
		T m_data;
		struct _STACKNODE *m_pNext;
	} STACKNODE, *PSTACKNODE;
	// 无参构造函数
	CStack();
	// 含参构造函数
	CStack(T data);
	// 析构函数
	~CStack();
	// 将栈内元素逆置
	void ReverseStack();
	// 往栈内压入一个结点
	bool Push(const STACKNODE &rcStackNode);
	bool Push(T data);
	// 从栈内弹出数据
	bool Pop();
	bool Pop(T &data);
	bool Pop(STACKNODE &node);
	// 创建一个栈
	bool CreateStack(const PSTACKNODE pcStackNodeAry, size_t nArySize);
	bool CreateStack(const T *pDataAry, size_t nArySize);
	// 返回栈顶结点内容
	T GetTop() const;
	// 判定链栈是否为空
	inline bool IsEmpty() const { return(!m_nStackSize || !m_pStackPtr->m_pNext); }
	// 获取当前元素数量
	inline size_t GetSize() const { return(m_nStackSize); }
	// 清理链栈, 会保留头结点
	void ClearStack();
	// 打印链栈
	void PrintStack() const;
private:
	// 释放栈的所有结点
	void ReleaseStack();
	// 释放栈的所有结点, 会保留头结点
	void ReleaseStackButHead();
	// ReleaseStack和ReleaseStackButHead的内部真实调用函数
	// fKeepHead代表是否保留头结点, 默认不保留
	void ReleaseImpl(bool fKeepHead = false);

	T GetValue(const STACKNODE &rcNodeVal) const { return(rcNodeVal.m_data); }
private:
	size_t		m_nStackSize;	// 栈的当前节点数(不包括头结点)
	PSTACKNODE	m_pStackPtr;	// 栈顶指针
};


template <typename T>
T CStack<T>::GetTop() const
{
	if (!m_nStackSize || !m_pStackPtr)
	{
		return(GetValue(STACKNODE()));
	}

	return(GetValue(*m_pStackPtr));
}


template <typename T>
void CStack<T>::ReverseStack()
{
	PSTACKNODE pLastNode = nullptr;
	T *pAry = nullptr;
	int iIdx = 0;

	if (!m_pStackPtr)
	{
		return;
	}
	pAry = new T[m_nStackSize];
	if (!pAry)
	{
		return;
	}
	pLastNode = m_pStackPtr;
	// 逆序保存当前栈节点内容
	while (pLastNode->m_pNext)
	{
		pAry[iIdx++] = pLastNode->m_data;
		pLastNode = pLastNode->m_pNext;
	}
	--iIdx;
	pLastNode = m_pStackPtr;
	while (pLastNode->m_pNext)
	{
		pLastNode->m_data = pAry[iIdx--];
		pLastNode = pLastNode->m_pNext;
	}
	if (pAry)
	{
		delete[] pAry;
		pAry = nullptr;
	}
}


template <typename T>
bool CStack<T>::Pop()
{
	PSTACKNODE pNextNode = nullptr;

	if (IsEmpty())
	{
		return(false);
	}

	if (m_pStackPtr)
	{
		// 释放栈顶元素后, 将栈顶指针指向最顶端
		pNextNode = m_pStackPtr->m_pNext;
		delete m_pStackPtr;
		m_pStackPtr = pNextNode;
	}
	--m_nStackSize;

	return(true);
}


template <typename T>
bool CStack<T>::Pop(STACKNODE &node)
{
	node.m_pNext = nullptr;

	return(Pop(node.m_data));
}


template <typename T>
bool CStack<T>::Pop(T &data)
{
	PSTACKNODE pNextNode = nullptr;

	if (IsEmpty())
	{
		return(false);
	}
	// 获取栈顶元素
	data = GetTop();
	
	if (m_pStackPtr)
	{
		// 释放栈顶元素后, 将栈顶指针指向最顶端
		pNextNode = m_pStackPtr->m_pNext;
		delete m_pStackPtr;
		m_pStackPtr = pNextNode;
	}
	--m_nStackSize;

	return(true);
}


template <typename T>
void CStack<T>::ClearStack()
{
	ReleaseStackButHead();
}


template <typename T>
void CStack<T>::ReleaseImpl(bool fKeepHead)
{
	PSTACKNODE pStackNode = nullptr;
	PSTACKNODE pNextNode = nullptr;

	pStackNode = m_pStackPtr;

	// 删除结点
	// 如果当前节点存在
	while (pStackNode)
	{
		// 保存下一个结点
		pNextNode = pStackNode->m_pNext;
		// 如果要保留头结点并且当前节点是头结点则break
		if (fKeepHead && !pNextNode)
		{
			// 重置栈顶指针
			m_pStackPtr = pStackNode;
			break;
		}
		// 释放当前结点
		delete pStackNode;
		pStackNode = nullptr;
		// 当前栈顶指针指向下一个节点
		pStackNode = pNextNode;
	}
	m_nStackSize = 0;

	return;
}


template <typename T>
void CStack<T>::ReleaseStackButHead()
{
	ReleaseImpl(true);
}


template <typename T>
void CStack<T>::ReleaseStack()
{
	ReleaseImpl();
}


template <typename T>
void CStack<T>::PrintStack() const
{
	PSTACKNODE pCurStackNode = nullptr;

	if (!m_pStackPtr)
	{
		return;
	}

	pCurStackNode = m_pStackPtr;
	// 如果next指向Null代表是头结点, 无需打印
	while (pCurStackNode->m_pNext)
	{
		cout << pCurStackNode->m_data << " ";
		pCurStackNode = pCurStackNode->m_pNext;
	}
	cout << endl;
}


template <typename T>
bool CStack<T>::Push(T data)
{
	STACKNODE stNode(data);

	return(Push(stNode));
}


template <typename T>
bool CStack<T>::Push(const STACKNODE &rcStackNode)
{
	PSTACKNODE pNode = nullptr;

	// 如果当前栈不存在, 直接调用构造函数构造
	if (!m_pStackPtr)
	{
		CStack(rcStackNode.m_data);
		// 如果栈顶指针不为空则代表成功
		if (m_pStackPtr && (m_nStackSize & 1))
		{
			return(true);
		}

		return(false);
	}

	// 如果当前栈存在
	// 分配一个新结点

	pNode = new STACKNODE(rcStackNode.m_data);
	if (!pNode)
	{
		return(false);
	}
	// 新结点成为新的栈顶
	pNode->m_pNext = m_pStackPtr;
	m_pStackPtr = pNode;

	m_nStackSize++;

	return(true);
}


template <typename T>
bool CStack<T>::CreateStack(const PSTACKNODE pcStackNodeAry, size_t nArySize)
{
	bool fOk = false;

	if (!pcStackNodeAry || !nArySize)
	{
		return(false);
	}

	// 由于不确定当前对象是否已经存在栈结构
	// 释放除了头结点外的所有节点
	ReleaseStackButHead();
	// 释放过后进行压栈操作, 如果有一个压栈操作失败
	// 释放所有已压入的元素空间并返回false
	for (size_t nIdx = 0; nIdx < nArySize; ++nIdx)
	{
		fOk = Push(pcStackNodeAry[nIdx]);
		if (!fOk)
		{
			ReleaseStack();
			return(false);
		}
	}

	return(true);
}


template <typename T>
bool CStack<T>::CreateStack(const T *pDataAry, size_t nArySize)
{
	PSTACKNODE	pStackNodeAry	= nullptr;
	bool		fOk				= false;

	if (!pDataAry || !nArySize)
	{
		return(false);
	}
	// 释放当前链栈, 除了头结点
	ReleaseStackButHead();
	// 压栈操作
	for (size_t nIdx = 0; nIdx < nArySize; ++nIdx)
	{
		if (!Push(pDataAry[nIdx]))
		{
			ReleaseStack();
			return(false);
		}
	}

	return(fOk);
}


template <typename T>
CStack<T>::~CStack()
{
	ReleaseStack();
}


template <typename T>
CStack<T>::CStack() :
	m_nStackSize(0), m_pStackPtr(nullptr)
{
	m_pStackPtr = new STACKNODE;
	if (m_pStackPtr)
	{
		m_pStackPtr->m_data = 0;
		m_pStackPtr->m_pNext = nullptr;
	}
}


template <typename T>
CStack<T>::CStack(T data) :
	m_nStackSize(1), m_pStackPtr(nullptr)
{
	PSTACKNODE pNewNode = nullptr;

	do
	{
		// 分配头结点
		m_pStackPtr = new STACKNODE;
		if (m_pStackPtr)
		{
			m_pStackPtr->m_data = 0;
			m_pStackPtr->m_pNext = nullptr;
			// 分配成功头结点开始分配第一个结点
			pNewNode = new STACKNODE(data);
			if (pNewNode)
			{
				// 把栈顶指针指向第一个结点
				pNewNode->m_pNext = m_pStackPtr;
				m_pStackPtr = pNewNode;

				return;
			}
		}
	} while (0);
	
	if (m_pStackPtr)
	{
		delete m_pStackPtr;
		m_pStackPtr = nullptr;
	}
	if (pNewNode)
	{
		delete pNewNode;
		pNewNode = nullptr;
	}
}