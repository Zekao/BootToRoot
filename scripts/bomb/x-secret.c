undefined1 *read_line(void)

{
    char cVar1;
    int iVar2;
    char *pcVar3;
    uint uVar4;

    iVar2 = skip();
    if (iVar2 == 0)
    {
        if (infile != stdin)
        {
            pcVar3 = getenv("GRADE_BOMB");
            if (pcVar3 != (char *)0x0)
            {
                /* WARNING: Subroutine does not return */
                exit(0);
            }
            infile = stdin;
            iVar2 = skip();
            if (iVar2 != 0)
                goto LAB_0804925f;
        }
        printf("Error: Premature EOF on stdin\n");
        explode_bomb();
    }
LAB_0804925f:
    uVar4 = 0xffffffff;
    pcVar3 = input_strings + num_input_strings * 0x50;
    do
    {
        if (uVar4 == 0)
            break;
        uVar4 = uVar4 - 1;
        cVar1 = *pcVar3;
        pcVar3 = pcVar3 + 1;
    } while (cVar1 != '\0');
    if (~uVar4 == 0x50)
    {
        printf("Error: Input line too long\n");
        explode_bomb();
    }
    iVar2 = num_input_strings * 0x50;
    *(undefined *)(~uVar4 + 0x804b67e + iVar2) = 0;
    num_input_strings = num_input_strings + 1;
    return input_strings + iVar2;
}

void phase_defused(void)
{
    int iVar1;
    undefined local_58[4];
    undefined local_54[80];

    if (num_input_strings == 6)
    {
        iVar1 = sscanf(input_strings + 0xf0, "%d %s", local_58, local_54);
        if (iVar1 == 2)
        {
            iVar1 = strings_not_equal(local_54, "austinpowers");
            if (iVar1 == 0)
            {
                printf("Curses, you\'ve found the secret phase!\n");
                printf("But finding it and solving it are quite different...\n");
                secret_phase();
            }
        }
        printf("Congratulations! You\'ve defused the bomb!\n");
    }
    return;
}

int fun7(int *param_1, int param_2)
{
    int iVar1;

    if (param_1 == (int *)0x0)
    {
        iVar1 = -1;
    }
    else if (param_2 < *param_1)
    {
        iVar1 = fun7(param_1[1], param_2);
        iVar1 = iVar1 * 2;
    }
    else if (param_2 == *param_1)
    {
        iVar1 = 0;
    }
    else
    {
        iVar1 = fun7(param_1[2], param_2);
        iVar1 = iVar1 * 2 + 1;
    }
    return iVar1;
}

void secret_phase(void)
{
    undefined4 uVar1;
    int iVar2;

    uVar1 = read_line();
    iVar2 = __strtol_internal(uVar1, 0, 10, 0);
    if (1000 < iVar2 - 1U)
    {
        explode_bomb();
    }
    iVar2 = fun7(n1, iVar2);
    if (iVar2 != 7)
    {
        explode_bomb();
    }
    printf("Wow! You\'ve defused the secret stage!\n");
    phase_defused();
    return;
}