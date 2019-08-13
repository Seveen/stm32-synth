import numpy as np
triwave = np.concatenate((np.linspace(0, 127, 128), np.linspace(127, 0, 128)))

tri_int16 = map(lambda x: int(np.round(x*32)), triwave)

with open('tri.cc', 'w') as f:
    f.write('#include "waves.h"\n')
    f.write("uint16_t tritable[256] = {\n")
    for sample in tri_int16:
        f.write("   %s,\n" % sample)
    f.write("};\n")