#include "common.h"
#include <vector>

struct ItemNode
{
    uint32 id;
};

class ItemNodePool : public XDSingleton<ItemNodePool>
{
public:
    ItemNode *getNode()
    {
        ItemNode *item = NULL;
        if (freeList_.empty()) {
            item = new ItemNode();
        } else {
            item = freeList_.back();
            freeList_.pop_back();
        }
        return item;
    }

    void returnNode(ItemNode *item)
    {
        freeList_.push_back(item);
    }

private:
    std::vector<ItemNode*> freeList_;
};

class CreateMsg : XDIRunnable
{
public:
    uint32 run()
    {
        return 1;
    }
};

uint32 count = 0;

int main(int argc, char **argv)
{
    XDBlockingQueue<ItemNode*> queue;
    ItemNode *item = ItemNodePool::instance().getNode();
    item->id = ++count;
    queue.push(item);
    ItemNode* t = queue.get();
    return 0;
}
