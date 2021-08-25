#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<math.h>
#define INT_MAX 2147483647;

// adjency Node
typedef struct Node
{
    int index;
    int weight;
    struct Node *next;
}Node;


// Min heap
typedef struct HeapNode{
    int  vertex;
    int weight;
}HeapNode;

typedef struct Heap{
    int size;
    HeapNode **arr;
    
}Heap;

Heap *createMinHeap(int size){
    Heap * minHeap = (Heap *)malloc(sizeof(Heap));
    minHeap -> size = size;
    minHeap ->arr =(HeapNode **)malloc(size * sizeof(HeapNode *));
    return minHeap;
}

HeapNode *createHeapNode(int vertex , int weight){
    HeapNode *newHeapNode = (HeapNode *)malloc(sizeof(HeapNode));
    newHeapNode ->vertex = vertex;
    newHeapNode ->weight = weight;
    return newHeapNode;
}


Node* createNode(int des , int weight){
    Node *newNode =(Node *)malloc(sizeof(Node));
    newNode ->weight = weight;
    newNode ->index = des-1;
    newNode ->next = NULL;
    return newNode;
}

int min(int x ,int  y){
    if( x <y) return x;
    else return y;
}

void swapNode(HeapNode **x , HeapNode **y){
    HeapNode * temp = *x;
    *x = *y;
    *y = temp;
}
int isEmpty(Heap * heap){
    if(heap ->size ==0) return 1;
    return 0;
}

int max( int a , int b){
    if( a>b) return a;
    else return b;
}

int ciel_heap(int a){
    if(a%2 ==0) return (a/2);
    else return (a/2) +1;
}
// heapify
void heapify (Heap *minheap ,int index){
    int smallest = index;
    int left = 2*index +1;
    int right = 2*index+2;

    if( left < minheap ->size && minheap ->arr[left] ->weight < minheap ->arr[smallest] ->weight){
        smallest= left;
    }
    if(right < minheap ->size && minheap ->arr[right] ->weight < minheap ->arr[smallest] ->weight){
        smallest = right;
    }
    if(smallest != index){
        swapNode(&minheap ->arr[smallest] ,& minheap ->arr[index]);
        heapify(minheap ,smallest);
    }
}

void buildHeap(Heap *heap){
    int size = heap ->size -1;
    for(int i=  size/2 ;i>=0 ;i--){
        heapify(heap ,i);
    }
}

// extract Min
int extractMinIndex(Heap *heap){
    if(isEmpty(heap)){ // no element in the heap
        return -1;
    }
    int minIndex = heap ->arr[0] ->vertex;
    swapNode(&heap ->arr[0] ,&heap ->arr[(heap ->size) -1]);
    free(heap ->arr[(heap ->size)-1]);
    heap ->size = (heap ->size) -1;
    heapify(heap ,0);
    return minIndex;
}

void perculateUp(Heap *heap ,int index){
    int current = index;
    int parent = ciel_heap(current) -1;
        while( parent >=0  && heap->arr[parent] ->weight > heap ->arr[current] ->weight){
            swapNode(&heap ->arr[current] ,&heap ->arr[parent]);
            current = parent;
            parent = ciel_heap(current) -1;
        } 
}

void updateKey(Heap *heap , int index , int newWeight ){
    for( int i=0 ; i<heap ->size ;i++){
        if( heap ->arr[i] ->vertex ==index){
            heap -> arr[i] ->weight = newWeight;
            perculateUp(heap ,i);
            return;
        }
    }
}


void printHeap(Heap *heap){
    for( int i=0; i<heap ->size; i++){
    printf("Heap index %d --> Heap weight %d \n" , heap ->arr[i] ->vertex ,heap->arr[i] ->weight);

    }
}

//  end of Min heap
// Functions

void kShortestDistanceTarget(Node **ptr ,int *dist , int *visited , int source , int k , Heap *heap , int size){
    updateKey(heap ,source ,0);
    visited[source] = 1;
    dist[source] =0;
    int minIndex ;
    int j =0;
    for(int i=0 ; i<size ;i++){
        minIndex = extractMinIndex(heap);
        if(minIndex !=  source){
            if(j == k) return;
            printf("%d \n", minIndex +1);
        }
        Node *temp = ptr[minIndex];
        while(temp != NULL){
            if(!visited[temp ->index]){
                if(dist[temp ->index] > dist[minIndex] + temp ->weight){
                    dist[temp ->index] = dist[minIndex] + temp ->weight;
                    updateKey(heap ,temp ->index , dist[temp ->index]);
                }
            }
            temp =temp ->next;
        }
        j++;
    }
}

int main(){
    FILE *file = fopen("input.txt","r");
    int N;
    int M;
    int S;
    int K;
    fscanf(file,"%d, %d, %d, %d" ,&N ,&M, &S,&K);
    int visited[N];
    int dist[N];
    Node *nodeptr[N];
    Heap *heap = createMinHeap(N);
    for( int i=0; i<N ;i++){
        nodeptr[i] = NULL;
        visited[i] =0;
        dist[i] = INT_MAX;
        heap ->arr[i] = createHeapNode(i ,2147483647);
    }
    for(int i=0; i<M ;i++){
        int source;
        int destination;
        int weight;
        fscanf(file ,"%d, %d, %d" ,&source ,&destination ,&weight);
        if(nodeptr[source-1] == NULL){
            nodeptr [source -1] = createNode(destination ,weight);
        }
        else{
            Node *temp = nodeptr[source-1];
            while( temp ->next != NULL){
                temp = temp ->next;
            }
            temp ->next = createNode(destination ,weight);
        }
        if(nodeptr[destination-1] == NULL){
            nodeptr [destination -1] = createNode(source ,weight);
        }
        else{
            Node *temp = nodeptr[destination-1];
            while( temp ->next != NULL){
                temp = temp ->next;
            }
            temp ->next = createNode(source ,weight);
        }  
    }
    fclose(file);
    kShortestDistanceTarget(nodeptr ,dist , visited ,S-1,K ,heap ,N);
    return 0;
}