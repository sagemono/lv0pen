long syscon_config_status(unsigned char status)
{
    switch (status) {
    case 0: return 0;
    case 1: return -10001;
    case 2: return -10002;
    case 254: return -10003;
    }
    return -99999;
}
