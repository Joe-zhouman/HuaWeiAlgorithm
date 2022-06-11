//
// Created by joe on 22-6-10.
//

#ifndef EMBEDDED_2022_SKIPLIST_H
#define EMBEDDED_2022_SKIPLIST_H

#include "List.h"
#include "Quadlist.h"
#include <stdlib.h>

namespace simple_dsa {
    template<typename T>
    class Skiplist : public List<Quadlist<T> *> {
    protected:
        bool SkipSearch(ListNode<Quadlist<Entry < KEY, VAL>> *

        > *&qlist, QuadlistNode<Entry < KEY, VAL>> *&p,
        KEY &k
        );

    public:
        int Size() const { return this->Empty() ? 0 : this->Last()->data_->Size(); }

        int Level() { return List::Size(); }

        bool Put(KEY, VAL);

        VAL *Get(KEY key);

        bool Remove(KEY key);
    };

    template<typename KEY, typename VAL>
    VAL *Skiplist<KEY, VAL>::Get(KEY key) {
        if (this->Empty())return nullptr;
        ListNodePosi<Quadlist<Entry < KEY, VAL>> *> qlist = this->First();
        QlistNodePosi<Entry < KEY, VAL>>
        p = qlist->data_->First();
        return SkipSearch(qlist, p, key) ? &(p->entry_.value_) : nullptr;
    }

/******************************************************************************************
 * Skiplist词条查找算法（供内部调用）
 * 入口：qlist为顶层列表，p为qlist的首节点
 * 出口：若成功，p为命中关键码所属塔的顶部节点，qlist为p所属列表
 *       否则，p为所属塔的基座，该塔对应于不大于k的最大且最靠右关键码，qlist为空
 * 约定：多个词条命中时，沿四联表取最靠后者
 ******************************************************************************************/
    template<typename T>
    bool Skiplist<T>::SkipSearch(ListNode<Quadlist<Entry < KEY, VAL>> *

    > *&qlist,//从指定层qlist的
    QuadlistNode<Entry < KEY, VAL>> *&p,
    KEY &k
    ) {//首节点p出发
    while (true) {//在每一层
    while (p->
    succ_ &&(p
    ->entry_.key_ <= k)) {
    p = p->succ_;//从前向后查找
//直到出现更大的key或溢出至trailer
}
p = p->pred_;//此时倒回一步，即可判断是否
if (p->
pred_ &&(k
== p->entry_.key_))return true;//命中
qlist = qlist->succ_;//否则转入下一层
if (!qlist->succ_)return false;//若已到穿透底层，则意味着失败
p = (p->pred_) ? p->below_ : qlist->data_->First();//否则转至当前塔的下一节点
}
}

template<typename KEY, typename VAL>
bool Skiplist<KEY, VAL>::Put(KEY key, VAL val) {
    Entry <KEY, VAL> e = Entry<KEY, VAL>(key, val);//待插入的词条（将被随机地插入多个副本）
    if (this->Empty()) {
        this->InsertAsFirst(new Quadlist <Entry<KEY, VAL>>);//插入首个Entry
    }
    ListNodePosi < Quadlist < Entry < KEY, VAL >> * > qlist = this->First();//从顶层四联表的
    QlistNodePosi <Entry<KEY, VAL>> p = qlist->data_->First();//首节点出发
    if (this->SkipSearch(qlist, p, key)) {//查找适当的插入位置（不大于关键码k的最后一个节点p）
        while (p->below_)p = p->below_;//若已有雷同词条，则需强制转到塔底
    }
    qlist = this->Last();//以下，紧邻于p的右侧，一座新塔将自底而上逐层生长
    QlistNodePosi <Entry<KEY, VAL>> b = qlist->data_->InsertAfterAbove(e, p);//新节点b即新塔基座
    while (rand() & 1) {//经投掷硬币，若确定新塔需要再长高一层，则
        while (qlist->data_->Valid(p) && !p->above_)p = p->pred_;//找出不低于此高度的最近前驱
        if (!qlist->data_->Valid(p)) {//若该前驱是header
            if (qlist == this->First())//且当前已是最顶层，则意味着必须
                this->InsertAsFirst(new Quadlist <Entry<KEY, VAL>>);//首先创建新的一层，然后
            p = qlist->pred_->data_->First()->pred_; //将p转至上一层Skiplist的header
        } else {//否则，可径自
            p = p->above_;//将p提升至该高度
        }
        qlist = qlist->pred_; //上升一层，并在该层
        b = qlist->data_->InsertAfterAbove(e, p, b);//将新节点插入p之后、b之上
    }
    return true;//Dictionary允许重复元素，故插入必成功——这与Hashtable等Map略有差异
}

} // simple_dsa

#endif //EMBEDDED_2022_SKIPLIST_H
