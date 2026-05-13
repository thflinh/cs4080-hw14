//> Chunks of Bytecode chunk-c
#include <stdlib.h>

#include "chunk.h"
//> chunk-c-include-memory
#include "memory.h"
//< chunk-c-include-memory
//> Garbage Collection chunk-include-vm
#include "vm.h"
//< Garbage Collection chunk-include-vm

void initChunk(Chunk* chunk) {
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
//> chunk-null-lines
  chunk->lines = NULL;
//< chunk-null-lines
//> chunk-init-constant-array
  initValueArray(&chunk->constants);
//< chunk-init-constant-array
  chunk->invokeCaches.count = 0;
  chunk->invokeCaches.capacity = 0;
  chunk->invokeCaches.entries = NULL;
}
//> free-chunk
void freeChunk(Chunk* chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
//> chunk-free-lines
  FREE_ARRAY(int, chunk->lines, chunk->capacity);
//< chunk-free-lines
//> chunk-free-constants
  freeValueArray(&chunk->constants);
//< chunk-free-constants
  FREE_ARRAY(InvokeCacheEntry, chunk->invokeCaches.entries,
             chunk->invokeCaches.capacity);
  initChunk(chunk);
}
//< free-chunk
/* Chunks of Bytecode write-chunk < Chunks of Bytecode write-chunk-with-line
void writeChunk(Chunk* chunk, uint8_t byte) {
*/
//> write-chunk
//> write-chunk-with-line
void writeChunk(Chunk* chunk, uint8_t byte, int line) {
//< write-chunk-with-line
  if (chunk->capacity < chunk->count + 1) {
    int oldCapacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(oldCapacity);
    chunk->code = GROW_ARRAY(uint8_t, chunk->code,
        oldCapacity, chunk->capacity);
//> write-chunk-line
    chunk->lines = GROW_ARRAY(int, chunk->lines,
        oldCapacity, chunk->capacity);
//< write-chunk-line
  }

  chunk->code[chunk->count] = byte;
//> chunk-write-line
  chunk->lines[chunk->count] = line;
//< chunk-write-line
  chunk->count++;
}
//< write-chunk
//> add-constant
int addConstant(Chunk* chunk, Value value) {
//> Garbage Collection add-constant-push
  push(value);
//< Garbage Collection add-constant-push
  writeValueArray(&chunk->constants, value);
//> Garbage Collection add-constant-pop
  pop();
//< Garbage Collection add-constant-pop
  return chunk->constants.count - 1;
}
//< add-constant
int addInvokeCache(Chunk* chunk) {
  if (chunk->invokeCaches.capacity < chunk->invokeCaches.count + 1) {
    int oldCapacity = chunk->invokeCaches.capacity;
    chunk->invokeCaches.capacity = GROW_CAPACITY(oldCapacity);
    chunk->invokeCaches.entries = GROW_ARRAY(InvokeCacheEntry,
        chunk->invokeCaches.entries, oldCapacity,
        chunk->invokeCaches.capacity);
  }

  int index = chunk->invokeCaches.count++;
  InvokeCacheEntry* entry = &chunk->invokeCaches.entries[index];
  entry->klass = NULL;
  entry->method = NULL;
  entry->owner = NULL;
  entry->name = NULL;
  entry->klassVersion = -1;
  entry->methodEpoch = -1;
  return index;
}
