#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <openssl/aes.h>

#ifdef _MSC_VER
#include <intrin.h> /* for rdtscp and clflush */
#pragma optimize("gt",on)
#else
#include <x86intrin.h> /* for rdtscp and clflush */
#endif

struct bounded_array
{
    volatile unsigned int length;
    volatile unsigned int mask;
    char * data;
};

unsigned int random_data[] = {1357,
    721,1204,1203,236,959,444,1131,433,968,224,862,1231,271,531,78,954,1216,1633,417,272,1612,1583,1580,706,750,504,332,905,1328,1449,1527,285,1395,212,1476,1383,1476,1099,329,476,
    18,1268,1491,1535,1638,1476,1541,1091,1175,1140,1215,977,146,110,1107,1398,565,1275,672,967,874,198,1306,620,1677,1671,1047,267,88,360,1620,1125,548,1245,559,885,961,1369,165,1319,
    1563,120,703,316,1101,73,58,1363,713,972,698,986,1395,827,1429,63,1680,287,260,859,149,765,527,428,1164,1047,779,1591,1352,1486,390,1394,1297,1167,1673,177,1196,843,227,580,
    1064,839,1280,453,1508,1684,462,1100,235,361,1395,1522,990,1501,1081,1293,488,88,112,514,659,482,55,799,1192,904,148,1434,480,1587,554,1108,655,6,1300,1615,1311,1017,366,677,
    242,1468,194,296,168,1023,148,465,891,202,1188,574,1474,284,1345,1298,1448,1316,1693,1115,1024,475,752,117,1327,65,1364,663,1290,1684,1164,1599,214,805,1674,325,719,721,376,272,
    327,71,1025,1370,858,472,1297,1027,1070,771,1240,349,850,461,6,1010,1582,605,140,929,66,1385,1091,579,70,1143,1579,973,464,270,1653,1487,337,1418,988,334,1421,728,98,1364,
    43,1270,373,113,918,377,89,996,229,137,1160,1381,334,1576,1367,1392,1618,1449,607,977,1605,834,794,272,448,630,458,208,426,279,850,241,34,496,22,956,555,1099,141,153,
    458,1571,350,327,35,215,1152,1296,1182,1609,1280,896,685,1653,322,229,233,657,533,1554,249,305,1628,1626,1061,85,16,730,850,425,963,543,1598,992,1033,1603,212,1489,526,854,
    729,20,198,588,536,1082,1693,1332,1380,1523,1641,1579,249,1642,1099,1249,85,392,1243,1386,1574,1328,978,1316,1507,1387,1152,294,1520,861,457,620,930,194,322,1089,886,1210,806,574,
    413,926,986,104,1510,352,1535,411,862,1294,1079,188,982,418,12,866,237,1166,1165,1519,401,714,581,825,826,15,1648,1525,766,1635,261,89,121,654,908,1643,1487,686,1594,1116,
    223,1373,1618,186,1033,1678,1536,78,743,835,1162,54,1120,1308,779,1532,1532,216,12,411,1656,222,102,1489,1449,972,71,823,1521,411,88,400,920,1444,961,12,190,216,627,722,
    662,1468,1658,1527,269,232,566,688,1233,614,1683,496,351,11,203,878,485,838,667,1318,863,336,1418,1680,891,1358,723,1230,146,1483,989,1547,1527,928,404,1041,723,1172,572,1144,
    1033,255,189,993,1259,1007,1550,683,1613,1394,152,1125,164,1500,1404,1496,591,1153,1696,108,119,310,702,1410,568,583,1370,1674,1477,572,143,276,385,623,1224,148,1409,1526,454,55,
    72,1674,688,1147,1676,1055,275,86,352,585,416,351,1682,513,224,114,1638,1276,1057,602,1623,616,422,470,1681,857,1298,503,8,1421,365,394,1062,991,1431,529,215,1317,1154,932,
    499,1294,1097,54,1158,1112,1220,1183,224,1290,572,1511,613,94,162,509,1023,1134,866,695,1097,1263,1317,1343,1470,351,14,1691,1438,507,8,157,1694,563,1272,1306,1051,1025,445,434,
    1187,567,130,1419,763,1396,416,1022,42,1460,89,141,1660,1,178,1,1344,328,678,417,286,64,188,590,1249,1303,1296,826,543,969,1657,296,387,117,630,388,944,491,248,1044,
    516,121,191,474,28,474,825,1009,76,1381,1002,946,333,445,1438,376,1551,37,1155,1551,1623,42,483,1427,737,884,1249,959,1007,1529,1340,128,1324,1148,1038,1151,1640,1516,1120,957,
    241,628,580,813,104,1597,1591,1160,867,1310,737,143,1087,1407,913,1370,642,1209,714,1560,802,827,205,68,304,1045,908,320,899,1297,27,659,1244,1253,190,691,1605,423,1632,1006,
    1356,1589,1012,559,1566,758,1347,1112,1563,537,1387,49,1428,588,1202,280,141,1525,997,60,1182,861,215,251,622,1389,820,990,1110,1405,1357,109,1162,1301,172,137,1629,894,371,1683,
    1573,31,32,1427,1082,491,156,1294,467,1505,207,1571,751,208,206,61,456,713,1682,1350,471,436,90,1423,317,1654,884,47,1509,565,988,157,1230,927,100,1679,1629,322,677,199,
    790,1520,1469,1346,1506,1361,711,1295,1032,129,1477,498,682,503,1599,288,916,698,594,152,1512,934,574,1190,550,1492,485,1297,1278,877,1097,1002,1523,993,1552,1093,654,590,510,597,
    1321,466,362,692,660,580,266,1282,717,1370,1453,1240,738,1608,1395,762,1494,231,705,22,1384,599,483,1637,560,1114,905,1629,110,1208,1423,1431,302,1053,53,627,386,1066,369,256,
    1396,72,1592,165,389,683,37,761,1549,1091,1253,870,1039,693,788,993,1541,588,386,335,515,232,1119,631,1,714,1238,1444,766,1144,1304,407,41,845,911,853,602,1362,812,861,
    1334,1115,1672,483,708,874,871,1291,665,387,1026,1483,1092,719,117,272,883,344,826,1181,1047,775,620,330,393,1570,129,1230,146,486,1272,851,1590,493,600,654,445,192,256,1573,
    494,1370,873,1208,1111,1353,31,1511,652,1102,101,637,1447,1419,405,804,526,658,745,1100,1561,636,1687,652,604,167,342,1081,350,387,1555,151,602,704,1676,677,398,1523,702,1656,
    707,782,823,1084,1652,1192,876,563,284,1278,1650,1254,457,750,249,590,1408,1428,1508,1400,717,729,534,59,1621,1559,752,684,1449,645,474,391,703,1105,318,1477,1126,639,719,599,
    438,372,62,1428,648,105,333,1052,715,1565,479,1441,1011,479,351,1368,1428,1207,1189,645,392,1071,460,738,772,1286,26,1119,374,1262,1640,846,1221,702,1009,1390,1559,1228,76,701,
    1239,14,738,564,245,1342,1519,1610,1327,882,431,1127,1649,149,46,478,349,829,1142,132,634,1043,961,303,1071,888,528,877,1257,658,828,876,637,554,1534,322,1284,538,756,53,
    974,852,596,569,1050,1169,1493,397,1254,485,98,274,1118,1438,323,1432,1548,1679,1223,960,658,740,1306,42,442,206,407,1096,1244,860,1255,1591,1600,257,590,643,1150,1552,674,1305,
    579,546,54,158,1471,1161,747,999,574,411,1533,863,1262,779,431,1240,125,1368,244,366,1127,1165,1241,691,1373,852,1440,701,87,1637,503,871,1438,704,130,1028,98,1433,192,488,
    334,5,356,1303,655,713,1166,1073,936,471,1301,250,1538,434,746,647,757,214,224,863,750,906,306,1333,1618,642,630,134,1092,1430,1268,1258,103,1046,889,1589,251,1153,928,194,
    1680,1558,1151,1375,652,974,274,992,16,760,630,999,296,1200,1085,22,85,488,1325,1458,37,92,1055,573,338,1601,920,1543,595,645,15,444,870,1286,4,923,358,1393,686,1518,
    1611,1430,201,1541,1659,781,318,968,103,304,422,440,517,30,1273,1276,193,521,1040,576,399,1192,1698,1373,1020,557,348,806,1046,607,67,1657,1338,70,1013,193,324,489,957,816,
    1308,1141,192,1366,990,796,558,236,1200,1688,1005,1588,151,800,1428,420,612,1162,61,1316,1181,128,322,1166,1369,595,445,681,1585,733,1459,917,282,22,1381,243,472,1050,288,638,
    966,1332,412,124,951,1088,9,978,93,166,249,1501,813,379,548,1274,124,242,272,379,1168,383,11,539,492,95,880,339,91,287,1018,1346,1550,102,1136,452,1120,219,795,314,
    666,1593,1455,322,1379,394,138,336,650,303,953,195,1443,102,204,952,133,1118,289,17,1346,969,559,1608,1048,1611,474,1597,1638,894,237,678,97,365,985,1132,939,46,558,141,
    938,355,650,128,1194,767,284,827,1223,4,725,1327,233,14,1373,1539,1552,23,1351,100,855,267,1561,260,15,531,718,1638,1004,550,1561,1637,1030,663,1528,77,587,1236,852,815,
    14,605,29,1091,183,1080,911,942,327,26,1335,131,1423,27,1406,543,39,653,724,780,292,703,1171,1105,583,1686,398,1632,85,1412,1260,1133,349,1259,26,1050,463,916,1462,701,
    760,336,408,1222,1415,94,1277,303,1687,1583,1666,1302,90,1605,301,1249,1311,1544,238,1507,970,1238,832,1538,1536,64,870,509,1689,1615,1248,825,1425,1099,1156,166,1057,593,464,898,
    815,101,1516,331,1634,301,946,638,1585,808,951,1265,1510,1594,696,1107,669,31,350,1231,406,317,1629,1060,1515,313,509,1126,444,1029,182,1615,44,1536,1051,1154,1488,411,507,};


