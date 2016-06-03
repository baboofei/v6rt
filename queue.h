#define CP_LESS -1
#define CP_EQUAL 0
#define CP_LARGER 1

enum nodestate {NEW, OLD, USING};

struct node {
	struct node *nxt;		// Pointer to next node
	enum nodestate state;		// State of node
	void *data;			// Pointer to data
};

struct queue {
	char *name;					// Name of queue
	struct node nodes[NNODES];			// Actual memory space for nodes
	struct node *head;				// Pointer to head of queue
	struct spinlock lock;				// Spinlock for this queue
};

