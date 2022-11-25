void phase_3(char *param_1)
{
    int iVar1;
    char cVar2;
    uint firstParam;
    char secParam;
    int thirdParam;

    iVar1 = sscanf(param_1, "%d %c %d", &firstParam, &secParam, &thirdParam);
    if (iVar1 < 3)
    {
        explode_bomb();
    }
    switch (firstParam)
    {
    case 0:
        cVar2 = 'q';
        if (thirdParam != 777)
        {
            explode_bomb();
        }
        break;
    case 1:
        cVar2 = 'b';
        if (thirdParam != 214)
        {
            explode_bomb();
        }
        break;
    case 2:
        cVar2 = 'b';
        if (thirdParam != 755)
        {
            explode_bomb();
        }
        break;
    case 3:
        cVar2 = 'k';
        if (thirdParam != 251)
        {
            explode_bomb();
        }
        break;
    case 4:
        cVar2 = 'o';
        if (thirdParam != 160)
        {
            explode_bomb();
        }
        break;
    case 5:
        cVar2 = 't';
        if (thirdParam != 458)
        {
            explode_bomb();
        }
        break;
    case 6:
        cVar2 = 'v';
        if (thirdParam != 780)
        {
            explode_bomb();
        }
        break;
    case 7:
        cVar2 = 'b';
        if (thirdParam != 524)
        {
            explode_bomb();
        }
        break;
    default:
        cVar2 = 'x';
        explode_bomb();
    }
    if (cVar2 != secParam)
    {
        explode_bomb();
    }
    return;
}