struct bounded_array random_dat = {
    1024,
    0x3ff,
    (char *)random_data
};

char * plain_text_test =   "oDFV2O1aP136YnmEbhZJLMizLukPQF3Ir6kzrYGMOm9M822cFsuLftYMulqTzNwmhvoTkUr7mFwm0r8w2t51ccg2qgRhdWrI5ldwsnRZXoXoogHLUYbNMQPn8Pc4SPVRckc1XtQVAoIFSaBrOX3WBl27GZQfqTUROjIrSwlErkwevuIXQfby8WtMRbw8f0RrvJCytHaJfWyD9rC0VMCMFl4gZstTw0WxxBvAEQEhtBdJkJKOEw1xUo9MyiLj77QD14XSzx2p9wFEpPTbP96X69Mz628IaGgmGbKO06uFesKISWF4qltlIe74Jm00kZpeXCx7uZQ02VGQ3vLPSanJUBv0FYVMbl2VoARBo1D0IAwYvk35fLR4qXUinVgoL8NxhaaNi6Al6zww23kBSlzXZimSkkG0V9mmjArlOyE5N6DR0C2n9R6jEtsUQejADev21cWPE742mQc8q50u8B5X5QWYiPsZVz4VlMnC0aNDRH7gQMz4gCfuEfd14sm4Kl7TdNGHw0VzrxaFARKR1T6kih3RgeBCQGYvIJiP9oWQQvXf0WkoL289SrwOYA5lj8ArAH3ftM15K4ih3UrXVfZHvE031bqwTueRZQPTGp7psY5jBNGs5G8bUROxYtUwS63lkJTj7IuvIUaTIgJvxQHrMUSnN86aG6uMUlNZCFF8lJamsDtLAU5WlXs5aWS2ckwmo0BECJxkZwg8FiPmY2A4EPrmcKnLIj0DHHmbelAV57KmPmRk9q3LeFZeNJvranJU3FDioc5rSAxT16M5rDlZlxdLANByfz6jaaVa3CcqTGFfS5F0ZHcZlDCEy4fzLQtwDACfQAoiUAOvmfI01q89U1fNqIBcbuXi8AZwcos19bJCpOZfaTkBEldeC2EmTLVLZZ6XhZWBJf6iKL2sJriGPfJY6NT67LOit0cvPs8N8o2v9XP7HSRw7RPm5h3GSeVGbcftzQ4VgEefEIlu4QWgoMqRsnASzEhAS0TPk4AUC63ieNwRjwBerK7PA60Oq9tyRfUfqWXlvCfqV8JOUDo9hzxxopC4Bk0HtjPZ21KyPqQD2AFGVSWcucK4ZL3eYed1R9yG2XWDUfpT5Z8pFNX59X9SAlyjob28IHayBhVmVlmJDFTVS7vcsVqACSetqJCexJ8kkBO1eCI1x67LjztTT2N7o4gmb8zunYutnHpIO9OFdVuqv1taRrcCFBhNrhpeBME7n94QQnTK2zJ7grqhEm1ZXLkO235sCIDXnSmkiCsvvNiYEfVyksi3bjZFlNLIgLorrdR3ykjFWAyJxkotmGCIxLQ1ykGJU8wDLoTnKD21z6IHm9YNl3HNLPEHIzOMdJuYwazUb1ih00RNsr9OYcSPxy7s0xrpt3sJZ44DWtGYwN84OY5eCHhyP2UdiV4Otlqtbj3seC2yCJ9hznVO67yNHsp07vQgIUXGZSbX5zzTRLrkHrDAVexrKElHrafqRgWwnzibtlvo8cd6jknGbIzXKypEAREYrCzLBusH3A7A8xMc6Gox4JEJxpZ22Ui5MuFA5fQt9xNwKSJmsZPENe55wjcLg58MWZey8cbiA3LpqK2lPRC7mvBpVvIb0dxD";
    
