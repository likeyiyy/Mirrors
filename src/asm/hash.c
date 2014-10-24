/*************************************************************************
	> File Name: hash.c
	> Author: likeyi
	> Mail: likeyi@sina.com 
	> Created Time: Tue 17 Jun 2014 04:36:47 PM CST
 ************************************************************************/

#include "includes.h"

typedef label_t hash_item_t ;


static inline int compare_label(label_t * item , label_t * label)
{
    label_t * l = (label_t *)item;
    if(!strcmp(l->name,label->name))
    {
        return 0;
    }
    return 1;

}
struct blist * find_list(struct list_head * head, label_t * label)
{
    struct list_head * p;
    struct blist * node;
    list_for_each(p,head)
    {
        node = list_entry(p,struct blist,listhead);
        if(compare_label(node->item, label) == 0)
        {
            return node;
        }
    }
    return NULL;
}
static void exit_if_ptr_is_null(void * ptr,const char * message) 
{
    if(ptr == NULL)
    {   
        printf("%s\n",message);
        exit(-1);
    } 
}
  
static int next_prime(int x)
{
     long i, j;
     int f;
     
     x = (x==0)?1:x;
     i = x;
     while (i++)
     {
          f=1;
          for (j=2; j<i; j++)
          {
               if (i%j == 0)
               {
                    f=0;
                    break;
                    
               }
          }
          if (f)
          {
             return (int)i;
          }
     }
    return 0;
}

/*
* 1. 创建
* */
hash_table * hash_create(int num)
{
    hash_table * result;
    bucket_t * b;
    int bytes;
    int i;
    result = malloc(sizeof(hash_table));
    exit_if_ptr_is_null(result,"Initizial hash table Error"); 
    num = next_prime(num);

    bytes = num * sizeof(bucket_t);

    result->buckets = b = malloc(bytes);
    exit_if_ptr_is_null(result->buckets,"hash table buckets alloc error");

    result->num_buckets = num;

    i = num;
    while(--i >= 0)
    {
        INIT_LIST_HEAD(&b->list); 
        pthread_mutex_init(&b->lock, NULL);
        b->count = 0;
        b++;
    }
    return result;
}
#if 1
/*
* 2. 查找
* */
void * hash_lookup_item(hash_table * ht, uint32_t key, void * value)
{
    struct list_head * ll;
    hash_item_t * label = (label_t *)value;
    bucket_t * bucket = &ht -> buckets[key % ht->num_buckets];
    ll = &bucket->list;
    return (void *) find_list(ll,label);    
}
/*
* 3. 插入
* 严格说来,blist 完全没有任何问题。
* */
int  hash_add_item(hash_table ** htp, uint32_t key, label_t * label )
{
    struct list_head * ll;
    struct blist * blist;
    hash_table * ht = *htp;
    /*
     * 
     * */
    bucket_t * bucket = &ht -> buckets[key % ht->num_buckets];
    ll = &bucket->list;
    blist = find_list(ll,label);    
    /*
     * 假如不存在于链表中。
     * */
    if(!blist)
    {
        ++bucket->count;
        blist = malloc(sizeof(struct blist));
        blist->item = malloc(sizeof(hash_item_t));
        blist->item->name = malloc(strlen(label->name) + 1);
        strcpy(blist->item->name, label->name);
        blist->item->line = label->line;
        blist->item->real_line = label->real_line;
        INIT_LIST_HEAD(&blist->listhead);
        list_add_tail(&blist->listhead,ll);
        ++bucket->count;
    }
    /* 
    * Found it, and memcpy it.
    * 主要是这一段无法公共化，其他的hash函数可能找到后，并不会copy
    * */
    else
    {

        printf("Error: %s redefined, first define is line %d\n",
               label->name,blist->item->real_line);
        exit(-2);
    }
    return 0;
}
#endif
/*
* 4. 遍历
* */

void  hash_travel_delete(hash_table * ht)
{
    register int i = 0;
    register bucket_t * bucket;
    bucket = ht->buckets;
    while(i++ < ht->num_buckets)
    {

        pthread_mutex_lock(&bucket->lock);
        pthread_mutex_unlock(&bucket->lock);
        bucket++;
    }
}



/*
 * unsigned int hash_count(hash_table *ht)
 *
 * Return total number of elements contained in hash table.
 */

uint32_t hash_count(hash_table * ht)

{
    register int i = 0;
    register int cnt = 0;
    register bucket_t *bucket;

    bucket = ht->buckets;
    while (i++ < ht->num_buckets)
    {
	    cnt += bucket->count;
	    bucket++;
    }

    return cnt;
}
