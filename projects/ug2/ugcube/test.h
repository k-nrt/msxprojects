#ifndef TEST_H
#define TEST_H

//. test_sincos.c
extern void Test_SinCosLp(const char *pszTitle);
extern void Test_SinLp_CosLp(const char *pszTitle);
extern void Test_SinHp(const char *pszTitle);

//. test_line.c
extern void Test_Line(const char* pszTitle);

//. test_pers.c
extern void Test_Pers(const char* pszTitle);
extern void Test_PersAnim(const char* pszTitle);
extern void Test_PersScroll(const char* pszTitle);
extern void Test_BBoxClip(const char* pszTitle);

//. test.c
extern void Test_DrawTitle(const char *pszTitle);
extern void Test_DrawTimerAndWait(void);
extern void Test_WaitForTrigger(const char *pszTitle);
extern void Test(const char *pszTitle);

#endif //TEST_H