struct bounded_array plain_text_orig = {
    1600,
    0x7FF,
    NULL
};


struct bounded_array plain_in = {
    8192,
    0x1FFF,
    NULL
};

char plain_in_data[8192];
char plain_text_orig_data[1600];

#define ONE_PAGE 4096

static void alloc_plain_buffers( void )
{
    char * plain_text_a = plain_text_orig_data;
    memset(plain_text_a, 0, 1600);
    plain_text_orig.data = plain_text_a;
    
    char * plain_text_b = plain_in_data;
    memset(plain_text_b, 0, 2*ONE_PAGE);
    plain_in.data = plain_text_b;
}
    
    
static inline char get_byte( struct bounded_array * ba, unsigned int offset )
{
    if( offset < ba->length )
    {
#ifdef MASK_PROTECT
        offset &= ba->mask;
#else
#  ifdef LOAD_FENCE
        __sync_synchronize();
#  endif
#endif
        return ba->data[offset];
    }
    
    return 0;
}

static inline int get_int( struct bounded_array * ba, unsigned int offset )
{
    if( offset < ba->length )
    {
#ifdef MASK_PROTECT
        offset &= ba->mask;
#else
#  ifdef LOAD_FENCE
        __sync_synchronize();
#  endif
#endif
        return ((int *)(ba->data))[offset];
    }
    
    return 0;
}

