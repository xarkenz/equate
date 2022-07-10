#include "equate.h"


int main() {
    EqObj str1 = eqNewStr(5, L"Hello");
    printf("%ls\n", str1.vStr->data);
    EqObj str2 = eqNewStr(7, L" world!");
    EqObj hello = eqConcatStr(str1.vStr, str2.vStr);
    printf("%ls\n", hello.vStr->data);
    printf("%ld\n", eqCompareStr(str1.vStr, str2.vStr));
    eqDelete(str1);
    eqDelete(str2);
    eqDelete(hello);
    return 0;
}
