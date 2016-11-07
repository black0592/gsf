#include "network_connect.h"


gsf::NetworkConnect::~NetworkConnect()
{

}

gsf::NetworkConnect::NetworkConnect()
{

}

gsf::NetworkConnect* gsf::NetworkConnect::instance_ = NULL;

gsf::NetworkConnect& gsf::NetworkConnect::instance()
{
	if (instance_ == NULL)
	{
		instance_ = new gsf::NetworkConnect();
	}
	return *instance_;
}

void gsf::NetworkConnect::cq_init(CQ *cq)
{
	cq->head = NULL;
	cq->tail = NULL;
}


gsf::CQ_ITEM * gsf::NetworkConnect::cq_pop(CQ *cq)
{
	CQ_ITEM *item;

	cq->lock.lock();
	item = cq->head;
	if (NULL != item) {
		cq->head = item->next;
		if (NULL == cq->head)
			cq->tail = NULL;
	}
	cq->lock.unlock();

	return item;
}


void gsf::NetworkConnect::cq_push(CQ *cq, CQ_ITEM *item)
{
	item->next = NULL;

	cq->lock.lock();
	if (NULL == cq->tail)
		cq->head = item;
	else
		cq->tail->next = item;
	cq->tail = item;
	cq->lock.unlock();
}


gsf::CQ_ITEM * gsf::NetworkConnect::cqi_new(void)
{
	CQ_ITEM *item = NULL;
	cqi_freelist_lock.lock();
	if (cqi_freelist) {
		item = cqi_freelist;
		cqi_freelist = item->next;
	}
	cqi_freelist_lock.unlock();

	if (NULL == item) {
		int i;

		/* Allocate a bunch of items at once to reduce fragmentation */
		item = (CQ_ITEM*)malloc(sizeof(CQ_ITEM)* ITEMS_PER_ALLOC);
		if (NULL == item) {
			//err dispose
			return NULL;
		}

		/*
		* Link together all the new items except the first one
		* (which we'll return to the caller) for placement on
		* the freelist.
		*/
		for (i = 2; i < ITEMS_PER_ALLOC; i++)
			item[i - 1].next = &item[i];

		cqi_freelist_lock.lock();
		item[ITEMS_PER_ALLOC - 1].next = cqi_freelist;
		cqi_freelist = &item[1];
		cqi_freelist_lock.unlock();
	}

	return item;
}

void gsf::NetworkConnect::cqi_free(CQ_ITEM *item)
{
	cqi_freelist_lock.lock();
	item->next = cqi_freelist;
	cqi_freelist = item;
	cqi_freelist_lock.unlock();
}
