#ifndef BST_H_INCLUDED
#define BST_H_INCLUDED

#include <memory>
#include <functional>

template <class _Key, class _Compare = std::less<_Key> >
class BST {
public:
    typedef _Key key_type;
    typedef _Compare key_compare;

    BST(const key_compare& comp = key_compare());
    bool insert(key_type value);
    bool find(key_type value);
    bool erase(key_type value);
    key_type& min(void);
    key_type& max(void);
private:
    const key_compare& comp;
    class Node {
    public:
        Node(key_type x);
        key_type& min();
        key_type& max();
        key_type value;
        std::shared_ptr<Node> parent;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
    };
    typedef std::shared_ptr<Node> shared_ptr;
    shared_ptr root;

    inline bool compe(key_type v1, key_type v2) { return !(comp(v1,v2) || comp(v2,v1)); }
    shared_ptr next(shared_ptr x);
    shared_ptr getNode(key_type value);
};

#endif // BST_H_INCLUDED