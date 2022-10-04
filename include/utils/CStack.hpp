#pragma once
// ��ջʵ����

template <typename T>
class CStack
{
public:
	// ջ�������ݽṹ
	typedef struct _STACKNODE
	{
		_STACKNODE() = default;
		_STACKNODE(T data) : m_data(data), m_pNext(nullptr) {}
		T m_data;
		struct _STACKNODE *m_pNext;
	} STACKNODE, *PSTACKNODE;
	// �޲ι��캯��
	CStack();
	// ���ι��캯��
	CStack(T data);
	// ��������
	~CStack();
	// ��ջ��Ԫ������
	void ReverseStack();
	// ��ջ��ѹ��һ�����
	bool Push(const STACKNODE &rcStackNode);
	bool Push(T data);
	// ��ջ�ڵ�������
	bool Pop();
	bool Pop(T &data);
	bool Pop(STACKNODE &node);
	// ����һ��ջ
	bool CreateStack(const PSTACKNODE pcStackNodeAry, size_t nArySize);
	bool CreateStack(const T *pDataAry, size_t nArySize);
	// ����ջ���������
	T GetTop() const;
	// �ж���ջ�Ƿ�Ϊ��
	inline bool IsEmpty() const { return(!m_nStackSize || !m_pStackPtr->m_pNext); }
	// ��ȡ��ǰԪ������
	inline size_t GetSize() const { return(m_nStackSize); }
	// ������ջ, �ᱣ��ͷ���
	void ClearStack();
	// ��ӡ��ջ
	void PrintStack() const;
private:
	// �ͷ�ջ�����н��
	void ReleaseStack();
	// �ͷ�ջ�����н��, �ᱣ��ͷ���
	void ReleaseStackButHead();
	// ReleaseStack��ReleaseStackButHead���ڲ���ʵ���ú���
	// fKeepHead�����Ƿ���ͷ���, Ĭ�ϲ�����
	void ReleaseImpl(bool fKeepHead = false);

	T GetValue(const STACKNODE &rcNodeVal) const { return(rcNodeVal.m_data); }
private:
	size_t		m_nStackSize;	// ջ�ĵ�ǰ�ڵ���(������ͷ���)
	PSTACKNODE	m_pStackPtr;	// ջ��ָ��
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
	// ���򱣴浱ǰջ�ڵ�����
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
		// �ͷ�ջ��Ԫ�غ�, ��ջ��ָ��ָ�����
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
	// ��ȡջ��Ԫ��
	data = GetTop();
	
	if (m_pStackPtr)
	{
		// �ͷ�ջ��Ԫ�غ�, ��ջ��ָ��ָ�����
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

	// ɾ�����
	// �����ǰ�ڵ����
	while (pStackNode)
	{
		// ������һ�����
		pNextNode = pStackNode->m_pNext;
		// ���Ҫ����ͷ��㲢�ҵ�ǰ�ڵ���ͷ�����break
		if (fKeepHead && !pNextNode)
		{
			// ����ջ��ָ��
			m_pStackPtr = pStackNode;
			break;
		}
		// �ͷŵ�ǰ���
		delete pStackNode;
		pStackNode = nullptr;
		// ��ǰջ��ָ��ָ����һ���ڵ�
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
	// ���nextָ��Null������ͷ���, �����ӡ
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

	// �����ǰջ������, ֱ�ӵ��ù��캯������
	if (!m_pStackPtr)
	{
		CStack(rcStackNode.m_data);
		// ���ջ��ָ�벻Ϊ�������ɹ�
		if (m_pStackPtr && (m_nStackSize & 1))
		{
			return(true);
		}

		return(false);
	}

	// �����ǰջ����
	// ����һ���½��

	pNode = new STACKNODE(rcStackNode.m_data);
	if (!pNode)
	{
		return(false);
	}
	// �½���Ϊ�µ�ջ��
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

	// ���ڲ�ȷ����ǰ�����Ƿ��Ѿ�����ջ�ṹ
	// �ͷų���ͷ���������нڵ�
	ReleaseStackButHead();
	// �ͷŹ������ѹջ����, �����һ��ѹջ����ʧ��
	// �ͷ�������ѹ���Ԫ�ؿռ䲢����false
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
	// �ͷŵ�ǰ��ջ, ����ͷ���
	ReleaseStackButHead();
	// ѹջ����
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
		// ����ͷ���
		m_pStackPtr = new STACKNODE;
		if (m_pStackPtr)
		{
			m_pStackPtr->m_data = 0;
			m_pStackPtr->m_pNext = nullptr;
			// ����ɹ�ͷ��㿪ʼ�����һ�����
			pNewNode = new STACKNODE(data);
			if (pNewNode)
			{
				// ��ջ��ָ��ָ���һ�����
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