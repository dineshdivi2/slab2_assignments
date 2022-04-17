#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

struct LList{
    void* mem;
    struct LList *next;
};

struct LList* Init_node(void *p,struct LList* next);
void insertHead(struct LList **head, void *ptr);
void insertTail (struct LList **headRef, void* ptr);
void deleteNode (struct LList **head, void* ptr);
void destroyList(struct LList **head);
void List_sort(struct LList **head);
void List_sort_size(struct LList **head);
void printLL(struct LList **head);


void swap(struct LList *a, struct LList *b){
        void* swap = a->mem;
        a->mem = b->mem;
        b->mem = swap;
}
enum allocation_algorithm {FIRST_FIT};

struct Myalloc {
    enum allocation_algorithm aalgorithm;
    int size;
    void* memory;
    struct LList *Alloc;
    struct LList *Free;
    void* startPtr;
    void* endPtr;
};

struct Myalloc myalloc;

struct LList* Init_node( void *p, struct LList* next){
	struct LList *h = (struct LList*) malloc(sizeof(struct LList));
	h->mem = p;
	h->next = next;
	return h;
}

void insertHead(struct LList **head, void *ptr){
	struct LList *n;
	n = Init_node(ptr, *head);
	*head = n;
}

void insertTail (struct LList **headRef, void *ptr){
	struct LList* curr = *headRef;
	if(curr == NULL){
		insertHead(headRef, ptr);
	}
	else if(curr->next == NULL){
		struct LList *n = Init_node(ptr,NULL);
		curr->next = n;
	}
	else{
		while(curr->next != NULL){
			curr = curr->next;
		}
		insertHead(&(curr->next),ptr);
	}
} 
void deleteNode (struct LList **head, void* ptr){ 
	if(*head == NULL || ptr == NULL){
		return;
	} 
	struct LList* temp = *head; 
	if(ptr == temp->mem){
		*head = temp->next;  
		free(temp);	 
		return;
	} 
	while(temp->next->mem != ptr){
		temp = temp->next;
	}	 
	struct LList *nextOfnode = temp->next->next;
	free(temp->next);
	temp->next = nextOfnode;
}

void destroyList(struct LList **head){
	struct LList* temp = *head;
	if(temp == NULL){
		return;
	}
	else{
		void* t;
		while(temp != NULL){
			t = temp->mem;
			deleteNode(head,t);
			temp = temp->next;
		}
	}
} 
void List_sort (struct LList **head){
    int swapped; 
    struct LList *ptr1; 
    struct LList *lptr = NULL;
    struct LList *start = *head;  
    if (start == NULL) 
        return; 
  
    do
    { 
        swapped = 0; 
        ptr1 = start; 
  
        while (ptr1->next != lptr) 
        { 
            if (*(long *)(ptr1->mem) > *(long *)(ptr1->next->mem)) 
            {  
                swap(ptr1, ptr1->next); 
                swapped = 1; 
            } 
            ptr1 = ptr1->next; 
        } 
        lptr = ptr1; 
    } 
    while (swapped); 
} 
void List_sort_size(struct LList **head){
	int swapped; 
    struct LList *ptr1; 
    struct LList *lptr = NULL;
    struct LList *start = *head; 
   
    if (start == NULL) 
        return; 
  
    do
    { 
        swapped = 0; 
        ptr1 = start; 
  
        while (ptr1->next != lptr) 
        { 
            if (*(long *)((ptr1->mem)-8) > *(long *)((ptr1->next->mem)-8)) 
            {  
                swap(ptr1, ptr1->next); 
                swapped = 1; 
            } 
            ptr1 = ptr1->next; 
        } 
        lptr = ptr1; 
    } 
    while (swapped); 
OBOB}

OBvoid printLL(struct LList **head){
OB	struct LList *h = *head;
	if(h == NULL){
OB		printf("Empty List\n");
		return;
OB	}
OB	int i = 1;
OB	while(h != NULL){
		if( h->mem == (myalloc.endPtr + 1)){
			printf("addr: %p size: 0\n",h->mem);
		}
		else{
OBOB			printf("Node[%d]: mem: %p size: %ld\n",i,h->mem, *(long *)((h->mem)-8));
OB		}
		h = h->next;
OB		i++;
OBOB	}
}
OBvoid initialize_allocator(int _size);