static inline void set_byte( struct bounded_array * ba, unsigned int offset, char value )
{
    if( offset < ba->length )
    {
#ifdef MASK_PROTECT
        offset &= ba->mask;
#else
#  ifdef LOAD_FENCE
        __sync_synchronize();
#  endif
#endif
        ba->data[offset] = value;
    }
}

static void do_work( int work_loop, struct bounded_array *random_dat_ptr, struct bounded_array *plain_in_ptr, struct bounded_array *plain_text_orig_ptr )
{
    int i;
    unsigned int index;
    
    for( i = 0; i < work_loop; i++ )
    {
        index = get_int( random_dat_ptr, i % 1024 );
        set_byte( plain_in_ptr, i, get_byte( plain_text_orig_ptr, index ) );
    }
}

int main( int argc, char ** argv )
{
    register uint64_t time1, time2, time_data, time_mmap, time_heap, time_stack;
    unsigned int junk;
    int i;
    int work_loop = 8192;
    
    alloc_plain_buffers();
    
    // ensure all involved data is not going to page fault.
    for( i = 0; i < 1600; i++ )
    {
        plain_text_orig.data[i] = plain_text_test[i];
    }
    
    // create mmap() values
    struct bounded_array * random_dat_mmap = mmap(NULL, ONE_PAGE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    random_dat_mmap->length = 1024;
    random_dat_mmap->mask   = 0x3ff;
    random_dat_mmap->data   = (char *)mmap(NULL, sizeof(random_data), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    for( i = 0; i < sizeof(random_data); i++ )
    {
        random_dat_mmap->data[i] = ((char *)random_data)[i];
    }

    struct bounded_array * plain_text_orig_mmap = mmap(NULL, ONE_PAGE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    plain_text_orig_mmap->length = 1600,
    plain_text_orig_mmap->mask   = 0x7FF;
    plain_text_orig_mmap->data   = (char *)mmap(NULL, sizeof(plain_text_orig_data), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    for( i = 0; i < sizeof(plain_text_orig_data); i++ )
    {
        plain_text_orig_mmap->data[i] = plain_text_orig_data[i];
    }

    struct bounded_array * plain_in_mmap = mmap(NULL, ONE_PAGE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    plain_in_mmap->length = 8192,
    plain_in_mmap->mask   = 0x1FFF;
    plain_in_mmap->data   = (char *)mmap(NULL, sizeof(plain_in_data), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    for( i = 0; i < sizeof(plain_in_data); i++ )
    {
        plain_in_mmap->data[i] = plain_in_data[i];
    }
    
    // create malloc() values
    struct bounded_array * random_dat_heap = malloc( ONE_PAGE );
    random_dat_heap->length = 1024;
    random_dat_heap->mask   = 0x3ff;
    random_dat_heap->data   = (char *)malloc(sizeof(random_data));
    
    for( i = 0; i < sizeof(random_data); i++ )
    {
        random_dat_heap->data[i] = ((char *)random_data)[i];
    }

    struct bounded_array * plain_text_orig_heap = malloc(ONE_PAGE);
    plain_text_orig_heap->length = 1600,
    plain_text_orig_heap->mask   = 0x7FF;
    plain_text_orig_heap->data   = (char *)malloc(sizeof(plain_text_orig_data));
    
    for( i = 0; i < sizeof(plain_text_orig_data); i++ )
    {
        plain_text_orig_heap->data[i] = plain_text_orig_data[i];
    }

    struct bounded_array * plain_in_heap = malloc(ONE_PAGE);
    plain_in_heap->length = 8192,
    plain_in_heap->mask   = 0x1FFF;
    plain_in_heap->data   = (char *)malloc(sizeof(plain_in_data));
    
    for( i = 0; i < sizeof(plain_in_data); i++ )
    {
        plain_in_heap->data[i] = plain_in_data[i];
    }
    
    // create stack values
    char plain_in_data_stack[8192];
    char plain_text_orig_data_stack[1600];
    int  random_data_stack[1024];
    
    struct bounded_array random_dat_stack = {
        1024,
        0x3ff,
        (char *)random_data_stack
    };
    
    struct bounded_array plain_text_orig_stack = {
        1600,
        0x7FF,
        plain_text_orig_data_stack
    };
    
    struct bounded_array plain_in_stack = {
        8192,
        0x1FFF,
        plain_in_data_stack
    };
    
    for( i = 0; i < sizeof(random_data); i++ )
    {
        random_dat_stack.data[i] = ((char *)random_data)[i];
    }

    for( i = 0; i < sizeof(plain_text_orig_data); i++ )
    {
        plain_text_orig_stack.data[i] = plain_text_orig_data[i];
    }

    for( i = 0; i < sizeof(plain_in_data); i++ )
    {
        plain_in_stack.data[i] = plain_in_data[i];
    }
    
    time_data = 0;
    time_mmap = 0;
    time_heap = 0;
    time_stack = 0;
    
    for( i = 0; i < 100; i++ )
    {
        time1 = __rdtscp( & junk);
        do_work( work_loop, &random_dat, &plain_in, &plain_text_orig );
        time2 = __rdtscp( & junk) - time1;
        time_data += time2;
        
        time1 = __rdtscp( & junk);
        do_work( work_loop, random_dat_mmap, plain_in_mmap, plain_text_orig_mmap );
        time2 = __rdtscp( & junk) - time1;
        time_mmap += time2;
        
        time1 = __rdtscp( & junk);
        do_work( work_loop, random_dat_heap, plain_in_heap, plain_text_orig_heap );
        time2 = __rdtscp( & junk) - time1;
        time_heap += time2;
        
        time1 = __rdtscp( & junk);
        do_work( work_loop, &random_dat_stack, &plain_in_stack, &plain_text_orig_stack );
        time2 = __rdtscp( & junk) - time1;
        time_stack += time2;
    }
    
    printf("data time  :[%lu]\n", time_data  );
    printf("mmap time  :[%lu]\n", time_mmap  );
    printf("heap time  :[%lu]\n", time_heap  );
    printf("stack time :[%lu]\n", time_stack );
    
    return 0;
}
