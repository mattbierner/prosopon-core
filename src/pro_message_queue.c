#include "pro_message_queue.h"


#pragma mark Private

typedef struct pro_message_node pro_message_node;


struct pro_message_queue
{
    pro_message_node* front;
};


struct pro_message_node
{
    pro_message_node* next;
    pro_ref message;
    pro_ref actor;
};

static pro_message_node* pro_message_node_new(pro_state_ref s,
    pro_ref message, pro_ref actor, pro_message_node* next)
{
    pro_message_node* t = pro_alloc(s, 0, sizeof(*t));
    if (!t) return 0;

    t->message = message;
    t->next = next;
    t->actor = actor;
    return t;
}

#pragma mark -
#pragma mark Internal

PRO_INTERNAL
pro_message_queue* pro_message_queue_new(pro_state_ref s)
{
    pro_message_queue* t = pro_alloc(s, 0, sizeof(*t));
    if (!t) return 0;
    
    t->front = 0;
    return t;
}


PRO_INTERNAL
void pro_message_queue_free(pro_state_ref s, pro_message_queue* t)
{
    for (pro_message_node* node = t->front; node; node = node->next)
        pro_alloc(s, node, 0);
        
    pro_alloc(s, t, 0);
}


PRO_INTERNAL
void pro_message_queue_enqueue(pro_state_ref s,
    pro_message_queue* t, pro_ref msg, pro_ref actor) 
{    
    pro_retain(s, msg);
    pro_retain(s, actor);

    pro_message_node* parent = t->front;
    if (!parent)
        t->front = pro_message_node_new(s, msg, actor, 0);
    else
    {
        while (parent->next)
            parent = parent->next;
        parent->next = pro_message_node_new(s, msg, actor, 0); 
    }
}


PRO_INTERNAL
pro_ref pro_message_queue_dequeue(pro_state_ref s,
    pro_message_queue* t, PRO_OUT pro_ref* actor)
{
    if (!t->front)
        return 0;
    else
    {
        pro_message_node* front = t->front;
        pro_ref msg = front->message;
        *actor = front->actor;
        t->front = front->next;
        
        pro_alloc(s, front, 0);
        
        return msg;
    }
}


PRO_INTERNAL
int pro_message_queue_is_empty(pro_state_ref s,
    pro_message_queue* t)
{
    return (t->front == 0);
}
