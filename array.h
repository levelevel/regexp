//動的配列（アレイ）
//
//## 機能
//- データ：任意のポインタ
//- 追加・削除・複製・push・pop・サイズ指定

//アレイのデータ型（任意のポインタ型）
typedef void* sarray_data_t;

//ポインタ版動的配列
typedef struct array {
    size_t num;             //データ数
    size_t capacity;        //最大データサイズ
    sarray_data_t *buckets; //データの配列
} array_t;

//アレイをアロケートする。
//init_capacityには初期最大データサイズを指定する。0の場合はデフォルト値が使用される。
//データ数(array->num)は常に0となる。
array_t *new_array(size_t init_capacity);

//アレイを開放する。
void free_array(array_t *array);

//アレイのサイズ(array->num)を取得する。
size_t num_array(const array_t *array);

//アレイのサイズを設定する。アレイのサイズは自動的に拡張される。
void set_array_size(array_t *array, size_t size);

//アレイのidx番目にデータを格納する。アレイのサイズは自動的に拡張される。
//データ未設定の領域にはNULLが設定される。
void put_array(array_t *array, int idx, sarray_data_t data);

//アレイのidx番目のデータを取得する。アレイのサイズは変わらない。
sarray_data_t get_array(const array_t *array, int idx);

#define poke_array(array, idx, data) ((array)->buckets[idx] = (sarray_data_t)data)
#define peek_array(array, idx)       ((array)->buckets[idx])

//アレイのidx番目のデータを削除する。以降のデータは前に詰められる。アレイのサイズは1減る。
void del_array(array_t *array, int idx);

//アレイの最後にデータを1個追加する。アレイのサイズは1増える。
void push_array(array_t *array, sarray_data_t data);

//アレイの最後のデータを1個取り出す。アレイのサイズは1減る。
sarray_data_t pop_array(array_t *array);

//アレイを複製する。
//複製されたアレイの内部データは必要最低限分が確保される。
array_t *dup_array(array_t *array);
