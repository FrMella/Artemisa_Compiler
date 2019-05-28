//
// Created by francisco on 28/05/19.
//

#define newMem(type, num) ((type*)(allocMem(sizeof(type) * (num))))
#define neMemZ(type, num) ((type*)(allocMem(sizeof(type) * (num))))
#define renewMem(type, ptr, num) ((type*)(allocMem((ptr), sizeof(type) * (num) )))

void releaseMem (void *ptr);
void *allocMem(int nBytes);
void *allocMemZ(int nBytes);
void *reallocMem(void *ptr, int nBytes);

int totalBytesMem;