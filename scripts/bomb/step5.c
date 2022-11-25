void phase_5()
{
    read_input(stdin, &str);
    if (strlen(str) != 6)
    {
        explode_bomb();
    }
    else
    {
        int i = 0;
        char *indexMe = "isrveawhobpnutfg";
        do
        {
            str[i] = indexMe[str[i] & 0xf];
            i++;
        } while (edx <= 5);
        if strcmp (str, "giants" != 0)
        {
            explode_bomb();
        }
    }
    return;
}