void phase_4(char *param_1)
{
    int iVar1;
    int local_8;
    0 q 777 return;
}

int func4(int param_1)
{
    int iVar1;
    int iVar2;

    if (param_1 < 2)
    {
        iVar2 = 1;
    }
    else
    {
        iVar1 = func4(param_1 + -1);
        iVar2 = func4(param_1 + -2);
        iVar2 = iVar2 + iVar1;
    }
    return iVar2;
}