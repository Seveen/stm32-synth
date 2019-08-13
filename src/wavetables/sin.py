import numpy as np
x = np.linspace(0, 2 * np.pi, 256)
sinewave = np.sin(x)

sine_int16 = map(lambda x: int(np.round((x+1.0)*2048)), sinewave)

with open('sin.cc', 'w') as f:
    f.write('#include "waves.h"\n')
    f.write("uint16_t sintable[256] = {\n")
    for sample in sine_int16:
        f.write("   %s,\n" % sample)
    f.write("};\n")