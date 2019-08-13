#include "waves.h"
uint16_t sintable[256] = {
   2048,
   2098,
   2149,
   2199,
   2250,
   2300,
   2350,
   2399,
   2449,
   2498,
   2548,
   2596,
   2645,
   2693,
   2741,
   2788,
   2835,
   2881,
   2927,
   2972,
   3017,
   3061,
   3105,
   3148,
   3190,
   3231,
   3272,
   3312,
   3351,
   3390,
   3428,
   3465,
   3501,
   3536,
   3570,
   3603,
   3636,
   3667,
   3697,
   3727,
   3755,
   3783,
   3809,
   3834,
   3858,
   3881,
   3903,
   3924,
   3944,
   3962,
   3980,
   3996,
   4011,
   4025,
   4037,
   4049,
   4059,
   4068,
   4075,
   4082,
   4087,
   4091,
   4094,
   4096,
   4096,
   4095,
   4093,
   4089,
   4085,
   4079,
   4072,
   4063,
   4054,
   4043,
   4031,
   4018,
   4003,
   3988,
   3971,
   3953,
   3934,
   3914,
   3892,
   3870,
   3846,
   3822,
   3796,
   3769,
   3741,
   3712,
   3682,
   3651,
   3620,
   3587,
   3553,
   3518,
   3483,
   3446,
   3409,
   3371,
   3332,
   3292,
   3252,
   3211,
   3169,
   3126,
   3083,
   3039,
   2995,
   2950,
   2904,
   2858,
   2811,
   2764,
   2717,
   2669,
   2621,
   2572,
   2523,
   2474,
   2424,
   2375,
   2325,
   2275,
   2224,
   2174,
   2124,
   2073,
   2023,
   1972,
   1922,
   1872,
   1821,
   1771,
   1721,
   1672,
   1622,
   1573,
   1524,
   1475,
   1427,
   1379,
   1332,
   1285,
   1238,
   1192,
   1146,
   1101,
   1057,
   1013,
   970,
   927,
   885,
   844,
   804,
   764,
   725,
   687,
   650,
   613,
   578,
   543,
   509,
   476,
   445,
   414,
   384,
   355,
   327,
   300,
   274,
   250,
   226,
   204,
   182,
   162,
   143,
   125,
   108,
   93,
   78,
   65,
   53,
   42,
   33,
   24,
   17,
   11,
   7,
   3,
   1,
   0,
   0,
   2,
   5,
   9,
   14,
   21,
   28,
   37,
   47,
   59,
   71,
   85,
   100,
   116,
   134,
   152,
   172,
   193,
   215,
   238,
   262,
   287,
   313,
   341,
   369,
   399,
   429,
   460,
   493,
   526,
   560,
   595,
   631,
   668,
   706,
   745,
   784,
   824,
   865,
   906,
   948,
   991,
   1035,
   1079,
   1124,
   1169,
   1215,
   1261,
   1308,
   1355,
   1403,
   1451,
   1500,
   1548,
   1598,
   1647,
   1697,
   1746,
   1796,
   1846,
   1897,
   1947,
   1998,
   2048,
};