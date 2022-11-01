#include "du_malloc_mem.h"
void fun1(listGobal *List)
{
    char *k = (char*)du_malloc(5, "char", List);
    // free(k);
    du_free(k, List);
    // free(k);
}
void fun2(listGobal *List)
{
    return fun1(List);
}
void fun3(listGobal *List)
{
    return fun2(List);
}
void fun9(int *p,listGobal *List)
{
    du_free(p,List);
}
int t(listGobal *List)
{
    void *k=(int*)du_malloc(100,"int",List);
    return k;
}
int main()
{
    listGobal *begin = ll_create_list();
    // ll_print_all(begin);
    // int *p = du_malloc(10, "int", begin);
    // int *p1 = du_malloc(10, "int", begin);
    // int *p2 = du_malloc(10, "int", begin);
    // int *p12 = du_malloc(10, "int", begin);
    // int *p32 = du_malloc(10, "int", begin);
    // // // // Free_ALL_List(&begin->head);
    fun2(begin);
    // fun2(begin);
    // fun2(begin);
    // fun2(begin);
    // fun3(begin);
    // fun3(begin);
    // fun3(begin);
    // fun3(begin);
    // fun3(begin);
    // ll_print_all(begin);
    // du_free(p, begin);
    // int *kd = t(begin);
    // du_free(kd,begin);
    // fun9(p, begin);
    // ll_print_all(begin);
    // fun9(p, begin);
    // ll_print_all(begin);
    // free_listGobal(begin);
    // int *k = du_malloc(10, "int", begin);
    ll_print_all(begin);
    // removeTraceFuntionIfNotExisInListGobal(begin);
    ll_dump(begin);
    // ll_delete_all(begin);

    // list *k = ;
}