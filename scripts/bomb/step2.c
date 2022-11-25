void phase_2(undefined4 param_1)
{
    int i;
    int argsToComp[7];

    read_six_numbers(param_1, argsToComp + 1);
    if (argsToComp[1] != 1)
    {
        explode_bomb();
    }
    i = 1;
    do
    {
        if (argsToComp[i + 1] != (i + 1) * argsToComp[i])
        {
            explode_bomb();
        }
        i = i + 1;
    } while (i < 6);
    return;
}