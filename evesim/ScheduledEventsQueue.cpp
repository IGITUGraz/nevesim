#include "ScheduledEventsQueue.h"

#include <iostream>

using std::cout;
using std::endl;


ScheduledEventsQueue::ScheduledEventsQueue(int chunkSize):
        chunkBuffer(0),
        currentFreeChunk(NULL),
        nextFreeNodeIdx(0),
        nextFreeChunkIdx(0),
        recycledNodes(NULL),
        chunkSize(chunkSize)
{
    reset();
}


ScheduledEventsQueue::~ScheduledEventsQueue()
{
    for(size_t i=0; i<chunkBuffer.size(); i++) {
        delete[] chunkBuffer[i];
    }
}

void ScheduledEventsQueue::init()
{
    if( chunkBuffer.size() < 1 ) {
        chunkBuffer.reserve( 10 );
        chunkBuffer.resize( 0 );
        chunkBuffer.push_back( new Node[ chunkSize ] );
    }
}

void ScheduledEventsQueue::reset()
{
    init();

    currentFreeChunk = chunkBuffer[0];
    nextFreeChunkIdx = 1;

    nextFreeNodeIdx  = 0;

    recycledNodes = NULL;

    event_queue = priority_queue<Node*, vector<Node*>, NodeCompare >();

}

ScheduledEventsQueue::Node *ScheduledEventsQueue::getFreeNode(void)
{
    Node *n;
    if (recycledNodes != NULL) {
        // get a recycled node
        n = recycledNodes;
        recycledNodes = recycledNodes->next;
    } else if ( nextFreeNodeIdx < chunkSize ) {
        // get slot from the current (allocIdx) pre-allocated memory chunk
        n = &(currentFreeChunk[nextFreeNodeIdx++]);
    } else if (nextFreeChunkIdx < chunkBuffer.size() ) {
        // current (currentFreeChunk) pre-allocated memory chunk used up: go to next chunk
        currentFreeChunk = chunkBuffer[nextFreeChunkIdx++];
        n = &(currentFreeChunk[0]);
        nextFreeNodeIdx = 1;
    } else {
        // no more chunks available: alloc a new one
        currentFreeChunk = new Node[chunkSize];
        chunkBuffer.push_back( currentFreeChunk );
        nextFreeChunkIdx++;
        n = &(currentFreeChunk[0]);
        nextFreeNodeIdx = 1;
    }
    return n;
}