OBvoid* allocate(int _size);
void deallocate(void* _ptr);
int available_memory();
void print_statistics();
OBint compact_allocation(void** _before, void** _after);
void destroy_allocator();

pthread_mutex_t lock;
 
long getSize(struct LList* node){
    void* temp = (node->mem)-8;
    return *(long *)temp;
OBOB}  
void setSize(struct LList* node, long *s){
OBOB    void *t = (node->mem)-8;
OB    memcpy(t,&s,sizeof(long));
} 
long aSize(){ 
OBOB    pthread_mutex_lock(&lock);
OB    struct LList* A = myalloc.Alloc;
OB    long s = 0;
OB    long hold;
    while(A != NULL){
        memcpy(&hold,A->mem-8,sizeof(long));
        s += hold;
OBOB        A = A->next;
    }
OBOBOBOBOB    pthread_mutex_unlock(&lock);
    return s;
}

int aCount(){  
    pthread_mutex_lock(&lock);
    struct LList* A = myalloc.Alloc;
    int c = 0;
    while(A != NULL){
OBOBOBOB        c++;
OB        A = A->next;
OBOB    }
    pthread_mutex_unlock(&lock);
OBOBOB    return c;
}
OBOBOB
int fCount(){  
    pthread_mutex_lock(&lock);
OBOBOBOBOBOBOBOBOBOB    struct LList* F = myalloc.Free;
    if(F->mem == (myalloc.endPtr + 1)){
OB        pthread_mutex_unlock(&lock);
        return 0;
OBOBOBOBOBOBOB    }
    else{
OBOBOBOBOB        int c = 0;
        while(F != NULL){
            c++;
            F = F->next;
OBOBOBOB        }
        pthread_mutex_unlock(&lock);
OBOB        return c;
    }
OB}
OB
int minfSize(){ 
    pthread_mutex_lock(&lock);
OBOB    struct LList* F = myalloc.Free;
OB    if(F->mem == (myalloc.endPtr + 1)){
OB        pthread_mutex_unlock(&lock);
        return 0;
OBOB    }
OB    long min = *(long*)(F->mem-8);
    long hold;
OBOB    F = F->next;
OB    while(F != NULL){
        hold = *(long*)(F->mem-8);
        if(hold <= min){
            min = hold;
        }
OBOBOBOBOB        F = F->next;
    }
OBOB    pthread_mutex_unlock(&lock);
    return min;
}

OBOBOBOBOBOBint maxfSize(){ 
    pthread_mutex_lock(&lock);
     struct LList* F = myalloc.Free;
    if(F->mem == (myalloc.endPtr + 1)){
OBOBOBOBOB        pthread_mutex_unlock(&lock);
        return 0;
    }
OB    long max = *(long*)(F->mem-8);
    long hold;
    F = F->next;
OBOB    while(F != NULL){
        hold = *(long*)(F->mem-8);
        if(hold >= max){
            max = hold;
        }
OBOB        F = F->next;
    }
    pthread_mutex_unlock(&lock);
    return max;
} 
long fSize(){  
    pthread_mutex_lock(&lock);
OBOBOB    struct LList* F = myalloc.Free;
    if(F->mem == (myalloc.endPtr + 1)){
        pthread_mutex_unlock(&lock);
OB        return 0;
    }
OBOBOBOBOBOB    else{
OBOB        long s = 0;
OBOB        long hold;
OBOBOB        while(F != NULL){
            memcpy(&hold,F->mem-8,sizeof(long));
OBOBOBOB            s += hold;
            F = F->next;
        }
        pthread_mutex_unlock(&lock);
        return s;
    }
} 
void compactionMerge(struct LList* a, long asize, struct LList* b){
    if(a == NULL || b == NULL){
        return;
    }
    else{
        if(a->mem + asize == b->mem){ 
            long n = asize + getSize(b);
            memcpy(a->mem-8,&n,sizeof(long));
OBOBOB            a->next = b->next;
OBOB            deleteNode(&(myalloc.Free),b->mem);  
OB        }
    }
OBOB}

