#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "array.h"

#define ARRAY_INIT_SIZE     16
#define ARRAY_GROW_FACTOR   2

//アレイをアロケートする。
array_t *new_array(int size) {
    assert(size>=0);
    array_t *array = calloc(1, sizeof(array_t));
    assert(array);
    array->capacity = size?size:ARRAY_INIT_SIZE;
    array->buckets = calloc(array->capacity, sizeof(void*));
    assert(array->buckets);
    return array;
}

//アレイを開放する。
void free_array(array_t *array) {
    if (array) free(array->buckets);
    free(array);
}

//アレイのサイズ(array->num)を取得する。
int num_array(const array_t *array) {
    assert(array);
    return array->num;
}

//アレイのサイズを設定する。アレイのサイズは自動的に拡張される。
void set_array_size(array_t *array, int size) {
    assert(array);
    assert(size>=0);
    array->num = size;
    array->buckets = realloc(array->buckets, size*sizeof(void*));
    assert(array->buckets);
    if (size>array->capacity) {
        memset(array->buckets+array->capacity, 0x00, size-array->capacity);
    }
    array->capacity = size;
}

//アレイのidx番目にデータを格納する。アレイのサイズは自動的に拡張される。
//データ未設定の領域にはNULLが設定される。
void put_array(array_t *array, int idx, void *data) {
    assert(array);
    assert(idx>=0);
    if (idx>=array->capacity) {     //buckets拡張時は拡張分を0クリアする
        assert(!array->size_fixed);
        int new_cap = array->capacity;
        while (idx>=new_cap) new_cap *= ARRAY_GROW_FACTOR;
        array->buckets = realloc(array->buckets, new_cap*sizeof(void*));
        assert(array->buckets);
        memset(array->buckets+array->capacity, 0x00, (new_cap-array->capacity)*sizeof(void*));
        array->capacity = new_cap;
    }
    array->buckets[idx] = data;
    if (idx>=array->num) array->num = idx+1;
}

//アレイのidx番目のデータを取得する。アレイのサイズは変わらない。
void *get_array(const array_t *array, int idx) {
    assert(array);
    assert(idx>=0);
    assert(idx<array->num);
    return (array->buckets[idx]);
}

//アレイのidx番目のデータを削除する。以降のデータは前に詰められる。アレイのサイズは1減る。
void del_array(array_t *array, int idx) {
    assert(array);
    assert(idx>=0);
    assert(idx<array->num);
    memcpy(array->buckets+idx, array->buckets+idx+1, (array->num-idx)*sizeof(void*));
    array->buckets[array->num-1] = NULL;
    array->num--;
}

//アレイの最後にデータを1個追加する。アレイのサイズは1増える。
void push_array(array_t *array, void *data) {
    assert(array);
    assert(!array->size_fixed);
    put_array(array, array->num, data);
}

//アレイの最後のデータを1個取り出す。アレイのサイズは1減る。
void *pop_array(array_t *array) {
    assert(array);
    assert(array->num>0);
    assert(!array->size_fixed);
    void *data = array->buckets[--array->num];
    array->buckets[array->num] = NULL;
    return data;
}

//アレイを複製する。
//複製されたアレイの内部データは必要最低限分が確保される。
array_t *dup_array(array_t *array) {
    assert(array);
    array_t *new = new_array(array->num);
    new->num = array->num;
    memcpy(new->buckets, array->buckets, array->num*sizeof(void*));
    return new;
}

//配列サイズを固定する。
//以降、配列サイズを変更するアクセス（push_array/pop_array等）はエラーとなる。
void fix_array_size(array_t *array) {
    array->size_fixed = 1;
    array->buckets = realloc(array->buckets, array->num*sizeof(void*));
    array->capacity = array->num;
}
