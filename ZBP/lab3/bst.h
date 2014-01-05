#include <memory>

template <class T>
class BST {
public:
	BST(const std::key_compare& comp = std::key_compare());
	~BST();
	bool insert(T x);
	bool find(T x);
	bool erase(T x);
	T& min(void);
	T& max(void);
private:
	std::shared_ptr<T> item;
};
