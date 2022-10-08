#include <vector>
#include <iostream>

using namespace std;
template <class K, class V>
struct SkipListNode {
    K key;
    V value;
    vector<SkipListNode<K,V>*> forward;
};
// 用heads是如何做到的

template <class K, class V>
class SkipList {
private:
    SkipListNode<K, V>* head;
    int maxlevel;
public:
    SkipList(int max_level) {
        maxlevel = max_level;
        head = new SkipListNode<K, V>;
        head->forward.resize(maxlevel);
        // todo 这个headKey要设置一个非常小的值
        for (int i = 0; i < maxlevel; i ++) {
            head->forward[i] = NULL;
        }
    }
    ~SkipList() {
        for (int i = 0; i < maxlevel; i ++) {
            SkipListNode<K,V>* pre;
            SkipListNode<K,V>* cur = head->forward[i];
            while(cur != NULL) {
                // head->forward[i] = cur->forward[i]; // 此行可要可不要，因为往前补往后
                pre = cur;
                cur = cur->forward[i];
                delete pre;
            }
        }
        delete head;
    }
    
    SkipListNode<K,V>* lookup(K key, V &value, bool &found) {
        found = false;
        int i = maxlevel - 1; 
        SkipListNode<K,V> *pre, *cur;
        pre = head;
        cur = head->forward[i];
        while (true){
            while(cur != NULL && cur->key < key) {
                pre = cur;
                cur = cur->forward[i];  // 同一层
            }
            if (cur != NULL && cur->key == key) {
                found = true;
                value = cur->value;
                return cur;
            }
            i --; // 往下一层
            if (i < 0) {
                break;
            }
            cur = pre->forward[i];
        }
        return NULL;
    }

    void del(K key){
        int i = maxlevel - 1; 
        SkipListNode<K, V> *cur, *pre;
        SkipListNode<K, V> *p = NULL;
        pre = head;
        cur = head->forward[i];
        while(true) {
            while(cur != NULL && cur->key < key) {
                pre = cur;
                cur = cur->forward[i];  // 同一层
            }
            if (cur != NULL && cur->key == key) {
                p = cur; // 记录要删除的node节点
                pre->forward[i] = cur->forward[i];
            }
            i --; // 往下一层
            if (i < 0) {
                break;
            }
            cur = pre->forward[i];
        }
        if (p) {
            delete p;    
        }
    }

    // 考虑边界情况
    // 插入第一个位置
    // 插入到最后
    SkipListNode<K, V>*  insert(K key, V value) {
        SkipListNode<K, V>* node = new SkipListNode<K, V>;
        SkipListNode<K, V> *cur, *pre;
       
        node->key = key;
        node->value = value;
        int j = rand() % maxlevel;
        cout << "insert level:" << j  << "  key:" << key << " value:" << value << endl;
     
        vector<SkipListNode<K, V>*> update(j+1);  // 记录指针
        int i = j;

        pre = head;
        cur = head->forward[i];
        bool is_insert = true;
        while(true) {
            // 1.此处头尾指针使用minkey和maxkey，避免了判断null，使用哨兵减少了一倍判断
            // 2.然后pre和cur本身有关联，只要一个pre指针即可，pre->forward[i].key <= key
            // 3.insert的思路也更加清晰
            // 找到insert的target位置
            // 然后判断是update还是insert
            // 然后new node进行插入
            // 4.cur_max_level, 这个字段也不知道有啥用， 按照增删来变更当前的cur_max_level
            // 5.除了head指针，还有tail指针。tail指针可以是null也可以不是null。 不是null指定范围maxkey
            // 6.获取插入层的方法不一样（随机分布）
            while(cur != NULL && cur->key <= key) {
                pre = cur;
                cur = cur->forward[i]; // 同一层
            }
            if (cur != NULL && cur->key == key) {
                cur->value = value;
                is_insert = false;
                break;
            }
            // node->forward[i] = cur; // 这样写是不对的，因为node的forward没有分配空间，会报segment fault
            update[i] = cur;
            pre->forward[i] = node;
            i --; // 往下一层
            if (i < 0) {
                break;
            }
            cur = pre->forward[i];
        }
        if (is_insert) {
            node->forward = update;
        }
        return node;
    }

    void print(){
        SkipListNode<K, V>* cur;
        for (int i = maxlevel -1; i >= 0; i --) {
            cur = head->forward[i];
            cout << "level " << i;
            while (cur != NULL) {
                cout << "\t" << cur->key << " " << cur->value << endl;
                cur = cur->forward[i];
            }
            cout << endl;
        }
    }
};

void test_skiplist() {
    int max_level = 5;
    SkipList<int, int> *s = new SkipList<int, int>(max_level);
    s->insert(10, 5);
    s->insert(3, 2);
    s->insert(13, 8);
    s->print();
    int value;
    bool found;
    int key = 13;
    s->lookup(key, value, found);
    cout << "key:" << key << " value:" << value << " found:" << found << endl;
    s->del(key);
    s->print();
    s->lookup(key, value, found);
    cout << "key:" << key << " value:" << value << " found:" << found << endl;
    s->print();
}

int main() {
    test_skiplist();
    return 0;
}