void fix(struct LList* a){
    void* t = a->mem - 8;
OBOB    void* t1 = a->next->mem - 8;
    long sa,sb,res;
OBOBOBOBOB    memcpy(&sa,t,sizeof(long));     
    memcpy(&sb,t1,sizeof(long));    
    res = sa+sb;                    
    memcpy(t,&res,sizeof(long));
    a->next = a->next->next;       
}

bool isContigious(struct LList* a, struct LList* b){
    bool retval = false;
    if(b == NULL || a == NULL){
        return retval;
    }
OBOBOBOBOB    if(a->mem +getSize(a) == b->mem){
        retval = true;
OBOBOB    }
OBOB    return retval;
}

OBOBvoid mergeMem(){
    List_sort(&(myalloc.Free)); 
    struct LList* current = myalloc.Free;
OBOB    while(current && current->next){
        if(isContigious(current,current->next)){
            fix(current);
OBOBOBOB        }
        current = current->next;        
OBOBOBOBOBOBOBOB    }
}
OBOBvoid initialize_allocator(int _size) {
OBOBOBOBOBOB    assert(_size > 0);
    myalloc.aalgorithm = FIRST_FIT;
    myalloc.size = _size;                                                                           
    myalloc.memory = malloc((size_t)myalloc.size);          
    void *currMem = myalloc.memory;                                                                 
    long s = _size;                                                                                
    memset(myalloc.memory,'0',myalloc.size);                                                
    memcpy(currMem,&s,sizeof(long));                               
    currMem += 8;  
    struct LList* F = Init_node(currMem,NULL);
    myalloc.Free = F;
    myalloc.startPtr = myalloc.memory;
    myalloc.endPtr = myalloc.memory + _size-1;
}                                                                                                                                                                             

void destroy_allocator() {
    pthread_mutex_lock(&lock);
    free(myalloc.memory);
    destroyList(&(myalloc.Free));
    destroyList(&(myalloc.Alloc));
    pthread_mutex_unlock(&lock);
   
}

void* allocate(int _size) {
    pthread_mutex_lock(&lock);         
    void* ptr = NULL;                  
    void* hptr;                         
    void* temp;                         
    long s = _size+8;            
    long size = _size;                
    long fsize;                         
    bool flag = false;
    struct LList* tempF = myalloc.Free; 
    if( tempF->mem == (myalloc.endPtr + 1)){
            pthread_mutex_unlock(&lock);
            return NULL;
    }
    switch(myalloc.aalgorithm){
        case FIRST_FIT:
            while(tempF != NULL){
                if(getSize(tempF) >= s){
                    flag = true;
                    break;
                }
                tempF = tempF->next;
            }  
            
            if(flag){ 
                ptr = tempF->mem;    
                hptr = ptr-8;
                memcpy(&fsize, hptr,sizeof(long)); 
                fsize -= s;        
                if(fsize > 8){
                    memcpy(hptr,&s,sizeof(long));   
                    insertTail(&(myalloc.Alloc),ptr);
                    temp = ptr + s;  
                    tempF->mem = temp;
                    temp -= 8; 
                    memcpy(temp,&fsize,sizeof(long)); 
                }
                else{
                    s += fsize;
                    memcpy(hptr,&s,sizeof(long));   
                    insertTail(&(myalloc.Alloc),ptr);
                    destroyList(&(myalloc.Free));
                    myalloc.Free = Init_node(myalloc.endPtr + 1,NULL);
                }
            }
            else{
                pthread_mutex_unlock(&lock);
                return NULL;
            }
            break;
    }
    pthread_mutex_unlock(&lock);
    return ptr;
}


void deallocate(void* _ptr) {
    pthread_mutex_lock(&lock);
    if(_ptr == NULL){
        printf("NULL pointer recieved by deallocator()\n");
        pthread_mutex_unlock(&lock);
        return;
    }
    if((myalloc.Free)->mem == myalloc.endPtr + 1){
        myalloc.Free->mem = _ptr;
        deleteNode(&(myalloc.Alloc),_ptr);  
        mergeMem();   
        pthread_mutex_unlock(&lock);
        return;    
    }
    else{
        insertTail(&(myalloc.Free),_ptr);  
        deleteNode(&(myalloc.Alloc),_ptr);  
        mergeMem();        
        pthread_mutex_unlock(&lock);
        return;
    } 
}

