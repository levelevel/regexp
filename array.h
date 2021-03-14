//動的配列（アレイ）
//
//## 機能
//- データ：任意のポインタ
//- 追加・削除・複製・push・pop・サイズ指定・サイズ固定

//ポインタ版動的配列
typedef struct array {
    int num;
    int size_fixed; //配列サイズ固定済み
    int capacity;
    void **buckets;
} array_t;

//アレイをアロケートする。
array_t *new_array(int size);

//アレイを開放する。
void free_array(array_t *array);

//アレイのサイズ(array->num)を取得する。
int num_array(const array_t *array);

//アレイのサイズを設定する。アレイのサイズは自動的に拡張される。
void set_array_size(array_t *array, int size);

//アレイのidx番目にデータを格納する。アレイのサイズは自動的に拡張される。
//データ未設定の領域にはNULLが設定される。
void put_array(array_t *array, int idx, void *data);

//アレイのidx番目のデータを取得する。アレイのサイズは変わらない。
void *get_array(const array_t *array, int idx);

//アレイのidx番目のデータを削除する。以降のデータは前に詰められる。アレイのサイズは1減る。
void del_array(array_t *array, int idx);

//アレイの最後にデータを1個追加する。アレイのサイズは1増える。
void push_array(array_t *array, void *data);

//アレイの最後のデータを1個取り出す。アレイのサイズは1減る。
void *pop_array(array_t *array);

//アレイを複製する。
//複製されたアレイの内部データは必要最低限分が確保される。
array_t *dup_array(array_t *array);

//配列サイズを固定する。
//以降、配列サイズを変更するアクセス（push_array/pop_array等）はエラーとなる。
void fix_array_size(array_t *array);

//配列サイズ固定を解除する。
#define unfix_array_size(array) (array)->size_fixed=0
