void phase_1(undefined4 param_1)
{
    int iVar1;

    iVar1 = strings_not_equal(param_1, "Public speaking is very easy.");
    if (iVar1 != 0)
    {
        explode_bomb();
    }
    return;
}