int compact_allocation(void** _before, void** _after) {
    pthread_mutex_lock(&lock);
    int compacted_size = 0;
    int i = 0;
    struct LList* A = myalloc.Alloc;
    struct LList* F = myalloc.Free;
   
    if(A == NULL || F == NULL){  
        pthread_mutex_unlock(&lock);
        return 0;
    }

    void* tFreeMem = F->mem;
    void* tAllocMem;

    while(A != NULL){   
        tAllocMem = A->mem;
        if(*(long*)tAllocMem > *(long*)tFreeMem){
            break;
        }
        _before[i] = A->mem;
        _after[i] = A->mem;
        i++;
        A = A->next;
    }
    long sizeA,sizeF;
    while(A != NULL){
        _before[i] = A->mem;
        sizeA = getSize(A);     
        sizeF = getSize(F);      
        memcpy(F->mem,A->mem,(size_t)(sizeA - 8));  
        memcpy(F->mem-8,&sizeA,sizeof(long));  
        A->mem = F->mem;
        F->mem += sizeA;
        setSize(F,&(sizeF));
        _after[i] = A->mem;
        mergeMem();
        A = A->next;
        i++;
    }
    compacted_size = i;

 
    pthread_mutex_unlock(&lock);
    return compacted_size;
}

int available_memory() {
    return fSize();
}

void print_statistics() {
    long allocated_size = 0;
    int allocated_chunks = 0;
    long free_size = 0;
    int free_chunks = 0;
    int smallest_free_chunk_size = myalloc.size;
    int largest_free_chunk_size = 0;
 
    allocated_size = aSize();
    allocated_chunks = aCount();
    free_size = myalloc.size - allocated_size;
    free_chunks = fCount();
    smallest_free_chunk_size = minfSize();
    largest_free_chunk_size = maxfSize();

    printf("Allocated size = %ld\n", allocated_size);
    printf("Allocated chunks = %d\n", allocated_chunks);
    printf("Free size = %ld\n", free_size);
    printf("Free chunks = %d\n", free_chunks);
    printf("Largest free chunk size = %d\n", largest_free_chunk_size);
    printf("Smallest free chunk size = %d\n", smallest_free_chunk_size);
}


void printChunk(){
	void *temp;
	for(int i=0; i < 100; i++){
    	temp = myalloc.memory + i;
    	printf("myalloc.memory[%d]-addr: %p\n",i+1,temp);
    }
}

void printUsedChunk(){
	struct LList* t = myalloc.Free;
	printf("Free memory holes:\n");
	while(t != NULL){
		if( t->mem == (myalloc.endPtr + 1)){
			printf("addr: %p size: 0\n",t->mem);
		}
		else{
			printf("addr: %p size: %ld\n",t->mem,*(long *)(t->mem - 8));
		}
		t = t->next;
	}
}

int main(int argc, char* argv[]) {
    initialize_allocator(100);
    printf("Using first fit algorithm on memory size 100\n");
    void *t = ((myalloc.Free)->mem)-8;
    printf("Size of free mem in initial Chunk allocated: %ld\n",*(long *)t);
    printf("\n\n");
    printChunk();
    printf("\n\n");
    int* p[50] = {NULL};
    for(int i=0; i<10; ++i) {
        p[i] = allocate(sizeof(int));
        if(p[i] == NULL) {
            printf("Allocation failed\n");
            continue;
        }
        *(p[i]) = i;
        printf("p[%d] = %p ; *p[%d] = %d\n", i, p[i], i, *(p[i]));
    }
    printf("\n");
    printUsedChunk();
    printf("\n");
    printf("\nFree List\n");
    printLL(&(myalloc.Free));
    printf("\nAllocated List\n");
    printLL(&(myalloc.Alloc));
    
    printf("\n\n");

    print_statistics();
	
	printf("\n\n");
    

    for (int i = 0; i < 4; ++i) {
        printf("Freeing p[%d]\n", i);
        deallocate(p[i]);
        p[i] = NULL;
    }


    printf("\n");
    printUsedChunk();
    printf("\n");
    printf("\nFree List\n");
    printLL(&(myalloc.Free));
    printf("\nAllocated List\n");
    printLL(&(myalloc.Alloc));
    printf("\n\n");
    printf("available_memory %d", available_memory());
    printf("\n\n");
    print_statistics();
    return 0;
}

